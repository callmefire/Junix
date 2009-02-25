#include <stdlib.h>
#include <stdio.h>
#include <asm-ia32/video.h>

/* export function pointer */
void (*putchar)(int c);

/* video mode */
static unsigned int video_mode;
static video_info_t video_info[MAX_MODE];

#define _FBUF		(video_info[video_mode].buffer)
#define _FBUF_SIZE	(video_info[video_mode].buf_size)
#define _REGS		(video_info[video_mode].regs)
#define _LINES		(video_info[video_mode].lines)
#define _COLUMNS	(video_info[video_mode].columns)
#define _XPOS		(video_info[video_mode].xpos)
#define _YPOS		(video_info[video_mode].ypos)
#define _CXPOS		(video_info[video_mode].cursor_xpos)
#define _CYPOS		(video_info[video_mode].cursor_ypos)
#define _FGCOLOR	(video_info[video_mode].fg_color)
#define _BGCOLOR	(video_info[video_mode].bg_color)
#define _CLS		video_info[video_mode].cls
#define _PUTCHAR	video_info[video_mode].putchar
#define _SET_FGCOLOR	video_info[video_mode].set_fgcolor
#define _SET_BGCOLOR	video_info[video_mode].set_bgcolor
#define _GET_FGCOLOR	video_info[video_mode].get_fgcolor
#define _GET_BGCOLOR	video_info[video_mode].get_bgcolor
#define _SET_CURSOR		video_info[video_mode].set_cursor
#define _GET_CURSOR		video_info[video_mode].get_cursor
#define _SCROLL			video_info[video_mode].scroll

static unsigned char text_regs[VGA_REG_MAX_IDX];

static const unsigned char g320x200x256_regs[VGA_REG_MAX_IDX] =
{
	/* CRT Index 0  -  7 */	  0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 
	/* CRT Index 8  - 15 */	  0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	/* CRT Index 16 - 23 */	  0x9C, 0x8E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
	/* ATT Index 0  -  7 */	  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	/* ATT Index 8  - 15 */	  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
	/* ATT Index 16 - 20 */   0x41, 0x00, 0x0F, 0x00, 0x00,
	/* GRA Index 0  -  7 */	  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 
	/* GRA Index 8		 */   0xFF,
	/* SEQ Index 0  -  4 */   0x03, 0x01, 0x0F, 0x00, 0x0E,
	/* MIS Index 0       */   0x63
};

static const unsigned char g320x240x256_regs[VGA_REG_MAX_IDX] =
{
	/* CRT Index 0  -  7 */	  0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0D, 0x3E, 
	/* CRT Index 8  - 15 */   0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	/* CRT Index 16 - 23 */   0xEA, 0xAC, 0xDF, 0x28, 0x00, 0xE7, 0x06, 0xE3,
	/* ATT Index 0  -  7 */   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	/* ATT Index 8  - 15 */   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
	/* ATT Index 16 - 20 */   0x41, 0x00, 0x0F, 0x00, 0x00,
	/* GRA Index 0  -  7 */   0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 
	/* GRA Index 8       */   0xFF,
	/* SEQ Index 0  -  4 */   0x03, 0x01, 0x0F, 0x00, 0x06,
	/* MIS Index 0       */   0xE3
};

static const unsigned char g640x480x16_regs[VGA_REG_MAX_IDX] =
{
	/* CRT Index 0  -  7 */	  0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E, 
	/* CRT Index 8  - 15 */   0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	/* CRT Index 16 - 23 */   0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	/* ATT Index 0  -  7 */   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	/* ATT Index 8  - 15 */   0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 
	/* ATT Index 16 - 20 */   0x01, 0x00, 0x0F, 0x00, 0x00,
	/* GRA Index 0  -  7 */	  0x00, 0x0F, 0x00, 0x20, 0x00, 0x00, 0x05, 0x0F, 
	/* GRA Index 8       */   0xFF,
	/* SEQ Index 0  -  4 */   0x03, 0x01, 0x0F, 0x00, 0x06,
	/* MIS Index 0  	 */	  0xE3
};

/* Clear the screen and initialize VIDEO, XPOS and YPOS.  */
static void text_cls(void)
{
    int i;

    for (i = 0; i < VGA_TEXT_COLUMNS * VGA_TEXT_LINES * 2; i++)
		*(_FBUF + i) = 0;

    _XPOS = 0;
    _YPOS = 0;
}

static void text_set_fgcolor(int color)
{
	_FGCOLOR = color;
}

static void text_set_bgcolor(int color)
{
	_BGCOLOR = color;
}

static int text_get_fgcolor(void)
{

	return 0;
}

static int text_get_bgcolor(void)
{
	
	return 0;
}

static void text_set_cursor(int x, int y)
{

}

static void text_get_cursor(int *x, int *y)
{

}

static void text_scroll(void)
{
	int i;
	memmove((void *)_FBUF,(void *)(_FBUF + VGA_TEXT_COLUMNS * 2), (VGA_TEXT_LINES - 1) * VGA_TEXT_COLUMNS *2);
	
	for (i = (VGA_TEXT_LINES - 1) * VGA_TEXT_COLUMNS * 2; i < VGA_TEXT_LINES * VGA_TEXT_COLUMNS * 2; i+=2 )
		_FBUF[i] = ' ';
}

/* Put the character C on the screen.  */
static void text_putchar(int c)
{
    if (c == '\n' || c == '\r') {
newline:
		_XPOS = 0;
		_YPOS++;
	
		if (_YPOS >= VGA_TEXT_LINES) {
	    	text_scroll();
			_YPOS--;
		}
		return;
    }

    *(_FBUF + (_XPOS + _YPOS * VGA_TEXT_COLUMNS) * 2) = c & 0xFF;
    *(_FBUF + (_XPOS + _YPOS * VGA_TEXT_COLUMNS) * 2 + 1) = ATTRIBUTE(_BGCOLOR,_FGCOLOR);

    _XPOS++;
    if (_XPOS >= VGA_TEXT_COLUMNS)
		goto newline;
}

static void text_mode_init(void)
{
    _FBUF = (unsigned char *) VIDEO_CTEXT_FBUFFER_BASE;
	_FBUF_SIZE = VIDEO_CTEXT_FBUFFER_SIZE;
	_REGS = text_regs;
	_LINES = VGA_TEXT_LINES;
	_COLUMNS = VGA_TEXT_COLUMNS;
	_XPOS = 0;
	_YPOS = 0;
	_CXPOS = 0;
	_CYPOS = 0;
	_FGCOLOR = CGA_IYELLOW;
	_BGCOLOR = CGA_BLACK;

	_CLS = text_cls;
	_PUTCHAR = text_putchar;
	_SET_FGCOLOR = text_set_fgcolor;
	_SET_BGCOLOR = text_set_bgcolor;
	_GET_FGCOLOR = text_get_fgcolor;
	_GET_BGCOLOR = text_get_bgcolor;
	_SET_CURSOR  = text_set_cursor;
	_GET_CURSOR  = text_get_cursor;
	_SCROLL      = text_scroll;
}

static void g320x200x256_init(void)
{

}

static void g320x240x256_init(void)
{

}

static void g640x480x16_init(void)
{

}

static void set_regs(void)
{

}

static void save_regs(void)
{

}

static void video_info_init(unsigned int mode)
{
	switch (mode) {
		case VGA_TEXT:
			video_info[mode].init = text_mode_init;
			break;
		case G320x200x256:
			video_info[mode].init = g320x200x256_init;
			break;
		case G320x240x256:
			video_info[mode].init = g320x240x256_init;
			break;
		case G640x480x16:
			video_info[mode].init = g640x480x16_init;
			break;
		default:
			break;
	}

	if (video_info[mode].init)
		video_info[mode].init();

	putchar = _PUTCHAR;
}

void video_init(void)
{
	video_mode = CONFIG_VIDEO_MODE;	

	video_info_init(video_mode);
	
	_CLS();
	
}

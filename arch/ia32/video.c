#include <stdlib.h>
#include <stdio.h>
#include <asm-ia32/video.h>

/* Variables.  */
/* Save the X position.  */
static int xpos;
/* Save the Y position.  */
static int ypos;

/* Save the foreground color */
static char fg_color;
static char bg_color;

/* Point to the video framebuffer */
static volatile unsigned char *video;

/* Clear the screen and initialize VIDEO, XPOS and YPOS.  */
static void cls(void)
{
    int i;

	if (!video)
		return;

    for (i = 0; i < COLUMNS * LINES * 2; i++)
		*(video + i) = 0;

    xpos = 0;
    ypos = 0;
}

static void set_fgcolor(char color)
{
	fg_color = color;
}

static void set_bgcolor(char color)
{
	bg_color = color;
}

void scroll(void)
{
	memmove((void *)video,(void *)(video + COLUMNS * 2), (LINES - 1) * COLUMNS *2);
	memset((void *)(video + (LINES - 1) * COLUMNS * 2), 0, COLUMNS * 2);	
}

/* Put the character C on the screen.  */
void putchar(int c)
{
    if (c == '\n' || c == '\r') {

newline:

	xpos = 0;
	ypos++;
	
	if (ypos >= LINES) {
	    scroll();
		ypos--;
	}
	return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE(bg_color,fg_color);

    xpos++;
    if (xpos >= COLUMNS)
		goto newline;
}

void video_init(void)
{
    video = (unsigned char *) VIDEO_FBUFFER;
	
	cls();
	
	set_fgcolor(CGA_IYELLOW);	
	set_bgcolor(CGA_BLACK);	
}

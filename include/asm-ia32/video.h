#ifndef __ASM_VIDEO_H_
#define __ASM_VIDEO_H_

#include <config.h>

#define	VGA_TEXT 		0
#define G320x200x256	1
#define G320x240x256	2
#define G640x480x16	    3
#define MAX_MODE		G640x480x16

typedef struct video_info {
	volatile unsigned char *buffer;	/* Framebuffer */
	unsigned int buf_size;			/* Framebuffer size */
	unsigned char *regs;			/* Register file */
	
	/* Resolution */
	unsigned int lines;
	unsigned int columns;
	
	/* Current position */
	short xpos;
	short ypos;

	/* Cursor location */
	short cursor_xpos;
	short cursor_ypos;
	
	/* Colors */
	int fg_color;
	int bg_color;

	/* Handler */
	void (*init)(void);
	void (*cls)(void);
	void (*putchar)(int);
	void (*set_fgcolor)(int);
	void (*set_bgcolor)(int);
	int  (*get_fgcolor)(void);
	int  (*get_bgcolor)(void);
	void (*set_cursor)(int x,int y);
	void (*get_cursor)(int *x,int *y);
	void (*scroll)(void);

} video_info_t;

/************************* VGA IO space start **************************/

/* VGA index register ports */
#define VGA_ATT_IW  0x3C0       /* Attribute Controller Index & Data Write Register */
#define VGA_SEQ_I   0x3C4       /* Sequencer Index */
#define VGA_PEL_IW  0x3C8       /* PEL Write Index */
#define VGA_PEL_IR  0x3C7       /* PEL Read Index */
#define VGA_GRA_I   0x3CE       /* Graphics Controller Index */
#define VGA_CRT_IC  0x3D4       /* CRT Controller Index - color emulation */

/* VGA data register ports */
#define VGA_ATT_R   0x3C1       /* Attribute Controller Data Read Register */
#define VGA_SEQ_D   0x3C5       /* Sequencer Data Register */
#define VGA_MIS_W   0x3C2       /* Misc Output Write Register */
#define VGA_MIS_R   0x3CC       /* Misc Output Read Register */
#define VGA_GRA_D   0x3CF       /* Graphics Controller Data Register */
#define VGA_CRT_DC  0x3D5       /* CRT Controller Data Register - color emulation */
#define VGA_IS1_RC  0x3DA       /* Input Status Register 1 - color emulation */

#define VGA_PEL_MSK 0x3C6       /* PEL mask register */
#define VGA_PEL_D   0x3C9       /* PEL Data Register */

/* Standard VGA indexes max counts */
#define VGA_REG_CRT_CNT   24      /* 24 CRT Controller Registers */
#define VGA_REG_ATT_CNT   21      /* 21 Attribute Controller Registers */
#define VGA_REG_GRA_CNT   9       /* 9  Graphics Controller Registers */
#define VGA_REG_SEQ_CNT   5       /* 5  Sequencer Registers */
#define VGA_REG_MIS_CNT   1       /* 1  Misc Output Register */

/* Register file index */
#define VGA_REG_CRT_IDX      0	       	  	  /* CRT Controller Registers start */
#define VGA_REG_ATT_IDX     (VGA_REG_CRT_IDX+VGA_REG_CRT_CNT) /* Attribute Controller Registers start */
#define VGA_REG_GRA_IDX     (VGA_REG_ATT_IDX+VGA_REG_ATT_CNT) /* Graphics Controller Registers start */
#define VGA_REG_SEQ_IDX     (VGA_REG_GRA_IDX+VGA_REG_GRA_CNT) /* Sequencer Registers */
#define VGA_REG_MIS_IDX     (VGA_REG_SEQ_IDX+VGA_REG_SEQ_CNT) /* General Registers */
#define VGA_REG_MAX_IDX     (VGA_REG_MIS_IDX+VGA_REG_MIS_CNT) /* Max number: 60 */

/************************* VGA IO space end **************************/

/* The video memory address space for VGA  */
#define VIDEO_MTEXT_FBUFFER_PHY			0xB0000
#define VIDEO_MTEXT_FBUFFER_SIZE		0x8000
#define VIDEO_MTEXT_FBUFFER_BASE		(VIDEO_MTEXT_FBUFFER_PHY+PAGE_OFFSET)

#define VIDEO_CTEXT_FBUFFER_PHY			0xB8000
#define VIDEO_CTEXT_FBUFFER_SIZE		0x8000
#define VIDEO_CTEXT_FBUFFER_BASE		(VIDEO_CTEXT_FBUFFER_PHY+PAGE_OFFSET)

#define VIDEO_GRAPHIC_FBUFFER_PHY		0xA0000
#define VIDEO_GRAPHIC_FBUFFER_SIZE		0x10000
#define VIDEO_GRAPHIC_FBUFFER_BASE		(VIDEO_GRAPHIC_FBUFFER_PHY+PAGE_OFFSET)

/*********************** TEXT mode info ************************/

/* The number of columns */
#define VGA_TEXT_COLUMNS			80
/* The number of lines */
#define VGA_TEXT_LINES			25

/* The attribute of an character */
#define ATTRIBUTE_DEFAULT   0x7
#define ATTRIBUTE(bg,fg)	( (bg << 4) | fg )

/************* CGA Color *************
Black               0000           00
Blue                0001           01
Green               0010           02
Cyan                0011           03
Red                 0100           04
Violet              0101           05
Yellow (brown)      0110           06
White               0111           07
Black (gray)        1000           08
Intense blue        1001           09
Intense green       1010           0A
Intense cyan        1011           0B
Intense red         1100           0C
Intense violet      1101           0D
Intense yellow      1110           0E
Intense white 		1111           0F
*************************************/

#define CGA_BLACK	0x00
#define CGA_BLUE	0x01
#define CGA_GREEN	0x02
#define CGA_CYAN	0x03
#define CGA_RED		0x04
#define CGA_VIOLET	0x05
#define CGA_YELLOW	0x06
#define CGA_WHITE	0x07
#define CGA_GRAY	0x08
#define CGA_IBLUE	0x09
#define CGA_IGREEN	0x0A
#define CGA_ICYAN	0x0B
#define CGA_IRED	0x0C
#define CGA_IVIOLET	0x0D
#define CGA_IYELLOW	0x0E
#define CGA_IWHITE	0x0F

/*********************** TEXT mode info end ************************/

/*********************** Graphic mode info ************************/
#define G320_LINES	 320
#define G640_LINES	 640
#define G200_COLUMNS 200
#define G240_COLUMNS 240
#define G480_COLUMNS 480

/*********************** Graphic mode info end ************************/

extern void video_init(void);
extern void (*putchar)(int c);
#endif

#ifndef __ASM_VIDEO_H_
#define __ASM_VIDEO_H_

/* Some screen stuff.  */
/* The number of columns.  */
#define COLUMNS			80
/* The number of lines.  */
#define LINES			24

/* The attribute of an character.  */
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
Intense white 		1110           0F
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

/* The video memory address.  */
#define VIDEO_FBUFFER			0xC00B8000

extern void video_init(void);
extern void putchar(int c);
#endif

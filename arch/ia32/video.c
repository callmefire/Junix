#include <asm-ia32/video.h>

/* Variables.  */
/* Save the X position.  */
static int xpos;
/* Save the Y position.  */
static int ypos;
/* Point to the video memory.  */
static volatile unsigned char *video;

/* Clear the screen and initialize VIDEO, XPOS and YPOS.  */
void cls(void)
{
    int i;

    video = (unsigned char *) VIDEO;

    for (i = 0; i < COLUMNS * LINES * 2; i++)
	*(video + i) = 0;

    xpos = 0;
    ypos = 0;
}

/* Put the character C on the screen.  */
void putchar(int c)
{
    if (c == '\n' || c == '\r') {
newline:
	xpos = 0;
	ypos++;
	if (ypos >= LINES)
	    ypos = 0;
	return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

    xpos++;
    if (xpos >= COLUMNS)
	goto newline;
}


#ifndef __STDLIB_H_
#define __STDLIB_H_

#define NULL	(void *)0

extern void itoa(char *buf, int base, int d);

extern void *memset(void *s, int c, unsigned int n);
extern void *memmove(void *dest, const void *src, unsigned int n);
extern void *memcpy(void *dest, const void *src, unsigned int n);

#endif

/* libc */

void itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put `-' in the head.  */
    if (base == 'd' && d < 0) {
	*p++ = '-';
	buf++;
	ud = -d;
    } else if (base == 'x')
	divisor = 16;

    /* Divide UD by DIVISOR until UD == 0.  */
    do {
	int remainder = ud % divisor;

	*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* Terminate BUF.  */
    *p = 0;

    /* Reverse BUF.  */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
	char tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
	p1++;
	p2--;
    }
}

void *memset(void *s, int c, unsigned int count)
{
	char *xs = s;
	while (count--)
		*xs++ = c;
	return s;
}

void *memmove(void *dest, const void *src, unsigned int n)
{
	char *d = (char *)dest;
	char *s = (char *)src;

	if (d <= s) {
		while (n--)
			*d++ = *s++;
	} else {
		while (n--)
			d[n-1] = s[n-1];
	}	

	return dest;
}

void *memcpy(void *dest, const void *src, unsigned int n)
{
	char *d = (char *)dest;
	char *s = (char *)src;

	while (n--) 
		*d++ = *s++;
	
	return dest;
}

int memcmp(const void *s1,const void *s2,unsigned int n)
{
	char *t1 = (char *)s1;
	char *t2 = (char *)t2;

	while (n--) {
		if (*t1 != *t2) {
			return (int)(*t1 - *t2);
		}
		t1++;
		t2++;
	}

	return 0;
}

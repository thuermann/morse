/*
 * $Id: morse.c,v 1.1 1999/01/19 09:01:22 urs Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void code(char *s);
void decode(char *s);

int main(int argc, char **argv)
{
    int decode_flag = 0;

    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
	argv++;
	decode_flag = 1;
    }
    while (*++argv)
	if (decode_flag)
	    decode(*argv);
	else
	    code(*argv);
    putchar('\n');

    return 0;
}

char mtab[] = "  etinamsdrgukwohblzfcp vx q yj";

void code(char *s)
{
    char c, *p;
    int q;

    while (c = tolower(*s++)) {
	if (!(p = strchr(mtab, c)))
	    continue;
	for (q = p - mtab; q > 1; q >>= 1)
	    putchar(".-"[q & 1]);
	putchar(' ');
    }
}

void decode(char *s)
{
    int q = 0, bit = 1;
    char c;

    while (c = *s++) {
	if (c == '.')
	    bit <<= 1;
	else if (c == '-') {
	    q |= bit;
	    bit <<= 1;
	} else if (c == ' ') {
	    q |= bit;
	    putchar(mtab[q]);
	    q = 0;
	    bit = 1;
	}
    }
    q |= bit;
    putchar(mtab[q]);
}

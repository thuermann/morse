/*
 * $Id: morse.c,v 1.2 2001/01/04 08:50:18 urs Exp $
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

/*             0123456789abcdef0123456789abcdef */
char mtab[] = "  etinamsdrgukwohblzfcp vx q yj "
	      "56 7   8       94       3   2 10"
	      "            ?                   "
	      "          .        ,            ";

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

/*
 * $Id: morse.c,v 1.3 2013/08/02 11:21:46 urs Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void encode(const char *s);
static void decode(const char *s);

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
	    encode(*argv);
    putchar('\n');

    return 0;
}

/*                          0123456789abcdef0123456789abcdef */
static const char mtab[] = "  etinamsdrgukwohblzfcp vx q yj "
			   "56 7   8       94       3   2 10"
			   "            ?                   "
			   "          .        ,            ";

static void encode(const char *s)
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

static void decode(const char *s)
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

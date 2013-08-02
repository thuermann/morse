/*
 * $Id: morse.c,v 1.5 2013/08/02 11:22:03 urs Exp $
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

/* Create an unsigned integer from a binary digit string of up to 8
 * digits, e.g. BIN(1010) will expand to an unsigned integer
 * const-expression with value 10.
 */
#define OCT(b)      0 ## b
#define BIT(n, k)   (OCT(n) >> 2 * (k) & 1 << (k))
#define BIN(n)      (BIT(n, 7) | BIT(n, 6) | BIT(n, 5) | BIT(n, 4) | \
		     BIT(n, 3) | BIT(n, 2) | BIT(n, 1) | BIT(n, 0))

/* Some macros for bit-reversal of an 8-bit integer. */
#define REV1(n)     ((((n) & 0xaa) >> 1) | (((n) & 0x55) << 1))
#define REV2(n)     ((((n) & 0xcc) >> 2) | (((n) & 0x33) << 2))
#define REV4(n)     ((((n) & 0xf0) >> 4) | (((n) & 0x0f) << 4))
#define REV8(n)     REV4(REV2(REV1(n)))
#define REV(n, len) (REV8(n) >> (8 - (len)))

/* Compute the index into the code table by appending the bit 1 and
 * then reversing the resulting bit string.
 */
#define LEN(b)      (sizeof(#b) - 1)
#define IDX(b)      (REV(BIN(b ## 1), LEN(b ## 1)))
#define M(c, b)     [IDX(b)] = c

static const char mtab[] = {
    M(' ', /* EMPTY */),   /* This is tricky: expands to index 1. */
    M('a', 01),     M('n', 10),     M('1', 01111),
    M('b', 1000),   M('o', 111),    M('2', 00111),
    M('c', 1010),   M('p', 0110),   M('3', 00011),
    M('d', 100),    M('q', 1101),   M('4', 00001),
    M('e', 0),      M('r', 010),    M('5', 00000),
    M('f', 0010),   M('s', 000),    M('6', 10000),
    M('g', 110),    M('t', 1),      M('7', 11000),
    M('h', 0000),   M('u', 001),    M('8', 11100),
    M('i', 00),     M('v', 0001),   M('9', 11110),
    M('j', 0111),   M('w', 011),    M('0', 11111),
    M('k', 101),    M('x', 1001),   M('.', 010101),
    M('l', 0100),   M('y', 1011),   M(',', 110011),
    M('m', 11),     M('z', 1100),   M('?', 001100),
};

/* Encode and decode by looking up the character or the morse code,
 * resp., in the morse table and printing the result.  Blanks are
 * handled specially: In encoding, the blank maps to the empty morse
 * code, i.e. index 1, which produces no dit or dah, thus we get a
 * longer pause.  In decoding, two consecutive blanks produce an empty
 * morse code which maps to a blank in the output.
 */
static void encode(const char *s)
{
    char c, *p;
    int q;

    while (c = tolower(*s++)) {
	if (!(p = memchr(mtab, c, sizeof(mtab))))
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

    do {
	c = *s++;
	if (c == '.')
	    bit <<= 1;
	else if (c == '-') {
	    q |= bit;
	    bit <<= 1;
	} else if (c == ' ' || c == 0) {
	    q |= bit;
	    if (q < sizeof(mtab) && mtab[q])
		putchar(mtab[q]);
	    q = 0;
	    bit = 1;
	}
    } while (c);
}

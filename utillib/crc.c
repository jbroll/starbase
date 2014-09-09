/*	Computing a POLY number from the crc equation.
 *	Crc's are usually expressed as an polynomial expression such as:
 *
 *		x^16 + x^12 + x^5 + 1
 *
 *	Since the highest order term is to the power 16 this is a
 *	16 bit crc.  The POLY number is determined by setting bits
 *	corresponding to the power terms in the polynomial equation
 *	in an integer.  To do this we number the bits of the integer
 *	with the most significant bit = bit 0. CAUTION: this is the
 *	opposite to the some bit numbering schemes.  This is due to
 *	the least significant bit first convention of crc calculations.
 *	The above equation becomes:
 *
 *	  1   0   0   0   0   1   0   0   0   0   0   0   1   0   0   0
 *	|___.___.___.___|___.___.___.___|___.___.___.___|___.___.___.___|
 *	  0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
 *	msb                                                          lsb
 *
 *	Note: the "1" term is equivalent to "x^0" and the "x^16"
 *	term is ignored (it does determine the length to be 16 bits).
 *	Thus the POLY number is 0x8408 (ccitt crc).
 */

/* crc16 - DDCMP and Bisync (16 bit)
 *	x^16 + x^15 + x^2 + 1
 */
#define	POLY_CRC16	0xa001
#define	INIT_CRC16	0

/* CCITT - ADCCP, HDLC, SDLC (16 bit)
 *	x^16 + x^12 + x^5 + 1
 */
#define POLY_CCITT	0x8408
#define	INIT_CCITT	-1


/* a bit at a time */
unsigned crc_16(in)
	char *in;
{
	register unsigned short crc, bit;
	register int i;

	crc = INIT_CRC16;
	while (*in) {
		for (i=1; i <= 0x80; i<<=1) {
			bit = (((*in)&i)?0x8000:0);
			if (crc & 1) bit ^= 0x8000;
			crc >>= 1;
			if (bit) crc ^= (int)(POLY_CRC16);
  		}
		++in;
	}
	return crc;
}


/* For even more fun - ccitt() crc routine */

/* nibble table for CCITT crc */
unsigned int ccitt_h[] = {
0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
0x8408, 0x9489, 0xa50a, 0xb58b, 0xc60c, 0xd68d, 0xe70e, 0xf78f, };

unsigned int ccitt_l[] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, };

unsigned short
ccitt(in, size)
	register unsigned char *in;
	register int		size;
{
	register unsigned int	n, crc;

	crc = -1;
	while ( size-- ) {
		n = *in++ ^ crc;
		crc = ccitt_l[n&0x0f] ^ ccitt_h[(n>>4)&0x0f] ^ (crc>>8);
	};
	return(crc);
}

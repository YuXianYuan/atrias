#include <crc.h>

/*
 * CRC lookup table generator and calculator adapted from
 * http://www.zorc.breitbandkatze.de/crc.html
 */

const uint8_t CRC_ORDER = 32;
const crc_t CRC_HIGHBIT = (crc_t) (1UL << 31);
const crc_t CRC_POLY = (crc_t) (0x04c11db7);
const crc_t CRC_INIT = (crc_t) (0xffffffff);

void crc_generate_table(void)
{
	/*
	 * Generate CRC lookup table
	 */
	uint8_t i;
	uint16_t dividend;
	crc_t remainder;

	for (dividend=0; dividend<256; dividend++) {
		remainder = (crc_t) dividend;
		remainder <<= CRC_ORDER-8;

		for (i=0; i<8; i++) {
			if (remainder & CRC_HIGHBIT) {;
				remainder = (remainder << 1) ^ CRC_POLY;
			}
			else {
				remainder = (remainder << 1);
			}
		}
		crc_table[dividend] = remainder;
	}
}

crc_t crc_calc(uint8_t *packet, uint8_t num_bytes)
{
	/*
	 * Fast lookup table algorithm without augmented zero bytes, e.g. used in
	 * pkzip. Only usable with polynom orders of 8, 16, 24 or 32.
	 */
	crc_t crc = CRC_INIT;

	/* Divide packet by polynomial one byte at a time. */
	while (num_bytes--) {
		crc = (crc << 8) ^ crc_table[ ((crc >> 24) & 0xff) ^ *packet++];
	}

	return crc;
}

uint8_t is_packet_good(crc_t my_crc, crc_t kvh_crc)
{
	return (my_crc == kvh_crc);
}



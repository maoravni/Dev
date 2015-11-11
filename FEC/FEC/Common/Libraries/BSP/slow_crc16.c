/**************************************************
 * Copyright 2007 IAR Systems. All rights reserved.
 *
 * $Revision: 15715 $
 *
 * A simple implementation of the crc-16 checksum algorithm.
 *
 * Note that if you use this algorithm/function, you need to call it with
 * an extra number of zero bytes equal to the size of the checksum
 * (in this case 2), to "rotate out the answer".
 *
 * Example of usage:
 *
 * unsigned short calc;
 * // Run the checksum algorithm
 * calc = slow_crc16(0, p, size);
 * // Rotate out the answer
 * unsigned char zeroes[2] = {0, 0};
 * calc = slow_crc16(calc, zeroes, 2);
 * // Test the checksum
 * if (calc != the_checksum)
 * {
 *   // Some error handling...
 * }
 *
 **************************************************/

unsigned short slow_crc16(unsigned short sum, unsigned char *p, unsigned int len)
{
  while (len--)
  {
    int i;
    unsigned char byte = *(p++);
    for (i = 0; i < 8; ++i)
    {
      unsigned long osum = sum;
      sum <<= 1;
      if (byte & 0x80)
        sum |= 1;
      if (osum & 0x8000)
        sum ^= 0x1021;
      byte <<= 1;
    }
  }
  return sum;
}

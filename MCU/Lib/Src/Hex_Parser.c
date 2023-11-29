/*
 * Hex_Parser.c
 *
 *  Created on: Oct 27, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include <string.h>

/*	SELF	*/
#include "Hex_Parser.h"

/*
 * parses a character to its 4-bits equivalent value.
 * returns true if the input character is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseHalfByte(char ch, u8* valPtr)
{
	if (ch >= '0'		&&		ch <= '9')
	{
		*valPtr = ch - '0';
		return true;
	}
	else if (ch >= 'A'		&&		ch <= 'F')
	{
		*valPtr = ch - ('A' - 10);
		return true;
	}
	else
	{
		return false;
	}
}

/*
 * parses two characters to their 8-bits equivalent value.
 * these two characters are: "strRecord[endIndex-1]", "strRecord[endIndex]"
 * respectively.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseByte(char* strRecord, u8 endIndex, u8* valPtr)
{
	u8 lHalfByte;
	u8 hHalfByte;

	if (!Hex_Parser_b8ParseHalfByte(strRecord[endIndex], &lHalfByte))
		return false;

	if (!Hex_Parser_b8ParseHalfByte(strRecord[endIndex - 1], &hHalfByte))
		return false;

	*valPtr = lHalfByte | (hHalfByte << 4);

	return true;
}

/*
 * parses four characters to their 16-bits equivalent value.
 * these four characters are: {"strRecord[endIndex-3]",...,
 * "strRecord[endIndex]"} respectively.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseHalfWord(char* strRecord, u8 endIndex, u16* valPtr)
{
	u8 lByte;
	u8 hByte;

	if (!Hex_Parser_b8ParseByte(strRecord, endIndex, &lByte))
		return false;

	if (!Hex_Parser_b8ParseByte(strRecord, endIndex - 2, &hByte))
		return false;

	*valPtr = lByte | (hByte << 8);

	return true;
}

/*
 * parses a complete record.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8Parse(char* strRecord, Hex_Record_t* record)
{
	/*	check that the record starts with ':'	*/
	if (strRecord[0] != ':')
	{
		return false;
	}

	/*	parse char count	*/
	if (!Hex_Parser_b8ParseByte(strRecord, 2, &(record->charCount)))
		return false;

	/*	parse lowPartAddress	*/
	if (! Hex_Parser_b8ParseHalfWord(strRecord, 6, &(record->lowPartAddress)))
		return false;

	/*	parse record type	*/
	if (!Hex_Parser_b8ParseByte(strRecord, 8, &(record->type)))
		return false;

	/*	parse data	*/
	u8* dataByteArr = (u8*)(record->data);
	for (u8 i = 0; i < record->charCount; i++)
	{
		if (!Hex_Parser_b8ParseByte(strRecord, 2 * i + 10, &dataByteArr[i]))
		{
			return false;
		}
	}

	/*	parse checksum	*/
	u8 ckecksumIn;
	if (
		!Hex_Parser_b8ParseByte(
			strRecord, 2 * record->charCount + 10, &ckecksumIn)
		)
	{
		return false;
	}

	/*	validate checksum	*/
	u8 checksumCalculated = Hex_Parser_u8CalculateCheckSum(record);
	if (checksumCalculated != ckecksumIn)
	{
		return false;
	}
	return true;
}

/*
 * from: "https://en.wikipedia.org/wiki/Intel_HEX#Checksum_calculation":
 *
 * "A record's checksum byte is the two's complement of the least significant
 * byte (LSB) of the sum of all decoded byte values in the record preceding the
 * checksum. It is computed by summing the decoded byte values and extracting
 * the LSB of the sum (i.e., the data checksum), and then calculating the two's
 * complement of the LSB (e.g., by inverting its bits and adding one)."
 */
u8 Hex_Parser_u8CalculateCheckSum(Hex_Record_t* record)
{
	u16 checksum =
		(u16)record->charCount +
		(u16)record->type +
		(record->lowPartAddress & 0xFF) +
		(record->lowPartAddress >> 8);

	u8* dataByteArr = (u8*) record->data;
	for (u8 i = 0; i < record->charCount; i++)
	{
		checksum += dataByteArr[i];
	}

	checksum &= 0xFF;

	return ((~checksum) + 1);
}


















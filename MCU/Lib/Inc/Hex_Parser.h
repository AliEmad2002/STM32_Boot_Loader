/*
 * Hex_Parser.h
 *
 *  Created on: Oct 27, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_HEX_PARSER_H_
#define INCLUDE_LIB_HEX_PARSER_H_

typedef enum{
	Hex_Record_Type_Data,
	Hex_Record_Type_EndOfFile,
	Hex_Record_Type_HighPartAddress = 4,
	Hex_Record_Type_StartingExecutionAddress
}Hex_Record_Type_t;

typedef struct{
	u8 charCount;
	u16 lowPartAddress;
	Hex_Record_Type_t type;
	u16 data[8];
}Hex_Record_t;


/*
 * parses a character to its 4-bits equivalent value.
 * returns true if the input character is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseHalfByte(char ch, u8* valPtr);

/*
 * parses two characters to their 8-bits equivalent value.
 * these two characters are: "strRecord[endIndex-1]", "strRecord[endIndex]"
 * respectively.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseByte(char* strRecord, u8 endIndex, u8* valPtr);

/*
 * parses four characters to their 16-bits equivalent value.
 * these four characters are: {"strRecord[endIndex-3]",...,
 * "strRecord[endIndex]"} respectively.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8ParseHalfWord(char* strRecord, u8 endIndex, u16* valPtr);

/*
 * parses a complete record.
 * returns true if the input is valid and could be parsed,
 * returns false otherwise.
 */
b8 Hex_Parser_b8Parse(char* strRecord, Hex_Record_t* record);

/*
 * from: "https://en.wikipedia.org/wiki/Intel_HEX#Checksum_calculation":
 *
 * "A record's checksum byte is the two's complement of the least significant
 * byte (LSB) of the sum of all decoded byte values in the record preceding the
 * checksum. It is computed by summing the decoded byte values and extracting
 * the LSB of the sum (i.e., the data checksum), and then calculating the two's
 * complement of the LSB (e.g., by inverting its bits and adding one)."
 */
u8 Hex_Parser_u8CalculateCheckSum(Hex_Record_t* record);



#endif /* INCLUDE_LIB_HEX_PARSER_H_ */

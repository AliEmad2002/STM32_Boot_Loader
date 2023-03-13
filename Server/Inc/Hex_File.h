/*
 * Hex_File.h
 *
 *  Created on: Mar 7, 2023
 *      Author: Ali Emad
 */

#ifndef INC_HEX_FILE_H_
#define INC_HEX_FILE_H_

class Hex_File_t{
private:
	char* dir;

public:
	/*
	 * Constructor: allocates and writes "dir".
	 */
	Hex_File_t(char* dirStr);

	/*
	 * Constructor: takes input from console, allocates and writes "dir".
	 */
	Hex_File_t(void);

	/*
	 * Destructor: deletes "dir" array.
	 */
	~Hex_File_t(void);

	/*
	 * Splits this file to smaller files of selected size and saves them on the
	 * hard disk.
	 *
	 * returns true if succeeded, false otherwise.
	 */
	bool split_to_chunks(int chunkSizeInBytes, char* outputChunksDir);
};



#endif /* INC_HEX_FILE_H_ */

/*
 * main.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Ali Emad
 */

#include <iostream>
#include <string.h>
#include "Hex_File.h"

using namespace std;

#define CHUNK_SIZE_IN_BYTES		900

#define TEMP_FOLDER_DIR			"C:\\Users\\ali20\\Desktop\\gfdQueues"

int main(void)
{
	/*	create file object (gets dir from user)	*/
	Hex_File_t programFile("C:\\Users\\ali20\\Desktop\\PythonApplication1\\PythonApplication1\\Blinky_Example.hex");

	/*	split input file to chunks	*/
	if (!programFile.split_to_chunks(CHUNK_SIZE_IN_BYTES, TEMP_FOLDER_DIR))
	{
		cout << "program failed!\n";
		exit(-1);
	}




	while(1);

	return 0;
}

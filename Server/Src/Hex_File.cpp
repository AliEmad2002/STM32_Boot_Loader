/*
 * Hex_File.cpp
 *
 *  Created on: Mar 7, 2023
 *      Author: Ali Emad
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Hex_File.h"

using namespace std;

#define MAX_STTR_LEN	128

Hex_File_t::Hex_File_t(char* dirStr)
{
	dir = new char[strlen(dirStr) + 1];

	strcpy(dir, dirStr);
}

Hex_File_t::Hex_File_t(void)
{
	char* tempStr = new char[MAX_STTR_LEN];

	cout << "enter .hex file directory (or drag and drop in this console):\n";

	cin.getline(tempStr, MAX_STTR_LEN);

	dir = new char[strlen(tempStr) + 1];

	strcpy(dir, tempStr);

	delete[] tempStr;
}


Hex_File_t::~Hex_File_t(void)
{
	delete[] dir;
}

bool Hex_File_t::split_to_chunks(int chunkSizeInBytes, char* outputChunksDir)
{
	/*	if output folder does not exist, create it	*/
	struct stat info;
	stat(outputChunksDir, &info);

	if(!(info.st_mode & S_IFDIR))
	{
		/*	if dir can not be created	*/
		if (mkdir(outputChunksDir) == -1)
			return false;
	}

	/*	open input (self) file	*/
	fstream programFile;
	programFile.open(dir);

	/*	if file could not be opened	*/
	if (!programFile)
		return false;

	/*	create chunks	*/
	char* chunkDir = new char[MAX_STTR_LEN];

	char* tempLine = new char[MAX_STTR_LEN];

	for (int chunkCount = 0; !programFile.eof(); chunkCount++)
	{
		sprintf(chunkDir, "%s\\%d.hex", outputChunksDir, chunkCount);

		ofstream chunkFile;
		chunkFile.open(chunkDir);

		int chunkSizeCurrent = 0;

		while (chunkSizeCurrent < chunkSizeInBytes)
		{
			programFile >> tempLine;

			if (programFile.eof())
				break;

			chunkFile << tempLine << endl;

			chunkSizeCurrent += strlen(tempLine);
		}

		chunkFile.close();
	}

	delete[] chunkDir;
	delete[] tempLine;

	return true;
}





















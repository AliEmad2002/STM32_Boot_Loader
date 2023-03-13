/*
 * Boot_Loader_private.h
 *
 *  Created on: Oct 27, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_APP_BOOT_LOADER_PRIVATE_H_
#define INCLUDE_APP_BOOT_LOADER_PRIVATE_H_


#define BOOT_LOADER_SIZE_IN_KB		24

typedef enum{
	BootLoader_ChunkParsingResult_Failed,

	BootLoader_ChunkParsingResult_Successful,

	BootLoader_ChunkParsingResult_WasLast	// means that the parsed chunk is
											// the last in the program (contains
											// "EndOfFile" record.
}BootLoader_ChunkParsingResult_t;

b8 Boot_Loader_b8GetVersionAvailableOnline(void);

u16 Boot_Loader_u16GetVersionAvailableOnFlash(void);

b8 Boot_Loader_b8RestartFtpConnection(void);

/*
 * Downloads i-th chuck to esp8266's buffer.
 *
 * Note: this function is void and does not return unless it succeeds in downloading
 * the required chunk. Because, at the point of downloading chunks, the old program
 * would have been erased from flash. Hence, there's nothing else to do (no program
 * to run) if a chunk of the new program can't be downloaded.
 */
void Boot_Loader_voidDownloadChunk(u8 chunkIndex);

/*
 * Parses, flashes the last downloaded chunk (available in esp8266's buffer)
 *
 * Note: execution starting address (if was in one of the downloaded records)
 * is stored in the very last word of the bootloader flash section. Could be
 * later got using: "Boot_Loader_u32GetStoredStartingExecutionAddress()"
 */
BootLoader_ChunkParsingResult_t Boot_Loader_enumParseLastDownloadedChunk(void);



#endif /* INCLUDE_APP_BOOT_LOADER_PRIVATE_H_ */

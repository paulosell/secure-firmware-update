/*
 * flashman.h
 *
 *  Created on: 16 de fev de 2021
 *      Author: sellp
 */

#ifndef FLASHMANAGER_FLASHMAN_H_
#define FLASHMANAGER_FLASHMAN_H_

#include <iostream>
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

/* USER MUST DEFINE PARTITIONS ADDRESS*/
#define FW_START_PAGE					0
#define FW_START_ADDRESS				((uint32_t)0x08040000)
#define FW_END_PAGE						63
#define FW_END_ADDRESS					((uint32_t)0x0805FFFF)

#define NEW_FW_START_PAGE 			    64
#define NEW_FW_START_ADDRESS			((uint32_t)0x08060000)
#define NEW_FW_END_PAGE					127
#define NEW_FW_END_ADDRESS				((uint32_t)0x0807FFFF)

#define NUM_OF_PAGES					64
#define PAGE_SIZE 						2048

union WORD_t {
	uint8_t word_8;
	uint16_t word_16;
	uint32_t word_32;
	uint64_t word_64;

};

typedef struct {
	WORD_t word;

} DATA_t;

class FlashManager {

public:

	enum STATUS_t {
		FAIL = 0, SUCCESS = 1
	};

	FlashManager();
	virtual STATUS_t flashWrite(DATA_t address, DATA_t word) = 0;
	virtual STATUS_t flashErase(DATA_t address) = 0;
	virtual STATUS_t flashLock(void) = 0;
	virtual STATUS_t flashUnlock(void) = 0;

	STATUS_t flashReadAll(DATA_t address, uint8_t *out, size_t out_len);
	bool isFlashLocked();
	void setFlashLocked(bool flashLocked);

private:
	bool flash_locked;
};

#endif /* FLASHMANAGER_FLASHMAN_H_ */

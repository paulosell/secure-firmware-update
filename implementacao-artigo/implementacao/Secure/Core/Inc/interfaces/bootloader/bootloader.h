/*
 * bootloader.h
 *
 *  Created on: 17 de fev de 2021
 *      Author: Paulo Sell
 */

#ifndef INTERFACES_BOOTLOADER_BOOTLOADER_H_
#define INTERFACES_BOOTLOADER_BOOTLOADER_H_

#include <iostream>

/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

class Bootloader{
public:

	enum STATE_t{
		VERIFICATION = 0x02,
		CONFIRMATION = 0x03,
		UPDATE_PREPARATION = 0x04,
		UPDATE_RECEPTION = 0x05,
		UPDATE_VALIDATION = 0x06,
		UPDATE_INSTALLATION = 0x07
	};

	enum COMMAND_t{
		ACK = 0x00,
		RESET = 0x01,
		STATE = 0x02,
		BOOTLOADER_START = 0x04,
		BOOTLOADER_WRITE = 0x05,
		BOOTLOADER_DONE = 0x06,
		ERROR = 0x08
	};


	enum STATUS_t{
		FAIL = 0,
		SUCCESS = 1
	};


	Bootloader();
	/* Should be used during secure boot */
	virtual STATUS_t checkFirmwareIntegrity(void) = 0;


	/* Should be used during secure update */
	virtual STATUS_t checkNewFirmwareSignature(void) = 0;
	virtual STATUS_t checkNewFirmwareVersion(void) = 0;
	virtual STATUS_t receiveNewFirmware(void) = 0;
	virtual STATUS_t hasToUpdate(void) = 0;
	virtual void launchFirmware(void) = 0;

	STATE_t getState(void);
	STATUS_t setState(STATE_t state);

	STATUS_t getIsReadyToBoot(void);
	STATUS_t setIsReadyToBoot(STATUS_t status);



private:

	STATE_t state;
	STATUS_t is_ready_to_boot;
};




#endif /* INTERFACES_BOOTLOADER_BOOTLOADER_H_ */

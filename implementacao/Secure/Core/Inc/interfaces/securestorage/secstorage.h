/*
 * securestorage.h
 *
 *  Created on: 16 de fev de 2021
 *      Author: Paulo Sell
 */

#ifndef SECURESTORAGE_SECSTORAGE_H_
#define SECURESTORAGE_SECSTORAGE_H_

#include <iostream>
#include "../flashmanager/flashman.h"

/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

/* USER MUST DEFINE ASSETS ADDRESS*/
#define ATTESTATION_KEY_PAGE					120
#define MODULUS_ADDRESS  						((uint32_t)0x0803C000)
#define PUBLIC_EXPONENT_ADDRESS 				(MODULUS_ADDRESS + 256) //0x0803C100

#define FIRMWARE_ASSETS_PAGE					121
#define FIRMWARE_HASH_ADDRESS   				((uint32_t)0x0803C800)
#define FIRMWARE_SIGNATURE_ADDRESS 				(FIRMWARE_HASH_ADDRESS + 32)  //0x0803C820
#define FIRMWARE_VERSION_ADDRESS				(FIRMWARE_SIGNATURE_ADDRESS + 256) //0x0803C920
#define FIRMWARE_SIZE_ADDRESS					(FIRMWARE_VERSION_ADDRESS + 16) //0x0803C930

#define NEW_FIRMWARE_ASSETS_PAGE	   			 122
#define NEW_FIRMWARE_HASH_ADDRESS   			((uint32_t)0x0803D000)
#define NEW_FIRMWARE_SIGNATURE_ADDRESS 			(NEW_FIRMWARE_HASH_ADDRESS + 32) // 0x0803D020
#define NEW_FIRMWARE_VERSION_ADDRESS			(NEW_FIRMWARE_SIGNATURE_ADDRESS + 256) //0x0803D120
#define NEW_FIRMWARE_SIZE_ADDRESS				(NEW_FIRMWARE_VERSION_ADDRESS + 16) // 0x0803D130

#define UPDATE_REQUEST_PAGE 					123
#define UPDATE_REQUEST_ADDRESS  				((uint32_t)0x0803D800)

class SecureStorage {

public:

	enum ASSET_t {
		PUBLIC_EXPONENT = 1,
		MODULUS = 3,
		FIRMWARE_HASH = 4,
		FIRMWARE_SIGNATURE = 5,
		FIRMWARE_VERSION = 6,
		NEW_FIRMWARE_HASH = 9,
		NEW_FIRMWARE_SIGNATURE = 10,
		NEW_FIRMWARE_VERSION = 11,
		FIRMWARE_SIZE = 12,
		NEW_FIRMWARE_SIZE = 13,
		UPDATE_REQUEST = 14,

	};

	enum STATUS_t {
		FAIL = 0, SUCCESS = 1
	};

	SecureStorage();
	virtual STATUS_t readData(DATA_t address, uint8_t *out, ASSET_t asset) ;
	virtual STATUS_t writeData(DATA_t address, DATA_t data) ;
	virtual STATUS_t eraseData(DATA_t address);

	/*
	 * User might need to add specific platform methods
	 */

};

#endif /* SECURESTORAGE_SECSTORAGE_H_ */

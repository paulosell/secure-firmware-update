/*
 * boot.h
 *
 *  Created on: 2 de mar de 2021
 *      Author: Paulo Sell
 */

#ifndef INC_ESPECIALIZACOES_BOOTLOADER_BOOT_H_
#define INC_ESPECIALIZACOES_BOOTLOADER_BOOT_H_

#include <especializacoes/comm/usb.h>
#include <especializacoes/flash/flash.h>
#include "interfaces/bootloader/bootloader.h"

class Boot : public Bootloader{
public:

	Boot();
	STATUS_t checkFirmwareIntegrity(void);

	STATUS_t checkNewFirmwareSignature(void);
	STATUS_t checkNewFirmwareVersion(void);
	STATUS_t receiveNewFirmware(void);
	STATUS_t hasToUpdate(void);
	void launchFirmware(void);

	STATUS_t validateNewFirmware(void);
	STATUS_t finishUpdate(void);
	STATUS_t storeAssets(uint8_t * assets, size_t len);
	STATUS_t forceUpdateFromBootloader();
	STATUS_t isButtonPressed();



};



#endif /* INC_ESPECIALIZACOES_BOOTLOADER_BOOT_H_ */

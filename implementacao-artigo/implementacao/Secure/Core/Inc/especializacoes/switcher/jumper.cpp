/*
 * jumper.cpp
 *
 *  Created on: 12 de mar de 2021
 *      Author: Paulo Sell
 */

#include <especializacoes/switcher/jumper.h>
#include "stm32l5xx_hal.h"
#include <especializacoes/flash/flash.h>
#include <especializacoes/storage/storage.h>

#include "main.h"
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

void Jumper::goToNonSecure() {


	Storage s;
	uint8_t update[8];
	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);

	uint32_t address;
	if (update[0]) {
		address = NEW_FW_START_ADDRESS;

	} else {
		address = FW_START_ADDRESS;
	}

	HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_All, GPIO_PIN_NSEC);
	funcptr_NS NonSecure_ResetHandler;

	SCB_NS->VTOR = address;

	/* Set non-secure main stack (MSP_NS) */
	__TZ_set_MSP_NS((*(uint32_t*) address));

	/* Get non-secure reset handler */
	NonSecure_ResetHandler = (funcptr_NS) (*((uint32_t*) ((address)
			+ 4U)));

	/* Start non-secure state software application */
	NonSecure_ResetHandler();

}

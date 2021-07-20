/*
 * bootloader.cpp
 *
 *  Created on: 17 de fev de 2021
 *      Author: Paulo Sell
 */

#include "bootloader.h"
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

/* USER MIGHT INITILIALIZE CLASS DUE TO PLATFORM NEEDS */
Bootloader::Bootloader() {
	this->state = STATE_t::VERIFICATION;
	this->is_ready_to_boot = STATUS_t::FAIL;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkFirmwareIntegrity(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkNewFirmwareSignature(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::checkNewFirmwareVersion(void) {
	return STATUS_t::SUCCESS;
}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::receiveNewFirmware(void) {
	return STATUS_t::SUCCESS;

}

/* MUST BE IMPLEMENTED BY USER*/
Bootloader::STATUS_t Bootloader::hasToUpdate(void) {
	return STATUS_t::SUCCESS;

}

/* MUST BE IMPLEMENTED BY USER*/
void Bootloader::launchFirmware(void) {

}

Bootloader::STATE_t Bootloader::getState(void) {
	return this->state;
}

Bootloader::STATUS_t Bootloader::setState(Bootloader::STATE_t state) {
	this->state = state;
	return STATUS_t::SUCCESS;
}

Bootloader::STATUS_t Bootloader::getIsReadyToBoot(void) {
	return this->is_ready_to_boot;
}
Bootloader::STATUS_t Bootloader::setIsReadyToBoot(STATUS_t status) {
	this->is_ready_to_boot = status;
	return STATUS_t::SUCCESS;
}


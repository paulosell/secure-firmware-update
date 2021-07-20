/*
 * boot.cpp
 *
 *  Created on: 2 de mar de 2021
 *      Author: Paulo Sell
 */

#include <especializacoes/bootloader/boot.h>
#include <especializacoes/cryptography/crypto-rsa.h>
#include <especializacoes/storage/storage.h>
#include <especializacoes/storage/storage.h>
#include <especializacoes/switcher/jumper.h>
#include "stm32l562e_discovery.h"

Boot::Boot() {
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
}



Boot::STATUS_t Boot::forceUpdateFromBootloader() {


	Flash f;
	Storage s;
	DATA_t word;
	DATA_t address;

	uint8_t current_value[8]; // obtem valor do bank atual (esta no indice 0)
	s.readData(current_value, Storage::ASSET_t::UPDATE_REQUEST);
	current_value[7] = 0x01;

	address.word.word_32 = UPDATE_REQUEST_ADDRESS;

	word.word.word_64 = ((uint64_t) current_value[7] << 56) | ((uint64_t) current_value[6] << 48) | ((uint64_t) current_value[5] << 40) |
						((uint64_t) current_value[4] << 32) | ((uint64_t) current_value[3] << 24) | ((uint64_t) current_value[2] << 16) |
						((uint64_t) current_value[1] << 8) | ((uint64_t) current_value[0] << 0);

	s.eraseData(address);
	s.writeData(address, word);

	return Boot::STATUS_t::SUCCESS;

}
;

Boot::STATUS_t Boot::isButtonPressed() {
	if (BSP_PB_GetState(BUTTON_USER) == SET) {
		return Boot::STATUS_t::SUCCESS;
	}

	return Boot::STATUS_t::FAIL;
}

Boot::STATUS_t Boot::hasToUpdate(void) {

	Storage s;
	uint8_t update[8];
	Boot::STATUS_t result = Boot::STATUS_t::FAIL;
	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);

	if (update[7]) {

		result = Boot::STATUS_t::SUCCESS;
		this->setState(Boot::STATE_t::UPDATE_PREPARATION);
	}

	return result;
}

Boot::STATUS_t Boot::storeAssets(uint8_t *assets, size_t len) {

	uint8_t sig[256];
	uint8_t size[8];
	uint8_t version[8];

	if (len != 272) {
		return Boot::STATUS_t::FAIL;
	}

	int i;

	for (i = 0; i < 256; i++) {
		sig[i] = assets[i];
	}

	for (i = 256; i < 264; i++) {
		size[i - 256] = assets[i];
	}

	for (i = 264; i < 272; i++) {
		version[i - 264] = assets[i];
	}

	Storage s;
	DATA_t address;
	uint8_t current_value[8];
	s.readData(current_value, Storage::ASSET_t::UPDATE_REQUEST);


	if(current_value[0]){
		// se banco atual é 0x0806 escreve nos endereco do banco 0x0804
		address.word.word_32 = FIRMWARE_SIGNATURE_ADDRESS;

			for (int i = 0; i < 256; i += 8) {
				DATA_t word;
				word.word.word_64 = ((uint64_t) sig[i + 7] << 56)
								| ((uint64_t) sig[i + 6] << 48) | ((uint64_t) sig[i + 5] << 40)
								| ((uint64_t) sig[i + 4] << 32) | ((uint64_t) sig[i + 3] << 24)
								| ((uint64_t) sig[i + 2] << 16) | ((uint64_t) sig[i + 1] << 8)
								| ((uint64_t) sig[i] << 0);

				if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
					return Boot::STATUS_t::FAIL;
				}

				address.word.word_32 = address.word.word_32 + 8;
			}

			address.word.word_32 = FIRMWARE_SIZE_ADDRESS;

			DATA_t word;
			word.word.word_64 = ((uint64_t) size[7] << 56) | ((uint64_t) size[6] << 48)
							| ((uint64_t) size[5] << 40) | ((uint64_t) size[4] << 32);

			if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}

			address.word.word_32 = FIRMWARE_VERSION_ADDRESS;

			word.word.word_64 = ((uint64_t) version[7] << 56);

			if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}





	} else {
		// se banco atual é 0x0804 escreve nos endereco do banco 0x0806
		address.word.word_32 = NEW_FIRMWARE_SIGNATURE_ADDRESS;

				for (int i = 0; i < 256; i += 8) {
					DATA_t word;
					word.word.word_64 = ((uint64_t) sig[i + 7] << 56)
									| ((uint64_t) sig[i + 6] << 48) | ((uint64_t) sig[i + 5] << 40)
									| ((uint64_t) sig[i + 4] << 32) | ((uint64_t) sig[i + 3] << 24)
									| ((uint64_t) sig[i + 2] << 16) | ((uint64_t) sig[i + 1] << 8)
									| ((uint64_t) sig[i] << 0);

					if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
						return Boot::STATUS_t::FAIL;
					}

					address.word.word_32 = address.word.word_32 + 8;
				}

				address.word.word_32 = NEW_FIRMWARE_SIZE_ADDRESS;

				DATA_t word;
				word.word.word_64 = ((uint64_t) size[7] << 56) | ((uint64_t) size[6] << 48)
								| ((uint64_t) size[5] << 40) | ((uint64_t) size[4] << 32);

				if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
					return Boot::STATUS_t::FAIL;
				}

				address.word.word_32 = NEW_FIRMWARE_VERSION_ADDRESS;

				word.word.word_64 = ((uint64_t) version[7] << 56);

				if (s.writeData(address, word) != Storage::STATUS_t::SUCCESS) {
					return Boot::STATUS_t::FAIL;
				}




	}

	return Boot::STATUS_t::SUCCESS;
}

Boot::STATUS_t Boot::receiveNewFirmware(void) {

	DATA_t address;
	DATA_t word;
	Flash f;
	Storage s;

	this->setState(Boot::STATE_t::UPDATE_PREPARATION);
	address.word.word_32 = UPDATE_REQUEST_ADDRESS;
	uint8_t current_value[8]; // obtem valor do bank atual (esta no indice 0)
	s.readData(current_value, Storage::ASSET_t::UPDATE_REQUEST);
	current_value[7] = 0x00;


	address.word.word_32 = UPDATE_REQUEST_ADDRESS;

	word.word.word_64 = ((uint64_t) current_value[7] << 56) | ((uint64_t) current_value[6] << 48) | ((uint64_t) current_value[5] << 40) |
						((uint64_t) current_value[4] << 32) | ((uint64_t) current_value[3] << 24) | ((uint64_t) current_value[2] << 16) |
						((uint64_t) current_value[1] << 8) | ((uint64_t) current_value[0] << 0);

	s.eraseData(address);
	s.writeData(address, word);

	USBCDC usb;
	BSP_LED_Init(LED9);
	BSP_LED_On(LED9);


	this->setState(Boot::STATE_t::UPDATE_RECEPTION);

	int reception_finished = 0;
	while (!reception_finished) {

		if (usb.hasData()) {
			uint8_t recv[2048] = { 0 };
			size_t len = 0;
			uint8_t reply = Boot::COMMAND_t::ACK;
			usb.getData(recv, &len);

			switch (recv[0]) {

			case Boot::COMMAND_t::STATE: {
				reply = (uint8_t) this->getState();
				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_START: {

				uint8_t update[8];
				s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);


				if (update[0]) {
					// se banco atual é 0x0806 apaga banco 0x0804
					address.word.word_32 = FIRMWARE_HASH_ADDRESS;
					s.eraseData(address);
					address.word.word_32 = FW_START_ADDRESS;
					f.flashErase(address, 63);


				} else {
					// se banco atual é 0x0804 apaga banco 0x0806
					address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;
					s.eraseData(address);
					address.word.word_32 = NEW_FW_START_ADDRESS;
					f.flashErase(address, 63);
				}


				uint8_t assets[len];
				memcpy((void*) assets, (const void*) recv + 5, len);
				if (this->storeAssets(assets, len) != Boot::STATUS_t::SUCCESS)
					reply = Boot::COMMAND_t::ERROR;
				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_WRITE: {
				uint8_t fw[len];
				memcpy((void*) fw, (const void*) recv + 5, len);

				for (int i = 0; i < len; i += 8) {
					DATA_t word;

					word.word.word_64 = ((uint64_t) fw[i + 7] << 56)
									| ((uint64_t) fw[i + 6] << 48)
									| ((uint64_t) fw[i + 5] << 40)
									| ((uint64_t) fw[i + 4] << 32)
									| ((uint64_t) fw[i + 3] << 24)
									| ((uint64_t) fw[i + 2] << 16)
									| ((uint64_t) fw[i + 1] << 8)
									| ((uint64_t) fw[i] << 0);

					if (f.flashWrite(address, word)
							!= Flash::STATUS_t::SUCCESS) {
						reply = Boot::COMMAND_t::ERROR;
						break;
					}

					address.word.word_32 = address.word.word_32 + 8;

				}

				break;
			}
			case Boot::COMMAND_t::BOOTLOADER_DONE: {
				reception_finished = 1;
				break;
			}
			}
			uint8_t res[1] = { reply };
			usb.sendData(res, 1);
		}

	}

	this->setState(Boot::STATE_t::UPDATE_VALIDATION);
	if (this->validateNewFirmware() == Boot::STATUS_t::FAIL) {
		while(true){
		HAL_Delay(1000);
		BSP_LED_Toggle(LED9);
		}
	}

	this->setState(Boot::STATE_t::UPDATE_INSTALLATION);
	if (this->finishUpdate() == Boot::STATUS_t::FAIL) {
		while(true){
			HAL_Delay(500);
			BSP_LED_Toggle(LED9);
		}

	}

	BSP_LED_Off(LED9);
	BSP_LED_DeInit(LED9);
	return Boot::STATUS_t::SUCCESS;

}

Boot::STATUS_t Boot::validateNewFirmware(void) {

	if (this->checkNewFirmwareVersion() != Boot::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}
	if (this->checkNewFirmwareSignature() != Boot::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	return Boot::STATUS_t::SUCCESS;

}

Boot::STATUS_t Boot::finishUpdate(void) {

	Storage s;
	Flash f;
	DATA_t address;
	uint8_t new_fw_size[8];

	CryptoRSA c;
	size_t digest_len;


	uint8_t hash[32];
	uint8_t current_value[8];
	s.readData(current_value, Storage::ASSET_t::UPDATE_REQUEST);

	if (current_value[0]) {


		s.readData(new_fw_size, Storage::ASSET_t::FIRMWARE_SIZE);
		uint32_t fw_len = (new_fw_size[4] << 24) | (new_fw_size[5] << 16)
							| (new_fw_size[6] << 8) | new_fw_size[7];
		uint8_t fw[fw_len];

		address.word.word_32 = FW_START_ADDRESS;
		f.flashReadAll(address, fw, fw_len);

		CryptoRSA::STATUS_t status = CryptoRSA::STATUS_t::FAIL;
				status = c.shaGen(fw, sizeof(fw), hash,
						&digest_len, CryptoRSA::SHA_t::SHA256);



		current_value[0] = 0x0;
		address.word.word_32 = UPDATE_REQUEST_ADDRESS;
		DATA_t word;
		word.word.word_64 = ((uint64_t) current_value[7] << 56) | ((uint64_t) current_value[6] << 48) | ((uint64_t) current_value[5] << 40) |
							((uint64_t) current_value[4] << 32) | ((uint64_t) current_value[3] << 24) | ((uint64_t) current_value[2] << 16) |
							((uint64_t) current_value[1] << 8) | ((uint64_t) current_value[0] << 0);

		s.eraseData(address);
		s.writeData(address, word);

		address.word.word_32 = FIRMWARE_HASH_ADDRESS;

	} else {
		s.readData(new_fw_size, Storage::ASSET_t::NEW_FIRMWARE_SIZE);

		uint32_t fw_len = (new_fw_size[4] << 24) | (new_fw_size[5] << 16)
								| (new_fw_size[6] << 8) | new_fw_size[7];
		uint8_t fw[fw_len];

		address.word.word_32 = NEW_FW_START_ADDRESS;
		f.flashReadAll(address, fw, fw_len);

		CryptoRSA::STATUS_t status = CryptoRSA::STATUS_t::FAIL;
				status = c.shaGen(fw, sizeof(fw), hash,
						&digest_len, CryptoRSA::SHA_t::SHA256);

		s.readData(current_value, Storage::ASSET_t::UPDATE_REQUEST);
		current_value[0] = 0x1;
		address.word.word_32 = UPDATE_REQUEST_ADDRESS;
		DATA_t word;
		word.word.word_64 = ((uint64_t) current_value[7] << 56) | ((uint64_t) current_value[6] << 48) | ((uint64_t) current_value[5] << 40) |
							((uint64_t) current_value[4] << 32) | ((uint64_t) current_value[3] << 24) | ((uint64_t) current_value[2] << 16) |
							((uint64_t) current_value[1] << 8) | ((uint64_t) current_value[0] << 0);

		s.eraseData(address);
		s.writeData(address, word);

		address.word.word_32 = NEW_FIRMWARE_HASH_ADDRESS;


	}


	for (int i = 0; i < 32; i += 8) {
		DATA_t word;
		word.word.word_64 = ((uint64_t) hash[i + 7] << 56)
						| ((uint64_t) hash[i + 6] << 48)
						| ((uint64_t) hash[i + 5] << 40)
						| ((uint64_t) hash[i + 4] << 32)
						| ((uint64_t) hash[i + 3] << 24)
						| ((uint64_t) hash[i + 2] << 16) | ((uint64_t) hash[i + 1] << 8)
						| ((uint64_t) hash[i] << 0);

		if (f.flashWrite(address, word) != Flash::STATUS_t::SUCCESS) {
			return Boot::STATUS_t::FAIL;
		}

		address.word.word_32 = address.word.word_32 + 8;

	}

	return Boot::STATUS_t::SUCCESS;
}

Boot::STATUS_t Boot::checkNewFirmwareVersion(void) {
	uint8_t fw_version[8];
	uint8_t new_fw_version[8];

	Storage s;

	uint8_t update[8];
	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);

	if (update[0]) {
		// se banco atual é 0x0806, fw_version = NEW_FIRMWARE_VERSION
		if (s.readData(fw_version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
					!= Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}

			if (s.readData(new_fw_version, Storage::ASSET_t::FIRMWARE_VERSION)
					!= Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}

			if (fw_version[7] < new_fw_version[7]) {
				return Boot::STATUS_t::SUCCESS;
			}


	} else {

		// se banco atual é 0x0804, fw_version = FIRMWARE_VERSION
		if (s.readData(fw_version, Storage::ASSET_t::FIRMWARE_VERSION)
					!= Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}

			if (s.readData(new_fw_version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
					!= Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;
			}

			if (fw_version[7] < new_fw_version[7]) {
				return Boot::STATUS_t::SUCCESS;
			}
	}



	return Boot::STATUS_t::FAIL;

}

Boot::STATUS_t Boot::checkFirmwareIntegrity(void) {

	CryptoRSA c;
	Flash f;

	uint8_t fw_size[8];
	Storage s;
	if (s.readData(fw_size, Storage::ASSET_t::FIRMWARE_SIZE)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;
	}

	uint32_t fw_len = (fw_size[4] << 24) + (fw_size[5] << 16)
					+ (fw_size[6] << 8) + fw_size[7];

	uint8_t buffer_firmware[fw_len];

	DATA_t address;
	uint8_t update[8];


	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);


	if (update[0]){
		address.word.word_32 = NEW_FW_START_ADDRESS;

	} else {
		address.word.word_32 = FW_START_ADDRESS;

	}
	f.flashReadAll(address, buffer_firmware, fw_len);

	uint8_t digest[32];
	size_t digest_len;

	CryptoRSA::STATUS_t status = CryptoRSA::STATUS_t::FAIL;
	status = c.shaGen(buffer_firmware, sizeof(buffer_firmware), digest,
			&digest_len, CryptoRSA::SHA_t::SHA256);

	if (status == CryptoRSA::STATUS_t::SUCCESS) {
		uint8_t hash_firmware[32];

		Storage s;
		if (update[0]){
			s.readData(hash_firmware, Storage::ASSET_t::NEW_FIRMWARE_HASH);

		} else {

			s.readData(hash_firmware, Storage::ASSET_t::FIRMWARE_HASH);
		}
		status = c.shaCheck(hash_firmware, digest, CryptoRSA::SHA_t::SHA256);
		if (status == CryptoRSA::STATUS_t::SUCCESS) {
			this->setIsReadyToBoot(Boot::STATUS_t::SUCCESS);
			return Boot::STATUS_t::SUCCESS;

		} else {
			return Boot::STATUS_t::FAIL;
		}

	} else {
		return Boot::STATUS_t::FAIL;
	}

	/* should never get here*/
	return Boot::STATUS_t::FAIL;

}

Boot::STATUS_t Boot::checkNewFirmwareSignature(void) {

	Boot::STATUS_t result = Boot::STATUS_t::FAIL;

	CryptoRSA c;
	Flash f;
	Storage s;
	uint8_t fw_size[8];
	uint8_t update[8];

	s.readData(update, Storage::ASSET_t::UPDATE_REQUEST);

	if(update[0]){

	if (s.readData(fw_size, Storage::ASSET_t::FIRMWARE_SIZE)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	uint32_t fw_len = (fw_size[4] << 24) + (fw_size[5] << 16)
					+ (fw_size[6] << 8) + fw_size[7];
	uint8_t buffer_firmware[fw_len + 8];
	memset((void*) buffer_firmware, '\0', fw_len + 8);
	uint8_t modulo[256];
	uint8_t expoente[3];

	if (s.readData(expoente, Storage::ASSET_t::PUBLIC_EXPONENT)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	if (s.readData(modulo, Storage::ASSET_t::MODULUS)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	CryptoRSA::key_t chave_publica;
	chave_publica.rsa_key.exponent = (uint8_t*) expoente;
	chave_publica.rsa_key.modulus = (uint8_t*) modulo;
	chave_publica.rsa_key.exponent_len = 3;
	chave_publica.rsa_key.modulus_len = 256;
	uint8_t assinatura[256];

	if (s.readData(assinatura, Storage::ASSET_t::FIRMWARE_SIGNATURE)
			!= Storage::STATUS_t::SUCCESS) {
		return result;
	}

	DATA_t address;
	address.word.word_32 = FW_START_ADDRESS;
	f.flashReadAll(address, buffer_firmware, fw_len);

	uint8_t new_fw_version[8];

	if (s.readData(new_fw_version, Storage::ASSET_t::FIRMWARE_VERSION)
			!= Storage::STATUS_t::SUCCESS) {
		return Boot::STATUS_t::FAIL;

	}

	for (int i = fw_len; i < fw_len + 8; i++) {
		buffer_firmware[i] = new_fw_version[i - fw_len];
	}

	if (c.sigCheck(assinatura, buffer_firmware, fw_len + 8, &chave_publica,
			CryptoRSA::ALGORITHM_t::RSA_PKCS1_V1_5)
			== CryptoRSA::STATUS_t::SUCCESS) {
		result = Boot::STATUS_t::SUCCESS;
		return result;
	}
	} else {
		if (s.readData(fw_size, Storage::ASSET_t::NEW_FIRMWARE_SIZE)
					!= Storage::STATUS_t::SUCCESS) {
				return result;
			}

			uint32_t fw_len = (fw_size[4] << 24) + (fw_size[5] << 16)
							+ (fw_size[6] << 8) + fw_size[7];
			uint8_t buffer_firmware[fw_len + 8];
			memset((void*) buffer_firmware, '\0', fw_len + 8);
			uint8_t modulo[256];
			uint8_t expoente[3];

			if (s.readData(expoente, Storage::ASSET_t::PUBLIC_EXPONENT)
					!= Storage::STATUS_t::SUCCESS) {
				return result;
			}

			if (s.readData(modulo, Storage::ASSET_t::MODULUS)
					!= Storage::STATUS_t::SUCCESS) {
				return result;
			}

			CryptoRSA::key_t chave_publica;
			chave_publica.rsa_key.exponent = (uint8_t*) expoente;
			chave_publica.rsa_key.modulus = (uint8_t*) modulo;
			chave_publica.rsa_key.exponent_len = 3;
			chave_publica.rsa_key.modulus_len = 256;
			uint8_t assinatura[256];

			if (s.readData(assinatura, Storage::ASSET_t::NEW_FIRMWARE_SIGNATURE)
					!= Storage::STATUS_t::SUCCESS) {
				return result;
			}

			DATA_t address;
			address.word.word_32 = NEW_FW_START_ADDRESS;
			f.flashReadAll(address, buffer_firmware, fw_len);

			uint8_t new_fw_version[8];

			if (s.readData(new_fw_version, Storage::ASSET_t::NEW_FIRMWARE_VERSION)
					!= Storage::STATUS_t::SUCCESS) {
				return Boot::STATUS_t::FAIL;

			}

			for (int i = fw_len; i < fw_len + 8; i++) {
				buffer_firmware[i] = new_fw_version[i - fw_len];
			}

			if (c.sigCheck(assinatura, buffer_firmware, fw_len + 8, &chave_publica,
					CryptoRSA::ALGORITHM_t::RSA_PKCS1_V1_5)
					== CryptoRSA::STATUS_t::SUCCESS) {
				result = Boot::STATUS_t::SUCCESS;
				return result;
			}
	}
	return Boot::STATUS_t::FAIL;
}

void Boot::launchFirmware(void) {
	Jumper j;
	j.goToNonSecure();
}


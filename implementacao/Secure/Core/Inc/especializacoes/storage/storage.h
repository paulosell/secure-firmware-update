/*
 * secure.h
 *
 *  Created on: 2 de mar de 2021
 *      Author: Paulo Sell
 */

#ifndef INC_ESPECIALIZACOES_STORAGE_STORAGE_H_
#define INC_ESPECIALIZACOES_STORAGE_STORAGE_H_

#include "interfaces/securestorage/secstorage.h"

class Storage: public SecureStorage {

public:

	Storage(){};

	STATUS_t readData(uint8_t *out, ASSET_t asset);
	STATUS_t writeData(DATA_t address, DATA_t data);
	STATUS_t eraseData(DATA_t address);

};

#endif /* INC_ESPECIALIZACOES_STORAGE_STORAGE_H_ */

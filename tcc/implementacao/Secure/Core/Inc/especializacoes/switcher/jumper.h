/*
 * jumper.h
 *
 *  Created on: 12 de mar de 2021
 *      Author: Paulo Sell
 */

#ifndef INC_ESPECIALIZACOES_SWITCHER_JUMPER_H_
#define INC_ESPECIALIZACOES_SWITCHER_JUMPER_H_

#include <iostream>
#include "interfaces/switcher/switcher.h"
#include "interfaces/flashmanager/flashman.h"

class Jumper: public Switcher {

public:

	Jumper(){};
	void goToNonSecure(void);

};

#endif /* INC_ESPECIALIZACOES_SWITCHER_JUMPER_H_ */

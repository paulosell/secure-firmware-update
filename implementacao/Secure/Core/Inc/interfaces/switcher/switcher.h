/*
 * switcher.h
 *
 *  Created on: 17 de fev de 2021
 *      Author: Paulo Sell
 */

#ifndef INTERFACES_SWITCHER_SWITCHER_H_
#define INTERFACES_SWITCHER_SWITCHER_H_

#include <iostream>
/* USER PRIVATE INCLUDE BEGINS HERE*/

/* USER PRIVATE INCLUDE ENDS HERE*/

class Switcher {
public:

	Switcher();
	virtual void goToSecure(void) = 0;
	virtual void goToNonSecure(void) = 0;
};

#endif /* INTERFACES_SWITCHER_SWITCHER_H_ */


#ifndef GPIO_H
#define	GPIO_H
#include "xc.h"




 
//******* Define two bits as output bits for the H-Bridge
#define HBRIDGE1 LATEbits.LATE0 

#define HBRIDGE2 LATEbits.LATE2


void initIO();
#endif	/* GPIO_H */


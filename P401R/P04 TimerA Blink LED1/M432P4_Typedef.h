/*
库函数的说明在该库函数之上！！！！！

uint_8->uchar
uint_16->short
uint_32->uint
*/

#ifndef M432P4_TYPEDEF_H_
#define M432P4_TYPEDEF_H_

/*Type*/
typedef long int lint;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long int ulint;

/*GPIO_PORT,数字型PORT直接写数字*/
#define PAA GPIO_PORT_PA
#define PBB GPIO_PORT_PB
#define PCC GPIO_PORT_PC
#define PDD GPIO_PORT_PD
#define PEE GPIO_PORT_PE
#define PJJ GPIO_PORT_PJ

/*GPIO_PIN*/
#define BIT8_ALL PIN_ALL8 
#define BIT16_ALL PIN_ALL16

#endif /* M432P4_TYPEDEF_H_ */

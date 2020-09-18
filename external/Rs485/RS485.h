/*
 * RS485.h
 *
 *  Created on: Nov 18, 2018
 *      Author: win7pro
 */

#ifndef RS485_H_
#define RS485_H_

#include "main.h"
//#include "debug.h"

#define RS485_GPIO_Port GPIOA
#define RS485_TX      	GPIO_PIN_2
#define RS485_RX      	GPIO_PIN_3

#define RS485_DE      	GPIO_PIN_4




void DE_Low(void);
void DE_High(void);


void MX_RS485_UART_Init(uint32_t IntEnable);
void RS485_SendBuffer(uint8_t *Buff, uint8_t LenBuff);



#endif /* RS485_H_ */

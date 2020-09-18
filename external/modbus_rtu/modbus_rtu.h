/*
 * modbus_rtu.h
 *
 *  Created on: Sep 17, 2020
 *      Author: ybkah
 */

#ifndef MODBUS_RTU_H_
#define MODBUS_RTU_H_

#include "main.h"
#include "ringbuffer.h"


#define  MODBUS_FUNC_READ_COILS       (0X01)
#define  MODBUS_FUNC_READ_INPUTBITS   (0X02)
#define  MODBUS_FUNC_READ_HOLDINGREGS (0X03)
#define  MODBUS_FUNC_READ_INPUTREGS   (0X04)
#define  MODBUS_FUNC_WRITE_ONECOIL    (0X05)
#define  MODBUS_FUNC_WRITE_ONEREG     (0X06)
#define  MODBUS_FUNC_WRITE_COILS      (0X0F)
#define  MODBUS_FUNC_WRITE_REGS       (0X10)
#define  MODBUS_FUNC_READ_FILERECORD  (0X14)
#define  MODBUS_FUNC_WRITE_FILERECORD (0X15)
#define  MODBUS_FUNC_MASK_WRITEREG    (0X16)
#define  MODBUS_FUNC_READ_WRITEREGS   (0X17)
#define  MODBUS_FUNC_READ_FIFOQUEUE   (0X18)



#endif /* MODBUS_RTU_H_ */

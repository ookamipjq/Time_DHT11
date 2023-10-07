#ifndef __I2C_H
#define __I2C_H	 
#include "sys.h"

#define I2CPORT		GPIOB	//定义IO接口
#define I2C_SCL		GPIO_Pin_10	//定义IO接口
#define I2C_SDA		GPIO_Pin_11	//定义IO接口

#define HostAddress	0xC0	//总线主机的器件地址
#define BusSpeed	200000	//总线速度（不高于400000）


void I2C_Configuration2(void);
void I2C_SAND_BUFFER(u8 SlaveAddr, u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite);
void I2C_SAND_BYTE(u8 SlaveAddr,u8 writeAddr,u8 pBuffer);
void I2C_READ_BUFFER(u8 SlaveAddr,u8 readAddr,u8* pBuffer,u16 NumByteToRead);
u8 I2C_READ_BYTE(u8 SlaveAddr,u8 readAddr);
		 				    
#endif

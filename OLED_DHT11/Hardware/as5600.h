#ifndef __AS5600_H
#define __AS5600_H

#include "sys.h"

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<28;} //SDA切换为输入模式 
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<28;} //SDA切换为输出模式

#define IIC_SCL PBout(8)
#define IIC_SDA PBout(9)
#define READ_SDA PBin(9)

#define	_raw_ang_hi 0x0c
#define	_raw_ang_lo 0x0d

void I2C_Configuration3(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(u8 txd);
u8 I2C_ReadByte(u8 ack);
u8 I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NAck(void);

void IIC_Write_OneByte(u8 daddr,u8 addr,u8 date);
void IIC_Read_OneByte(u8 daddr,u8 addr);	
u8 AS5600Read_OneByte(u16 ReadAddr);
void AS500_WriteOneByte(u16 WriteAddr,u8 WriteData);
u8 AS500_ReadTwoByte(u16 ReadAddr_hi,u16 ReadAddr_lo);


#endif
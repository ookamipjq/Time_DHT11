#ifndef __DHT11_H
#define __DHT11_H	 
#include "sys.h"
#include "delay.h"

#define DHT11PORT	GPIOB	//定义IO接口
#define DHT11_IO	GPIO_Pin_12	//定义IO接口


//IO方向设置
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
//IO操作函数											   
#define	DHT11_DQ_OUT PAout(0) //数据端口	PA0 
#define	DHT11_DQ_IN  PAin(0)  //数据端口	PA0

typedef struct
{
	u8 temp;
	u8 humi;
}dht;

extern dht DHT;


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11 

//void DHT11_IO_OUT (void);
//void DHT11_IO_IN (void);
//void DHT11_RST (void);
//u8 Dht11_Check(void); 	   
//u8 Dht11_ReadBit(void); 
//u8 Dht11_ReadByte(void); 
//u8 DHT11_Init (void);
////u8 DHT11_ReadData(u8 *h);   u8 *temp,u8 *humi
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);
		 				    
#endif

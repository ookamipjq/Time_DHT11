#ifndef __DHT11_H
#define __DHT11_H	 
#include "sys.h"
#include "delay.h"

#define DHT11PORT	GPIOB	//����IO�ӿ�
#define DHT11_IO	GPIO_Pin_12	//����IO�ӿ�


//IO��������
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
//IO��������											   
#define	DHT11_DQ_OUT PAout(0) //���ݶ˿�	PA0 
#define	DHT11_DQ_IN  PAin(0)  //���ݶ˿�	PA0

typedef struct
{
	u8 temp;
	u8 humi;
}dht;

extern dht DHT;


u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11 

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

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "OLED_I2C.h"
#include "sys.h"

#define USART_REC_LEN 200

Time time;

uint8_t rx_buff[100];  //接收缓存  
uint8_t rx_done = 0; //接收完成标志  
uint8_t rx_cnt = 0;//接收数据长度  

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
u16 USART_RX_STA=0;       //接收状态标记	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

//void USART1_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		Serial_RxData = USART_ReceiveData(USART1);
//		Serial_RxFlag = 1;
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}



//void USART1_IRQHandler(void)                	//串口1中断服务程序
//{
//	u8 len,t;
//	int len1 = 0;
//	u8 Res;
//	char *asd;
//	
//	rec_cnt = 0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//	{
//		Res =USART_ReceiveData(USART1);	//读取接收到的数据
//		Rec_Dat[rec_cnt++] = USART_ReceiveData(USART1);
//		if((USART_RX_STA&0x8000)==0)//接收未完成
//		{
//				if(USART_RX_STA&0x4000)//接收到了0x0d
//				{
//						if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//						else USART_RX_STA|=0x8000;	//接收完成了 
//				}
//				
//				else //还没收到0X0D
//				{	
//					if(Res==0x0d)	USART_RX_STA|=0x4000;
//					else
//						{
//							USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//							USART_RX_STA++;
//							if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//						}		 
//				}
//			}   		 
//   } 
//	 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//	 for(t = 0; t < len;t++)
//	 {
//			Serial_Printf("%c",USART_RX_BUF[t]);
//			len1++;
//			OLED_ShowNum(100,0,len1,3,2);
//		  while((USART1->SR&0X40)==0);//等待发送结束
//	 }
////	 asd = "123";
////	 OLED_ShowNum(100,0,len1,3,2);
//	 OLED_ShowChar(50,0,USART_RX_BUF[0],2);
//	 USART_RX_STA=0;
//} 

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
		int hour; 
		int min;
		int sec;
		u8 len,t;
    uint8_t temp;  
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到一个字节，进入一次接收中断  
    {  
        rx_buff[rx_cnt++] = USART_ReceiveData(USART1); //将接收的数据存入rx_buff中  
        if(rx_cnt >= 8) rx_cnt = 0;                //每包数据不能超过接收buff的总长度  
  
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除接收中断标志  
    }  
  
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//接收完数据后进入空闲中断  
    {  
        //USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_IDLE);//这条语句是无效的  
				temp = USART1->SR; //先读SR,再读DR才能完成idle中断的清零，否则会一直进入中断。  
        temp = USART1->DR; 
  
        rx_done = 1; //检测到空闲状态，置位接收完成位  
    }  
	 temp = rx_cnt;
//	 hour = (rx_buff[0] - '0')*10 + (rx_buff[1] - '0');
//	 min = (rx_buff[3] - '0')*10 + (rx_buff[4] - '0');
//	 sec = (rx_buff[6] - '0')*10 + (rx_buff[7] - '0');
		time.Hour = (rx_buff[0] - '0')*10 + (rx_buff[1] - '0');
		time.Mine = (rx_buff[3] - '0')*10 + (rx_buff[4] - '0');
		time.Seco = (rx_buff[6] - '0')*10 + (rx_buff[7] - '0');
		
	 for(t = 0; t < temp;t++)
	 {
			Serial_Printf("%c",rx_buff[t]);
//			OLED_ShowNum(100,0,temp,3,2);
//		 OLED_ShowChar(50+t*8,0,rx_buff[t],2);
		  while((USART1->SR&0X40)==0);//等待发送结束
	 }
//	 asd = "123";
//	 OLED_ShowNum(50,0,time.Hour,2,2);
//	 OLED_ShowNum(66,0,time.Mine,2,2);
//	 OLED_ShowNum(82,0,time.Seco,2,2);
//	 rx_cnt = 0;
	 USART_RX_STA=0;
} 


#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "OLED_I2C.h"
#include "sys.h"

#define USART_REC_LEN 200

Time time;

uint8_t rx_buff[100];  //���ջ���  
uint8_t rx_done = 0; //������ɱ�־  
uint8_t rx_cnt = 0;//�������ݳ���  

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
u16 USART_RX_STA=0;       //����״̬���	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

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



//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//	u8 len,t;
//	int len1 = 0;
//	u8 Res;
//	char *asd;
//	
//	rec_cnt = 0;
//	
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
//		Rec_Dat[rec_cnt++] = USART_ReceiveData(USART1);
//		if((USART_RX_STA&0x8000)==0)//����δ���
//		{
//				if(USART_RX_STA&0x4000)//���յ���0x0d
//				{
//						if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//						else USART_RX_STA|=0x8000;	//��������� 
//				}
//				
//				else //��û�յ�0X0D
//				{	
//					if(Res==0x0d)	USART_RX_STA|=0x4000;
//					else
//						{
//							USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//							USART_RX_STA++;
//							if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//						}		 
//				}
//			}   		 
//   } 
//	 len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//	 for(t = 0; t < len;t++)
//	 {
//			Serial_Printf("%c",USART_RX_BUF[t]);
//			len1++;
//			OLED_ShowNum(100,0,len1,3,2);
//		  while((USART1->SR&0X40)==0);//�ȴ����ͽ���
//	 }
////	 asd = "123";
////	 OLED_ShowNum(100,0,len1,3,2);
//	 OLED_ShowChar(50,0,USART_RX_BUF[0],2);
//	 USART_RX_STA=0;
//} 

void USART1_IRQHandler(void)                	//����1�жϷ������
{
		int hour; 
		int min;
		int sec;
		u8 len,t;
    uint8_t temp;  
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�һ���ֽڣ�����һ�ν����ж�  
    {  
        rx_buff[rx_cnt++] = USART_ReceiveData(USART1); //�����յ����ݴ���rx_buff��  
        if(rx_cnt >= 8) rx_cnt = 0;                //ÿ�����ݲ��ܳ�������buff���ܳ���  
  
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//��������жϱ�־  
    }  
  
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//���������ݺ��������ж�  
    {  
        //USART_ClearITPendingBit(DEBUG_USARTx, USART_IT_IDLE);//�����������Ч��  
				temp = USART1->SR; //�ȶ�SR,�ٶ�DR�������idle�жϵ����㣬�����һֱ�����жϡ�  
        temp = USART1->DR; 
  
        rx_done = 1; //��⵽����״̬����λ�������λ  
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
		  while((USART1->SR&0X40)==0);//�ȴ����ͽ���
	 }
//	 asd = "123";
//	 OLED_ShowNum(50,0,time.Hour,2,2);
//	 OLED_ShowNum(66,0,time.Mine,2,2);
//	 OLED_ShowNum(82,0,time.Seco,2,2);
//	 rx_cnt = 0;
	 USART_RX_STA=0;
} 


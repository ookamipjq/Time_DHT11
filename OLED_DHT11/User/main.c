#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED_I2C.h"
#include "Serial.h"
#include "led.h"
#include "DHT11.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


char data_x = 25;


//Time time;

void OLED_ShowSet1(void)
{
	//温度
	OLED_ShowCN(data_x,3,0);
	OLED_ShowCN(data_x+16,3,2);
	OLED_ShowStr(data_x+16+16,3,":",2);
	OLED_ShowCN(data_x+16*3+14,3,3);
	//湿度
	OLED_ShowCN(data_x,6,1);
	OLED_ShowCN(data_x+16,6,2);
	OLED_ShowStr(data_x+16+16,6,":",2);
	OLED_ShowCN(data_x+16*3+14,6,4);
	//时间
	OLED_ShowCN(5,0,5);
	OLED_ShowCN(21,0,6);
	OLED_ShowStr(37,0,":",2);
	OLED_ShowStr(66,0,":",2);
	OLED_ShowStr(90,0,":",2);
}
int main(void)
{
	u8 temp = 0,humi = 0;
	Serial_Init();
	LED_Init();
	I2C_Configuration1();
	DHT11_Init();
	OLED_Init();
	OLED_Fill(0x00);
	OLED_ShowSet1();
	
	while (1)
	{
		OLED_ShowNum(50,0,time.Hour,2,2);
	  OLED_ShowNum(74,0,time.Mine,2,2);
	  OLED_ShowNum(98,0,time.Seco,2,2);
		time.Seco++;
		if(time.Seco >59)
		{
			time.Seco = 0;
			time.Mine++;
			if(time.Mine > 59)
			{
				time.Mine = 0;
				time.Hour++;
				if(time.Hour > 23) time.Hour = 0;
			}
		}
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	  Delay_ms(400);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(400);
		DHT11_Read_Data(&temp,&humi);
		OLED_ShowNum(data_x+16*2+10,3,temp,2,2);
		OLED_ShowNum(data_x+16*2+10,6,humi,2,2);
	}
}

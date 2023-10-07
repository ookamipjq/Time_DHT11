#include "encdor.h"
#include "stm32f10x.h"
void ENCODER_Init(void)
{
	GPIO_InitTypeDef EnCoder_GPIO;
	TIM_TimeBaseInitTypeDef EnCoder_TIM;
	TIM_ICInitTypeDef EnCoder_IO;
	NVIC_InitTypeDef EnCoder_NV;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	EnCoder_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	EnCoder_GPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&EnCoder_GPIO);
	
	EnCoder_TIM.TIM_ClockDivision = TIM_CKD_DIV1;
	EnCoder_TIM.TIM_CounterMode = TIM_CounterMode_Up;
	EnCoder_TIM.TIM_Period = 0;
	EnCoder_TIM.TIM_Prescaler = 65535;
	EnCoder_TIM.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&EnCoder_TIM);
	
	TIM_ICStructInit(&EnCoder_IO);
	EnCoder_IO.TIM_Channel = TIM_Channel_1;
	EnCoder_IO.TIM_ICFilter = 0xf;
	TIM_ICInit(TIM3,&EnCoder_IO);
	EnCoder_IO.TIM_Channel = TIM_Channel_2;
	EnCoder_IO.TIM_ICFilter = 0xf;
	TIM_ICInit(TIM3,&EnCoder_IO);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	EnCoder_NV.NVIC_IRQChannel = TIM3_IRQn;
	EnCoder_NV.NVIC_IRQChannelPreemptionPriority = 2;
	EnCoder_NV.NVIC_IRQChannelSubPriority = 2;
	EnCoder_NV.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&EnCoder_NV);
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

unsigned int Encoder_Get(void)
{
	unsigned int Temp;
//	Temp = TIM_GetCounter(TIM3);
//	TIM_SetCounter(TIM3,0);
	return Temp;
}

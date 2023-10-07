#include "stm32f10x.h"
#include "motor.h"
//PA8 PB12 PB13
void Motor_Init(void)
{
    GPIO_InitTypeDef Motor_GPIO;
    TIM_TimeBaseInitTypeDef Motor_TIM;
    TIM_OCInitTypeDef Motor_PWM;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    
    GPIO_StructInit(&Motor_GPIO); // 重新初始化Motor_GPIO结构体

    Motor_GPIO.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    Motor_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    Motor_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&Motor_GPIO);

    GPIO_StructInit(&Motor_GPIO); // 重新初始化Motor_GPIO结构体

    Motor_GPIO.GPIO_Pin = GPIO_Pin_9;
    Motor_GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
    Motor_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&Motor_GPIO);

    TIM_InternalClockConfig(TIM4);
    Motor_TIM.TIM_ClockDivision = TIM_CKD_DIV1;
    Motor_TIM.TIM_CounterMode = TIM_CounterMode_Up;
    Motor_TIM.TIM_Period = 1000;
    Motor_TIM.TIM_Prescaler = 0;
    Motor_TIM.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4,&Motor_TIM);

    TIM_OCStructInit(&Motor_PWM);
    Motor_PWM.TIM_OCMode = TIM_OCMode_PWM1;
    Motor_PWM.TIM_OCPolarity = TIM_OCPolarity_High;
    Motor_PWM.TIM_OutputState = TIM_OutputState_Enable;
    Motor_PWM.TIM_Pulse = 500;
    TIM_OC4Init(TIM4,&Motor_PWM);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4,ENABLE);
}


void Motor_Go(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
    TIM_SetCompare4(TIM4, 500);  // 修改为设置PWM占空比为50%
}
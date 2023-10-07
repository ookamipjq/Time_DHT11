#include "as5600.h"
#include "delay.h"

void I2C_Configuration3(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9);
}
void I2C_Start(void)
{
	SDA_OUT();
	IIC_SDA = 1;
	IIC_SCL = 1;
	Delay_us(10);
	IIC_SDA = 0;
	Delay_us(4);
	IIC_SCL = 0;
}
void I2C_Stop(void)
{
	SDA_OUT();
	IIC_SCL = 0;
	IIC_SDA = 0;
	Delay_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1;
	Delay_us(4);
}
void I2C_SendByte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0;
	for(t = 0;t<8;t++)
	{
		if((txd&0x80)>>7)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		txd<<=1;
		Delay_us(2);
		IIC_SCL = 1;
		Delay_us(2);
		IIC_SCL = 0;
		Delay_us(2);
	}
}
u8 I2C_ReadByte(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA¨¦¨¨???a¨º?¨¨?
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		Delay_us(1); 
    }					 
    if (!ack)
        I2C_NAck();//¡¤¡é?¨ªnACK
    else
        I2C_Ack(); //¡¤¡é?¨ªACK   
    return receive;
}
u8 I2C_WaitAck(void)
{
	u8 ucErrTime = 0;
	SDA_IN();
	IIC_SDA = 1; Delay_us(1);
	IIC_SCL = 1; Delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	IIC_SCL = 0;
	return 0;
}
void I2C_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;

}
void I2C_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;
}

void IIC_Write_OneByte(u8 daddr,u8 addr,u8 date);
void IIC_Read_OneByte(u8 daddr,u8 addr);	
u8 AS5600Read_OneByte(u16 ReadAddr)
{
	u8 temp=-1;		  	    																 
  I2C_Start();  
	I2C_SendByte((0X36<<1)|0x00);	   //¡¤¡é?¨ªD¡ä?¨¹¨¢?
	I2C_WaitAck(); 
  I2C_SendByte(ReadAddr);   //¡¤¡é?¨ª¦Ì??¡¤
	I2C_WaitAck();	    
	I2C_Start();  	 	   
	I2C_SendByte((0X36<<1)|0x01);           //??¨¨??¨®¨º??¡ê¨º?			   
	I2C_WaitAck();	 
  temp=I2C_ReadByte(0);		   
  I2C_Stop();//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t	    
	return temp;
}
void AS500_WriteOneByte(u16 WriteAddr,u8 WriteData)
{
	I2C_Start();  
	I2C_SendByte((0X36<<1)|0x00);	   //¡¤¡é?¨ªD¡ä?¨¹¨¢?
	I2C_WaitAck(); 
  I2C_SendByte(WriteAddr);   	//¡¤¡é?¨ª¦Ì??¡¤
	I2C_WaitAck();	    
	I2C_Start();  	 	   
	I2C_SendByte(WriteData);          //¡¤¡é?¨ª¨ºy?Y		   
	I2C_WaitAck();	 	   
  I2C_Stop();//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t	    
	Delay_ms(10);

}
u8 AS500_ReadTwoByte(u16 ReadAddr_hi,u16 ReadAddr_lo)
{
	u16 TwoByte_Data=-1;
	u8 hi_Data=0,lo_Data=0;
	hi_Data=AS5600Read_OneByte(ReadAddr_hi);
	lo_Data=AS5600Read_OneByte(ReadAddr_lo);
	TwoByte_Data = (hi_Data<<8)|lo_Data;
	return TwoByte_Data;

}
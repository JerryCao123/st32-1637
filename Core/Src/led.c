#include"led.h"

// 数码管段码表（共阳数码管）
const uint8_t SEG_CODE[] = 
{
    0x3F, // 0     0x3F,
    0x06, // 1 0xF9
    0x5B, // 2 0x5B,   B6
    0x4F, // 3 0x4F,    9F
    0x66, // 4 0x66,
    0x6D, // 5 0x6D, 
    0x7D, // 6 0x7D,
    0x07, // 7 0x07,
    0x7F, // 8 0x7F,   
    0x6F, // 9 0x6F,   
    0x80, // . 0xBF  
		0x00,	//＋
		0x40	//-
};

// 延时函数
void delay_us(uint32_t us) 
{
    uint32_t count = us * (SystemCoreClock / 1000000 / 4);
    while (count--);
}
// 发送一个字节到TM1637
void TM1637_SendByte(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t data) 
{
    for (int i = 0; i < 8; i++)
	{
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
        if (data & 0x01) 
					{
            HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
        } 
				else 
					{
            HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
        }
				delay_us(5);	
        data >>= 1;
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
        delay_us(5);
    }

}

// 启动信号
void TM1637_Start(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) 
{
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
    delay_us(2);
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
}

// 停止信号
void TM1637_Stop(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) 
{
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    delay_us(2);
    HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
}

void TM1637_Ack(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN)
{
	HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
	delay_us(5);
	while(HAL_GPIO_ReadPin(DIO_PORT, DIO_PIN));
	HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
	delay_us(2);
	HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
}	

// 初始化TM1637
void TM1637_Init(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN) 
{
    TM1637_Start(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
    TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, 0x40); // 数据命令设置，普通模式，自动地址增加
		TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    TM1637_Stop(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);

    TM1637_Start(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
    TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, 0x8A); // 显示控制命令设置，显示开，亮度最大
		TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    TM1637_Stop(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
	
}

// 显示数字（带小数点）
void TM1637_Display(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length) 
{
    TM1637_Start(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
	/***************
		for (uint8_t i = 0; i < length; i++)
	{
        uint8_t address = 0xC0 + i; // 地址0x00~0x05对应第1~6位（左到右）
        TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, address);
        TM1637_Ack(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
        TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, SEG_CODE[digits[i]]);
        TM1637_Ack(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
    }
	**********************/
    TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, 0xC0); // 地址命令设置，起始地址0x00
		TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    for (uint8_t i = 0; i < length; i++) 
		{
			if(i==1)
			{
				TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, (SEG_CODE[digits[i]]|0x80));  //SEG_CODE[digits[i]]
			}
			else
			{
				TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, SEG_CODE[digits[i]]);  //SEG_CODE[digits[i]]
			}
        
				TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    }
    TM1637_Stop(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
}
// 显示数字(不带小数点)
void TM1637_Display1(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN, uint8_t* digits, uint8_t length) 
{
    TM1637_Start(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
	
    TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, 0xC0); // 地址命令设置，起始地址0x00
		TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    for (uint8_t i = 0; i < length; i++) 
		{
		
				TM1637_SendByte(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN, SEG_CODE[digits[i]]);  //SEG_CODE[digits[i]]
				TM1637_Ack(CLK_PORT,CLK_PIN,DIO_PORT,DIO_PIN);
    }
    TM1637_Stop(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN);
}
//显示VSIN和VCOS
void display_float(GPIO_TypeDef* CLK_PORT, uint16_t CLK_PIN, GPIO_TypeDef* DIO_PORT, uint16_t DIO_PIN,float num)
{
		unsigned char display[6];
		display[5]=(uint32_t)(num*10000) % 10;							//个
		display[4]=(uint32_t)(num*10000)/10 % 10;						//十
		display[3]=(uint32_t)(num*10000)/100 % 10;					//百
		display[2]=(uint32_t)(num*10000)/1000 % 10;					//千
		display[1]=(uint32_t)(num*10000)/10000 % 10;				//万
		int index=(num<0)?1:0;		
		if(index)
		{
			display[0]=12;
		}
		else
		{
			display[0]=11;
		}
		TM1637_Display(CLK_PORT, CLK_PIN, DIO_PORT, DIO_PIN,display , 6);     //CLK， DIO	
}
//显示电气角度
void display_angle(float num)
{
	unsigned char dis[6];
	if((num<10)&&(num>0))
	{
		dis[5]=(uint32_t)(num*100) % 10;							
		dis[4]=(uint32_t)(num*100)/10 % 10;						
		dis[3]=(uint32_t)(num*100)/100 % 10;						
		dis[2]=11;					
		dis[1]=11;				
		dis[0]=11;				
	}
	else if((num<100)&&(num>=10))
	{
		dis[5]=(uint32_t)(num*100) % 10;							
		dis[4]=(uint32_t)(num*100)/10 % 10;						
		dis[3]=(uint32_t)(num*100)/100 % 10;						
		dis[2]=(uint32_t)(num*100)/1000 % 10;					
		dis[1]=11;				
		dis[0]=11;				
	}
		else if((num<1000)&&(num>=100))
	{
		dis[5]=(uint32_t)(num*100) % 10;							
		dis[4]=(uint32_t)(num*100)/10 % 10;						
		dis[3]=(uint32_t)(num*100)/100 % 10;						
		dis[2]=(uint32_t)(num*100)/1000 % 10;					
		dis[1]=(uint32_t)(num*100)/10000 % 10;				
		dis[0]=11;				
	}
			else if((num<9999)&&(num>=1000))
	{
		dis[5]=(uint32_t)(num*100) % 10;							
		dis[4]=(uint32_t)(num*100)/10 % 10;						
		dis[3]=(uint32_t)(num*100)/100 % 10;						
		dis[2]=(uint32_t)(num*100)/1000 % 10;					
		dis[1]=(uint32_t)(num*100)/10000 % 10;				
		dis[0]=(uint32_t)(num*100)/100000 % 10;			
	}
	  TM1637_Start(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1);

    TM1637_SendByte(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1 ,0xC0); // 地址命令设置，起始地址0x00
		TM1637_Ack(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1);
    for (uint8_t i = 0; i < 6; i++) 
		{
			if(i==3)
			{
				TM1637_SendByte(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1, (SEG_CODE[dis[i]]|0x80));  //SEG_CODE[digits[i]]
			}
			else
			{
				TM1637_SendByte(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1, SEG_CODE[dis[i]]);  //SEG_CODE[digits[i]]
			}
        
				TM1637_Ack(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1);
    }
    TM1637_Stop(GPIOB,GPIO_PIN_0, GPIOB, GPIO_PIN_1);
}	



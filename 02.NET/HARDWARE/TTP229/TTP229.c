#include "TTP229.h"
#include "delay.h"

void Touch_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		  // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50Mhz速度
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SDA_OUT();
	TTP_SDO = 0;
	TTP_SCL = 0;
	Touch_Read();
}

uint16_t Touch_Read2(void)
{
	uint8_t i = 0;
	uint16_t real_Value = 0;

	SDA_OUT();
	TTP_SDO = 1;
	delay_us(100);
	TTP_SDO = 0;
	delay_us(20);

	SDA_IN();

	for (i = 0; i < 16; i++)
	{
		TTP_SCL = 1;
		delay_us(200);
		TTP_SCL = 0;

		if (TTP_SDI == 1)
		{
			real_Value |= (1 << i);
		}

		delay_us(200);
	}

	delay_ms(2);

	return real_Value;
}

uint16_t Touch_Read(void)
{
	uint8_t i = 0;
	uint16_t real_Value = 0;

	SDA_OUT();
	TTP_SDO = 1;
	delay_us(100);
	TTP_SDO = 0;
	delay_us(20);

	SDA_IN();

	for (i = 0; i < 16; i++)
	{
		TTP_SCL = 1;
		delay_us(100);
		TTP_SCL = 0;
		delay_us(1);

		if (TTP_SDI == 1)
		{
			real_Value |= (1 << i);
		}
	}

	delay_ms(2);

	return real_Value;
}

uint16_t Previous = 0;
uint16_t Current = 0;

uint8_t Touch_KeyScan(void)
{
	uint8_t key = 0;

	Current = Touch_Read();

	if ((Current & 0x0001) && !(Previous & 0x0001))
	{
		key = 1;
	}
	else if ((Current & 0x0002) && !(Previous & 0x0002))
	{
		key = 2;
	}
	else if ((Current & 0x0004) && !(Previous & 0x0004))
	{
		key = 3;
	}
	else if ((Current & 0x0008) && !(Previous & 0x0008))
	{
		key = 0x0A;
	}
	else if ((Current & 0x0010) && !(Previous & 0x0010))
	{
		key = 4;
	}
	else if ((Current & 0x0020) && !(Previous & 0x0020))
	{
		key = 5;
	}
	else if ((Current & 0x0040) && !(Previous & 0x0040))
	{
		key = 6;
	}
	else if ((Current & 0x0080) && !(Previous & 0x0080))
	{
		key = 0x0B;
	}
	else if ((Current & 0x0100) && !(Previous & 0x0100))
	{
		key = 7;
	}
	else if ((Current & 0x0200) && !(Previous & 0x0200))
	{
		key = 8;
	}
	else if ((Current & 0x0400) && !(Previous & 0x0400))
	{
		key = 9;
	}
	else if ((Current & 0x0800) && !(Previous & 0x0800))
	{
		key = 0x0C;
	}
	else if ((Current & 0x1000) && !(Previous & 0x1000))
	{
		key = 13;
	}
	else if ((Current & 0x2000) && !(Previous & 0x2000))
	{
		key = 14;
	}
	else if ((Current & 0x4000) && !(Previous & 0x4000))
	{
		key = 15;
	}
	else if ((Current & 0x8000) && !(Previous & 0x8000))
	{
		key = 16;
	}

	Previous = Current;

	return key;
}

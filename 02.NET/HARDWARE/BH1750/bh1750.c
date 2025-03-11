#include "BH1750.h"
#include "sys.h"
#include "delay.h"

u8 BUF[8];
u16 temp2 = 0;
float lux = 0;
void bh1750_iic_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6); // PB6,PB7 输出高
}

/****向BH1750单次写命令****/
void Single_Write_bh1750(unsigned char Reg_Address)
{
	bh1750_iic_start();
	bh1750_send_byte(0x46); // 发送器件地址0100 0110   最后一位0，表示写
	// IIC_Wait_Ack();
	bh1750_send_byte(Reg_Address);
	// IIC_Wait_Ack();
	bh1750_iic_stop();
}

/****BH1750初始化****/
void bh1750_Init(void)
{

	//	TIM4_PWM_Init(899,0);
	bh1750_iic_init();
	TIM_SetCompare3(TIM3, 0);
}

// 光照强度数据接收和处理
u8 getValue(void)
{
	unsigned char value = 0;
	Single_Write_bh1750(0x01);
	Single_Write_bh1750(0x10);
	delay_ms(200);
	bh1750_ReadContinuous1();
	temp2 = BUF[0];
	temp2 = (temp2 << 8) + BUF[1];
	lux = (float)temp2 / 1.2; // lux是float型
	if ((lux > 0) && (lux < 50))
		value = 10;
	else if (lux < 75)
		value = 20;
	else if (lux < 100)
		value = 40;
	else if (lux < 125)
		value = 50;
	else if (lux < 150)
		value = 60;
	else if (lux < 175)
		value = 70;
	else if (lux < 200)
		value = 80;
	else if (lux < 250)
		value = 90;
	else if (lux < 280)
		value = 110;
	else if (lux < 300)
		value = 120;
	else if (lux < 320)
		value = 130;
	else if (lux < 340)
		value = 140;
	else if (lux < 360)
		value = 150;
	else if (lux < 380)
		value = 160;
	else if (lux < 400)
		value = 170;
	else if (lux < 410)
		value = 180;
	else if (lux < 450)
		value = 190;
	else if (lux < 800)
		value = 240;
	// printf("光照强度:%f   value:%d\r\n",lux,value);
	TIM_SetCompare3(TIM3, value);
	return value;

	// Delay_ms(1000);
}
/****从BH1750单次读Lux****/
/****
模式选择：H-Resolution Mode,分辨率1lux
单次读Opecode：0010 0000 ，即0x20
器件地址：0100011，ADDR接GND
****/
void bh1750_ReadOneTime(void)
{

	bh1750_iic_start();
	bh1750_send_byte(0x46); // 发送器件地址0100 0110   最后一位0，表示写
	bh1750_recv_ack();
	bh1750_send_byte(0x20); // 发送One time H-Resolution Mode的Opecode 0010 0000，分辨率为1lux
	bh1750_recv_ack();
	bh1750_iic_stop();
	delay_ms(200);
	bh1750_iic_start();
	bh1750_send_byte(0x47); //
	bh1750_recv_ack();
	BUF[0] = bh1750_recv_byte(1);
	// IIC_Ack();        //主机应答
	// temp1=temp<<8;
	BUF[1] = bh1750_recv_byte(0);
	// IIC_NAck();        //主机不应答
	bh1750_iic_stop();

	// 	temp2=BUF[0];
	// 	temp2=(temp2<<8)+BUF[1];
	// 	lux=(float)temp2/1.2;    //lux是float型

	// return lux;
	// Delay_ms(200);
}

/****从BH1750连续读Lux****/
/****
模式选择：H-Resolution Mode,分辨率1lux
连续读Opecode：0001 0000
器件地址：0100011，ADDR接GND
****/
void bh1750_ReadContinuous1(void)
{
	//	u16 temp=0,temp1=0;
	bh1750_iic_start();
	bh1750_send_byte(0x46); // 发送器件地址0100 0110   最后一位0，表示写
	bh1750_recv_ack();
	bh1750_send_byte(0x10); // 发送Continuous H-Resolution Mode的Opecode 0001 0000，分辨率为1lux
	bh1750_recv_ack();
	bh1750_iic_stop();
	delay_ms(200);
	bh1750_iic_start();
	bh1750_send_byte(0x47);
	bh1750_recv_ack();

	BUF[0] = bh1750_recv_byte(1);
	BUF[1] = bh1750_recv_byte(0);

	// temp=bh1750_recv_byte(1);
	// temp1=bh1750_recv_byte(0);

	bh1750_iic_stop();

	// temp2=temp1+(temp<<8);
	// lux=(float)temp2/1.2;    //lux是float型
}

void Conversion(void)
{
	temp2 = BUF[0];
	temp2 = (temp2 << 8) + BUF[1];
	lux = (float)temp2 / 1.2; // lux是float型
}

/**************************************
起始信号
**************************************/
void bh1750_iic_start(void)
{
	BH1750_SDA_OUT() // 设置SDA线为输出
	// 在开始数据传输前，先让SDA和SCL都拉高
	BH1750_IIC_SDA = 1; // 发送起始条件的数据信号，释放总线
	delay_us(2);
	BH1750_IIC_SCL = 1;

	delay_us(5); // Tsu;STA：起始条件的建立时间大于4.7us。

	BH1750_IIC_SDA = 0; // SDA由高变为低表示开始信号
	delay_us(4);		// 起始条件的保持时间大于4us
	BH1750_IIC_SCL = 0; // 钳住IIC总线，准备发送或者接收数据
	delay_us(2);
}

/**************************************
停止信号
**************************************/
void bh1750_iic_stop(void)
{
	BH1750_SDA_OUT() // 设置SDA线为输出

	// IIC_SCL=0;    //此句是调试BH1750时加，2018.03.03

	BH1750_IIC_SDA = 0; // 发送停止信号的数据信号
	delay_us(2);
	BH1750_IIC_SCL = 1; // 发送停止信号的时钟信号
	delay_us(5);		// 停止信号的建立时间大于4us
	BH1750_IIC_SDA = 1; // 发送停止信号
	delay_us(4);		//;
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void bh1750_send_ack(char ack)
{
	BH1750_IIC_SCL = 0;
	BH1750_SDA_OUT();
	BH1750_IIC_SDA = ack; // 主器件应答
	delay_us(2);
	BH1750_IIC_SCL = 1;
	delay_us(4);		// SCL高电平周期大于4us
	BH1750_IIC_SCL = 0; // 清时钟线，钳住IIC总线以便继续接收
	delay_us(2);
}

/**************************************
接收应答信号
**************************************/
char bh1750_recv_ack(void)
{
	u8 ucErrorTime = 0;
	BH1750_SDA_IN(); // SDA设置成输入
	BH1750_IIC_SDA = 1;
	delay_us(2);
	BH1750_IIC_SCL = 1;
	delay_us(2);
	while (BH1750_READ_SDA)
	{
		ucErrorTime++;
		if (ucErrorTime >= 250)
		{
			bh1750_iic_stop();
			return 1;
		}
	}
	BH1750_IIC_SCL = 0;
	return 0;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void bh1750_send_byte(char dat)
{
	u8 i;
	BH1750_SDA_OUT();

	for (i = 0; i < 8; i++)
	{
		if ((dat << i) & 0x80)
			BH1750_IIC_SDA = 1; // 判断发送位
		else
			BH1750_IIC_SDA = 0;

		delay_us(2);
		BH1750_IIC_SCL = 1; // 拉高SCL，通知被控器开始接收数据位
		delay_us(4);		// 保证SCL高电平周期大于4us
		BH1750_IIC_SCL = 0; // 拉低SCL，允许SDA传输下一位数据。因为只有在SCL=0时才允许SDA的状态发生变化
		delay_us(2);
	}
	// 以下四句是调试BH1750时添加的，2018.03.03
	// 特别注意;用方法三时，以下四句不可去掉，去掉后得到的数有问题
	// 用方法二时，以下四句需注释掉，否则也会有问题，至于原因暂时还不清楚。
	// 	 IIC_SDA=0;
	// 	 Delay_us(2);
	// 	 IIC_SCL=1;
	// 	 Delay_us(2);

	BH1750_IIC_SCL = 0; // 拉低SCL，为下次数据传输做好准备
	delay_us(2);
}

/**************************************
从IIC总线接收一个字节数据
用来接收从器件传来的数据，ack为1时，发送Ack(产生应答);ack为0时，发送NAck(不产生应答)
**************************************/
char bh1750_recv_byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	BH1750_SDA_IN();
	for (i = 0; i < 8; i++)
	{
		BH1750_IIC_SCL = 0; // 置SCL为低，准备接收数据位
		delay_us(5);		// 时钟低电平周期大于4.7us
		BH1750_IIC_SCL = 1; // 置SCL为高，使SDA上数据有效
		delay_us(2);
		receive = receive << 1;
		if (BH1750_READ_SDA)
			receive = receive + 1; // 读取SDA，把接收的数据位放入receve中，若此处写成IIC_SDA==1，会失败，思考原因？
		delay_us(2);
	}
	if (!ack)
		bh1750_send_ack(1); // 发送NAck
	else
		bh1750_send_ack(0); // 发送Ack
	// IIC_SCL=0;      //在此处有何用处？？？(猜测：为下次数据传输做好准备， //钳住IIC总线，准备发送或者接收数据)
	// Delay_us(2);
	return receive;
}

/*************************************END OF FILE******************************/

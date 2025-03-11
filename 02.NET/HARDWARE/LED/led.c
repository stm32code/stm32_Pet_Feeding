#include "delay.h"
#include "led.h"
// 协议文件
#include "onenet.h"
// 网络设备驱动
#include "esp8266.h"
extern Device_Satte_Typedef device_state_init; // 设备状态
// 初始化PB5和PE5为输出口.并使能这两个口的时钟
// LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Relay_GPIO_CLK | LED1_GPIO_CLK, ENABLE); // 使能端口时钟C

	GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;	  // 端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz
	//
	//	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	  // 端口配置, 推挽输出
	//	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz
	//
	//	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;	  // 端口配置, 推挽输出
	//	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz
	//
	//	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;	  // 端口配置, 推挽输出
	//	GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz
	//
	//	GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;	  // 端口配置, 推挽输出
	//	GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz

	//	LED = OFF;
	//	LED1_OFF;
	//	LED2_OFF;
	//	LED3_OFF;
	//	LED4_OFF;
}
/**
 * @brief  MQTT操作LED模块
 * @param  无
 * @retval 无
 */
void Mqtt_LED(U8 Cmd)
{

	switch (Cmd)
	{
	case 1:
		LED1_ON;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		//	device_state_init.led_state=1;
		break;
	case 2:
		LED1_ON;
		LED2_ON;
		;
		LED3_OFF;
		LED4_OFF;
		//	device_state_init.led_state=2;
		break;
	case 3:
		LED1_ON;
		LED2_ON;
		LED3_ON;
		LED4_OFF;
		// device_state_init.led_state=3;
		break;
	case 4:
		LED1_ON;
		LED2_ON;
		LED3_ON;
		LED4_ON;
		//	device_state_init.led_state=4;
		break;
	case 0:
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
		// device_state_init.led_state=0;
		break;
	}
	OneNet_SendMqtt(2); // 上传订阅的主题
}
/*********************************************************************************
 * @Function	:	系统设置PB3和PB4为普通IO口
 * @Input		:	deviceSta,设备状态
 * @Output		: 	None
 * @Return		: 	None
 * @Others		: 	JTAG调试方式会受影响
 * @Date			:  2020-04-22
 **********************************************************************************/
void System_PB34_setIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}



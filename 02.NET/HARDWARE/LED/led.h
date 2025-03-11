#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "git.h"
/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define Relay_GPIO_PORT GPIOC               /* GPIO端口 */
#define Relay_GPIO_CLK RCC_APB2Periph_GPIOC /* GPIO端口时钟 */
#define Relay_GPIO_PIN GPIO_Pin_13

#define LED1_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED1_GPIO_PIN GPIO_Pin_4

#define LED2_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED2_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED2_GPIO_PIN GPIO_Pin_7

#define LED3_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED3_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED3_GPIO_PIN GPIO_Pin_8

#define LED4_GPIO_PORT GPIOB               /* GPIO端口 */
#define LED4_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO端口时钟 */
#define LED4_GPIO_PIN GPIO_Pin_9

#define Read_State GPIO_ReadInputDataBit(Relay_GPIO_PORT, Relay_GPIO_PIN)

#define LED PCout(13) // LED接口
#define LED1 PBout(4) // LED接口
#define LED2 PBout(7) // LED接口
#define LED3 PBout(8) // LED接口
#define LED4 PBout(9) // LED接口

#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)

#define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_ON digitalHi(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF digitalLo(LED2_GPIO_PORT, LED2_GPIO_PIN)

#define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_ON digitalHi(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF digitalLo(LED3_GPIO_PORT, LED3_GPIO_PIN)

#define LED4_TOGGLE digitalToggle(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_ON digitalHi(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_OFF digitalLo(LED4_GPIO_PORT, LED4_GPIO_PIN)

#define OFF 0
#define ON 1

void LED_Init(void); // 初始化
/**
 * @brief  MQTT操作LED模块
 * @param  无
 * @retval 无
 */
void Mqtt_LED(U8 Cmd);

void System_PB34_setIO(void);
#endif

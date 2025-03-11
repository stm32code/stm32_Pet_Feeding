#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

//  引脚定义
#define KEY1_GPIO_CLK RCC_APB2Periph_GPIOB
#define KEY1_GPIO_PORT GPIOB
#define KEY1_GPIO_PIN GPIO_Pin_14

#define KEY2_GPIO_CLK RCC_APB2Periph_GPIOB
#define KEY2_GPIO_PORT GPIOB
#define KEY2_GPIO_PIN GPIO_Pin_15

#define KEY3_GPIO_CLK RCC_APB2Periph_GPIOB
#define KEY3_GPIO_PORT GPIOB
#define KEY3_GPIO_PIN GPIO_Pin_15

#define KEY1 GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN) // 读取按键0
#define KEY2 GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN) // 读取按键1
#define KEY3 GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN) // 读取按键2

#define KEY1_PRES 1 // KEY1按下
#define KEY2_PRES 2 // KEY2按下
#define KEY3_PRES 3 // KEY3按下

#define Key_Scan_Time 30 // 短按时长时间判断

/** 按键按下标置宏
 *  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
 *  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
 */
#define KEY_ON 0
#define KEY_OFF 1
/*********************************************************************************
 * @Function	:  初始化控制LED的IO
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  None
 * @Date			:  2022-07-23
 **********************************************************************************/
void Key_GPIO_Config(void);
/*********************************************************************************
 * @Function	:  检测是否有按键按下
 * @Input		:  GPIOx：x 可以是 A，B，C，D或者 E
 *						:  GPIO_Pin：待读取的端口位
 * @Output		:  None
 * @Return		:  KEY_OFF(没按下按键)、KEY_ON（按下按键）
 * @Others		:  None
 * @Date			:  2022-07-23
 **********************************************************************************/
uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/*********************************************************************************
 * @Function	:  按键处理函数
 * @Input		:  Gmode:0,不支持连续按;1,支持连续按;
 * @Output		:  None
 * @Return		:  0，没有任何按键按下
 *							 1，KEY1按下
 *						   2，KEY2按下
 *							 3，KEY3按下
 * @Others		:  注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
 * @Date			:  2022-07-23
 **********************************************************************************/
u8 KEY_Scan(u8 mode);
/*********************************************************************************
 * @Function	:  按键事件返回
 * @Input		:  None
 *						:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
 * @Date			:  2022-07-23
 **********************************************************************************/
void Send_Cmd(void);

#endif /* __KEY_H */

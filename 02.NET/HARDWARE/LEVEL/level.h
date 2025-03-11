#ifndef __LEVEL_H
#define __LEVEL_H

#include "stm32f10x.h"

//  引脚定义
#define LEVEL1_GPIO_CLK RCC_APB2Periph_GPIOA
#define LEVEL1_GPIO_PORT GPIOA
#define LEVEL1_GPIO_PIN GPIO_Pin_5

#define LEVEL2_GPIO_CLK RCC_APB2Periph_GPIOA
#define LEVEL2_GPIO_PORT GPIO6
#define LEVEL2_GPIO_PIN GPIO_Pin_6

#define LEVEL3_GPIO_CLK RCC_APB2Periph_GPIOB
#define LEVEL3_GPIO_PORT GPIOB
#define LEVEL3_GPIO_PIN GPIO_Pin_14

#define LEVEL1 PAin(5) // 读取按键0
#define LEVEL2 PAin(6) // 读取按键1
#define LEVEL3 GPIO_ReadInputDataBit(LEVEL3_GPIO_PORT, LEVEL3_GPIO_PIN) // 读取按键2

#define LEVEL1_PRES 1 // LEVEL1按下
#define LEVEL2_PRES 2 // LEVEL2按下
#define LEVEL3_PRES 3 // LEVEL3按下

#define LEVEL_Scan_Time 30 // 短按时长时间判断

void Level_GPIO_Config(void);

#endif /* __LEVEL_H */

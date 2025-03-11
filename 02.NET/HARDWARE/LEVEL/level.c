#include "level.h"
#include "delay.h"

/*********************************************************************************
 * @Function	:  初始化控制LED的IO
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  None
 * @Date			:  2022-07-23
 **********************************************************************************/
void Level_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键端口的时钟*/
    RCC_APB2PeriphClockCmd(LEVEL3_GPIO_CLK, ENABLE);

    // 选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = LEVEL1_GPIO_PIN;
    // 设置按键的引脚为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // 使用结构体初始化按键
    GPIO_Init(LEVEL1_GPIO_PORT, &GPIO_InitStructure);

//    // 选择按键的引脚
//    GPIO_InitStructure.GPIO_Pin = LEVEL2_GPIO_PIN;
//    // 设置按键的引脚为浮空输入
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    // 使用结构体初始化按键
//    GPIO_Init(LEVEL2_GPIO_PORT, &GPIO_InitStructure);

    //	// 选择按键的引脚
    //	GPIO_InitStructure.GPIO_Pin = LEVEL3_GPIO_PIN;
    //	// 设置按键的引脚为浮空输入
    //	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    //	// 使用结构体初始化按键
    //	GPIO_Init(LEVEL3_GPIO_PORT, &GPIO_InitStructure);
}

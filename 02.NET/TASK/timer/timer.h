#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可
#define GENERAL_TIM2 0
#define GENERAL_TIM3 0
#define GENERAL_TIM4 1
#define GENERAL_TIM5 0
#define GENERAL_TIM6 0
#define GENERAL_TIM7 0
/*********************************************************************************
 * @Function	: TIME实例
 **********************************************************************************/
typedef enum
{
	TIM_2,
	TIM_3,
	TIM_4,
	TIM_5,
	TIM_6,
	TIM_7,
	TIME_NUM
} TimeModule;
/*********************************************************************************
 * @Function	: 固定分频率
 **********************************************************************************/
#define GENERAL_TIM2_Period (1000) // 1ms一次中断
#define GENERAL_TIM2_Prescaler 72  // 时钟预分频数

#define GENERAL_TIM3_Period (1000) // 1ms一次中断
#define GENERAL_TIM3_Prescaler 72	   // 时钟预分频数

#define GENERAL_TIM4_Period (1000) // 1ms一次中断
#define GENERAL_TIM4_Prescaler 72	   // 时钟预分频数

#define GENERAL_TIM5_Period (1000 - 1) // 1ms一次中断
#define GENERAL_TIM5_Prescaler 71	   // 时钟预分频数

#define GENERAL_TIM6_Period 90	  // 100us一次中断
#define GENERAL_TIM6_Prescaler 80 // 时钟预分频数

#define GENERAL_TIM7_Period (1000) // 1ms一次中断
#define GENERAL_TIM7_Prescaler 72  // 时钟预分频数
/*********************************************************************************
 * @Function	: TIM2-5 初始化
 * @Input		: time,TIM定时器（TIM_2-TIM_5）
 *						PreemptionPriority，抢占优先级
 *						SubPriority，子优先级
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
void GENERAL_TIM_Init(TimeModule time, u8 PreemptionPriority, u8 SubPriority);
/*********************************************************************************
 * @Function	: TIM定时器中断初始化
 * @Input		: time,TIM定时器（TIM_2-TIM_5）
 *						PreemptionPriority，抢占优先级
 *						SubPriority，子优先级
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
void GENERAL_TIM_NVIC_Config(TimeModule time, u8 PreemptionPriority, u8 SubPriority);
#endif

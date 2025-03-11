#include "timer.h"
#include "delay.h"
#include "task.h"
#include "usart.h"
#include "stdbool.h"

// 网络设备驱动
#include "esp8266.h"
#include "onenet.h"
/*********************************************************************************
 * @Function	: 通用定时器的选择（TIM2-TIM5）
 *	@Author   : HR
 * @Date			: 2022-08-30
 **********************************************************************************/
#if GENERAL_TIM2
u32 time_2 = 0; // ms 计时变量
extern u8 Connect_Net;
#endif
#if GENERAL_TIM3
u32 time_3 = 0; // ms 计时变量
extern u8 Connect_Net;
#endif
#if GENERAL_TIM4
u32 time_4 = 0; // ms 计时变量
extern u8 Connect_Net;
#endif
#if GENERAL_TIM5
U16 time_5 = 0; // ms 计时变量
#endif
#if GENERAL_TIM6
U16 time_6 = 0; // ms 计时变量
#endif
#if GENERAL_TIM7
U16 Usart1_Time = 0;		 // ms 计时变量
U16 Usart2_Time = 0;		 // ms 计时变量
U16 Usart3_Time = 0;		 // ms 计时变量
U16 Usart4_Time = 0;		 // ms 计时变量
U16 Usart5_Time = 0;		 // ms 计时变量
U8 bTimeout1s = MY_FALSE;	 /** < 1秒钟定时器超时标志 */
U8 bTimeout2s = MY_FALSE;	 /** < 2秒钟定时器超时标志 */
U8 bTimeout500ms = MY_FALSE; /** < 500毫秒定时器超时标志 */
U8 bTimeout250ms = MY_FALSE; /** < 50毫秒定时器超时标志 */
U8 bTimeout10ms = MY_FALSE;	 /** < 10毫秒定时器超时标志 */
U32 TimeCount1ms = 0;		 /** < 10毫秒超时计数 */
#endif
/*********************************************************************************
 * @Function	: TIM定时器选择
 * @Input		: time,TIM定时器（TIM_2-TIM_5）
 * @Output		:	TIM模块
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
static TIM_TypeDef *TimeModule_get(TimeModule time)
{
	TIM_TypeDef *timeTemp;
	switch ((u8)time) // UART模块选择
	{
#if GENERAL_TIM2
	case TIM_2:
		timeTemp = TIM2;
		break;
#endif
#if GENERAL_TIM3
	case TIM_3:
		timeTemp = TIM3;
		break;
#endif
#if GENERAL_TIM4
	case TIM_4:
		timeTemp = TIM4;
		break;
#endif
#if GENERAL_TIM5
	case TIM_5:
		timeTemp = TIM5;
		break;
#endif
#if GENERAL_TIM6
	case TIM_6:
		timeTemp = TIM6;
		break;
#endif
#if GENERAL_TIM7
	case TIM_7:
		timeTemp = TIM7;
		break;
#endif
	}
	return timeTemp;
}
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
void GENERAL_TIM_NVIC_Config(TimeModule time, u8 PreemptionPriority, u8 SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断组为0
	// Usart NVIC 配置
	switch ((u8)time) // 中断通道配置
	{
	case TIM_2:
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		break; // TIM2中断
	case TIM_3:
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		break;
	case TIM_4:
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		break;
		//	case TIM_5:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
		//		break;
		//	case TIM_6:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
		//		break;
		//	case TIM_7:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
		//		break;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;			   // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);											   // 根据指定的参数初始化VIC寄存器
}
/*********************************************************************************
 * @Function	: TIM时钟使能
 * @Input		: time,time时钟（TIM_1-TIM_5）
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
static void GENERAL_TIMClock_Enable(TimeModule time)
{
	switch ((u8)time)
	{
	case TIM_2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		break; // 通用定时器2时钟使能
	case TIM_3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		break; // 通用定时器2时钟使能
	case TIM_4:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		break; // 通用定时器2时钟使能
	case TIM_5:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		break; // 通用定时器2时钟使能
	case TIM_6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		break; // 通用定时器2时钟使能
	case TIM_7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
		break; // 通用定时器2时钟使能
	}
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef;
 *-----------------------------------------------------------------------------
 */
static void GENERAL_TIM_Mode_Config(TimeModule time)
{
	TIM_TypeDef *timeTemp;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	timeTemp = TimeModule_get(time); // TIME时钟选择
	switch ((u8)time)
	{
	case TIM_2:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM2_Prescaler;
		break; // 时钟预分频数
	case TIM_3:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM3_Period - 1; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM3_Prescaler - 1;
		break; // 时钟预分频数
	case TIM_4:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM4_Period; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM4_Prescaler;
		break; // 时钟预分频数
	case TIM_5:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM5_Period - 1; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM5_Prescaler - 1;
		break; // 时钟预分频数
	case TIM_6:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM6_Period - 1; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM6_Prescaler - 1;
		break; // 时钟预分频数
	case TIM_7:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM7_Period - 1; // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM7_Prescaler - 1;
		break; // 时钟预分频数
	}
	// 时钟分频因子 ，没用到不用管
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// 初始化定时器
	TIM_TimeBaseInit(timeTemp, &TIM_TimeBaseStructure);
	// 清除计数器中断标志位
	TIM_ClearFlag(timeTemp, TIM_FLAG_Update);
	// 开启计数器中断
	TIM_ITConfig(timeTemp, TIM_IT_Update, ENABLE);
	// 使能计数器
	TIM_Cmd(timeTemp, DISABLE); // 关闭定时器
}
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
void GENERAL_TIM_Init(TimeModule time, u8 PreemptionPriority, u8 SubPriority)
{
	GENERAL_TIMClock_Enable(time);									// TIM使能化
	GENERAL_TIM_NVIC_Config(time, PreemptionPriority, SubPriority); // TIM中断优先级配置
	GENERAL_TIM_Mode_Config(time);									// TIM初始化
}
/*********************************************************************************
 * @Function	:  TIMER2定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM2
void TIM2_IRQHandler(void) // TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{
		timer_ticks();
		time_2++;
		// 一分钟平台未回消息判断是否掉网
		if (time_2 % 1000 == 0)
		{
			time_2 = 0;
			if (Connect_Net > 0)
			{
				Connect_Net--;
			}
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif
/*********************************************************************************
 * @Function	:  TIMER3定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM3
void TIM3_IRQHandler(void) // TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{

		timer_ticks();
		time_3++;
		if (time_3 % 25 == 0)
		{
			// 密码输入
			Input_Password();
		}

		// 一分钟平台未回消息判断是否掉网
		if (time_3 % 1000 == 0)
		{
			time_3 = 0;
			if (Connect_Net > 0)
			{
				Connect_Net--;
			}
		}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif
/*********************************************************************************
 * @Function	:  TIMER4定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM4
void TIM4_IRQHandler(void) // TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{

		time_4++; // 时间记录器
		timer_ticks();

		//		if (time_4 % 25 == 0)
		//		{
		//			 // 密码输入
		//       Check_Key_ON_OFF();
		//		}

		// 一分钟平台未回消息判断是否掉网
		if (time_4 % 1000 == 0)
		{
			time_4 = 0;
			Automation_Close();
			// if (Connect_Net > 0)
			// {
			// 	Connect_Net--;
			// }
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif
/*********************************************************************************
 * @Function	:  TIMER5定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM5
void TIM5_IRQHandler(void) // TIM5中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{

		time_5++; // 时间记录器
		// 满足条件关定时器
		if (time_5 > 9)
		{
			time_5 = 0;
			TIM_Cmd(TIM5, DISABLE); // 关闭定时器
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif
/*********************************************************************************
 * @Function	:  TIMER5定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM6
void TIM6_IRQHandler(void) // TIM5中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{
		Ultrasound_Probe_Work_Control();
		// 满足条件关定时器
		if (time_6 > 99)
		{
			time_6 = 0;
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif
/*********************************************************************************
 * @Function	:  TIMER5定时器中断服务
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM7
void TIM7_IRQHandler(void) // TIM5中断
{

	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) // 检查TIM3更新中断发生与否
	{
		TimeCount1ms++;
		if (bTimeout10ms == MY_TRUE)
		{
			if (Usart1_Time > 0)
			{
				Usart1_Time++; // 串口五接收间隔判断
			}
			else if (Usart2_Time > 0)
			{
				Usart2_Time++; // 串口五接收间隔判断
			}
			else if (Usart5_Time > 0)
			{
				Usart5_Time++; // 串口五接收间隔判断
			}
			else if (Usart4_Time > 0)
			{
				Usart4_Time++; // 串口四接收间隔判断
			}
			else if (Usart3_Time > 0)
			{
				Usart3_Time++; // 串口三接收间隔判断
			}
			// 满足条件关定时器
			if (Usart5_Time == 10 || Usart3_Time == 10 || Usart4_Time == 10 || Usart1_Time == 10 || Usart2_Time == 10)
			{
				bTimeout10ms = MY_FALSE;
			}
		}
		if (TimeCount1ms % 250 == 0) // 250ms
		{
			bTimeout250ms = MY_TRUE;
		}
		if (TimeCount1ms % 500 == 0) // 500ms
		{
			bTimeout500ms = MY_TRUE;
		}
		if (TimeCount1ms % 1000 == 0) // 1s
		{
			bTimeout1s = MY_TRUE;
		}
		if (TimeCount1ms % 2000 == 0) // 1s
		{
			bTimeout2s = MY_TRUE;
			TimeCount1ms = 0;
		}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update); // 清除TIMx更新中断标志
	}
}
#endif

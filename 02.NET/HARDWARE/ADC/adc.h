#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define SMOG_PIN46_R 1000 // 烟雾传感器管脚4、6接出到地的电阻值

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define ADC_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_CLK RCC_APB2Periph_ADC1

#define ADC_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK RCC_APB2Periph_GPIOA
#define ADC_PORT GPIOA

// 转换通道个数
#define NOFCHANEL 1

#define ADC_PIN1 GPIO_Pin_0
#define ADC_CHANNEL1 ADC_Channel_0

#define ADC_PIN2 GPIO_Pin_1
#define ADC_CHANNEL2 ADC_Channel_1

// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define ADCx ADC1
#define ADC_DMA_CHANNEL DMA1_Channel1
#define ADC_DMA_CLK RCC_AHBPeriph_DMA1

/**************************函数声明********************************/
void ADCx_Init(void);
u16 Smog_Trans_Concentration(void);
u16 Light_value(void);
#endif /* __ADC_H */

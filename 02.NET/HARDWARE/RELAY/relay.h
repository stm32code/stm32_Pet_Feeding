#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"
#include "sys.h"
#include "git.h"

#define BEEP_GPIO_PORT GPIOB
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN GPIO_Pin_7

#define RELAY4_GPIO_PORT GPIOB
#define RELAY4_GPIO_CLK RCC_APB2Periph_GPIOB
#define RELAY4_GPIO_PIN GPIO_Pin_6

#define RELAY4_TOGGLE digitalToggle(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN)
#define RELAY4_OFF digitalLo(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN)
#define RELAY4_ON digitalHi(RELAY4_GPIO_PORT, RELAY4_GPIO_PIN)

#define BEEP_TOGGLE digitalToggle(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF digitalHi(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_ON digitalLo(BEEP_GPIO_PORT, BEEP_GPIO_PIN)

#define BEEP PBout(7)
void RELAY_GPIO_Config(void);
void Mqtt_Relay(U8 Cmd);

#endif /* __RELAY_H */

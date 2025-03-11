#include "stm32f10x.h"

#define MOTOR_Z       1   //正转 顺时针转动
#define MOTOR_F   0   //反转 逆时针转动
#define MOTOR_SPEED           3   //转速

#define DD_GPIO_PORT    	GPIOB
#define DD_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define DD_GPIO_PIN		    GPIO_Pin_15	


#define CC_GPIO_PORT    	GPIOB			              
#define CC_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define CC_GPIO_PIN		    GPIO_Pin_14	

#define BB_GPIO_PORT    	GPIOB			              
#define BB_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define BB_GPIO_PIN		    GPIO_Pin_13	

#define AA_GPIO_PORT    	GPIOB			              
#define AA_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define AA_GPIO_PIN		    GPIO_Pin_12	

#define DD_0    GPIO_ResetBits(DD_GPIO_PORT , DD_GPIO_PIN) 	//电机控制口，连接电机驱动板IN4 	
#define DD_1    GPIO_SetBits(DD_GPIO_PORT , DD_GPIO_PIN) 	  //电机控制口，连接电机驱动板IN4 
#define CC_0    GPIO_ResetBits(CC_GPIO_PORT , CC_GPIO_PIN) 	//电机控制口，连接电机驱动板IN3 	
#define CC_1    GPIO_SetBits(CC_GPIO_PORT , CC_GPIO_PIN) 	  //电机控制口，连接电机驱动板IN3 
#define BB_0    GPIO_ResetBits(BB_GPIO_PORT , BB_GPIO_PIN) 	//电机控制口，连接电机驱动板IN2 	
#define BB_1    GPIO_SetBits(BB_GPIO_PORT , BB_GPIO_PIN) 	  //电机控制口，连接电机驱动板IN2 
#define AA_0    GPIO_ResetBits(AA_GPIO_PORT , AA_GPIO_PIN) 	//电机控制口，连接电机驱动板IN1 	
#define AA_1    GPIO_SetBits(AA_GPIO_PORT , AA_GPIO_PIN) 	  //电机控制口，连接电机驱动板IN1 

void motor_init(void);
void MotorStep(uint8_t X,uint16_t Speed);
void MotorStop(void);
void Motor_Ctrl_Angle_F(int angle,int n);
void Motor_Ctrl_Angle_Z(int angle,int n);



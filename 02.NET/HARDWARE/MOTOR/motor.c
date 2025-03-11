#include "stm32f10x.h"
#include "motor.h"
#include "delay.h"


/***************************************************************************************************
 * 描  述 : 初始化步进电机用GPIO口
 * 参  数 : 无
 * 返回值 : 无
 **************************************************************************************************/
void motor_init(void)
{
   //定义IO初始化配置结构体
   GPIO_InitTypeDef  GPIO_InitStructure;
   //打开PA端口时钟
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	 
	 //配置的IO是PF0~PF3
   GPIO_InitStructure.GPIO_Pin  = AA_GPIO_PIN | BB_GPIO_PIN | CC_GPIO_PIN | DD_GPIO_PIN;		
	 //配置为推挽输出
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 //IO口速度为50MHz
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 //配置IO
   GPIO_Init(GPIOB, &GPIO_InitStructure);	

}

/***************************************************************************
 * 描  述 : 步进电机驱动函数
 * 参  数 : X表示正反转；Speed表示转速设置
 * 返回值 : 无
 ***************************************************************************/
void MotorStep(uint8_t X,uint16_t Speed)
{
  if(X==1)   //顺时针转动
	{
		DD_0;CC_0;BB_1;AA_1;	
    delay_ms(Speed);  //转速调节

		DD_0;CC_1;BB_1;AA_0;	
    delay_ms(Speed);  //转速调节

		DD_1;CC_1;BB_0;AA_0;	
    delay_ms(Speed);  //转速调节

		DD_1;CC_0;BB_0;AA_1;	
    delay_ms(Speed);  //转速调节
  }
	else       //逆时针转动
	{
		DD_1;CC_0;BB_0;AA_1;	
    delay_ms(Speed);  //转速调节

		DD_1;CC_1;BB_0;AA_0;	
    delay_ms(Speed);  //转速调节

		DD_0;CC_1;BB_1;AA_0;	
    delay_ms(Speed);  //转速调节

		DD_0;CC_0;BB_1;AA_1;	
    delay_ms(Speed);  //转速调节
  }	
}

/***************************************************************************
 * 描  述 : 步进电机停止转动
 * 参  数 : 无
 * 返回值 : 无
 ***************************************************************************/
void MotorStop(void)
{
   DD_0;CC_0;BB_0;AA_0;
}

/***************************************************************************
 * 描  述 : 步进电机旋转度
 * 参  数 : 无
 * 返回值 : 无
 ***************************************************************************/
void Motor_Ctrl_Angle_F(int angle,int n){
	u16 j;
	for(j=0;j<64*angle/45;j++) 
	{
		MotorStep(0,n);
	}
}
void Motor_Ctrl_Angle_Z(int angle,int n){
	u16 j;
	for(j=0;j<64*angle/45;j++) 
	{
		MotorStep(1,n);
		}
	}
//switch(*RxMessage.Data)
//			{				 
//				case 0x11://收到电机状态	
//						Motor_Ctrl_Angle_Z(180,3);
//						CAN_SetMsg(&TxMessage,1);
//						/*把报文存储到发送邮箱，发送*/
//						CAN_Transmit(CANx, &TxMessage);				
//					break;
//				case 0x12://电机旋转180°	
//					 Motor_Ctrl_Angle_F(180,3);
//					 CAN_SetMsg(&TxMessage,2);
//						/*把报文存储到发送邮箱，发送*/
//					 CAN_Transmit(CANx, &TxMessage);	
//					break;
//				case 0x13://电机持续运行	
//					  MotorStep(MOTOR_Z,3);
//						CAN_SetMsg(&TxMessage,3);
//						/*把报文存储到发送邮箱，发送*/
//						CAN_Transmit(CANx, &TxMessage);
//						flage=1;
//					break;
//				case 0x14://电机停止运行	
//						MotorStop();
//						CAN_SetMsg(&TxMessage,4);
//						/*把报文存储到发送邮箱，发送*/
//						CAN_Transmit(CANx, &TxMessage);
//						flage=0;
//					break;
//        default:   	
//        break;
//			}


#ifndef __GIT_H
#define __GIT_H
#include "stdio.h"

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

// 服务器信息
#define SSID "CMCC-KPUD"				 // 路由器SSID名称
#define PASS "p675eph9"			 // 路由器密码
#define ServerIP "183.230.40.39" // 服务器IP地址
#define ServerPort 6002			 // 服务器IP地址端口号
// 设备信息
#define PROID "634589"	  // 产品ID
#define DEVID "1215496310" // 设备ID
#define AUTH_INFO "11"	  // 鉴权信息

// MQTT主题
#define S_TOPIC_NAME "/broadcast/h9sjXmbDhjc/test1" // 需要订阅的主题
#define P_TOPIC_NAME "/broadcast/h9sjXmbDhjc/test2" // 需要发布的主题


                                
//  设备使用外设定义
#define OLED 1 // 是否使用OLED
// 检测次数
#define SIZE 10

// 自定义执行结果类型
typedef enum
{
	MY_SUCCESSFUL = 0x01, // 成功
	MY_FAIL = 0x00		  // 失败

} mySta; // 成功标志位

typedef enum
{
	OPEN = 0x01, // 打开
	CLOSE = 0x00 // 关闭

} On_or_Off_TypeDef; // 成功标志位

typedef enum
{
	DERVICE_SEND = 0x00, // 设备->平台
	PLATFORM_SEND = 0x01 // 平台->设备

} Send_directino; // 发送方向

typedef struct
{
	U8 App;	 // 指令模式
	U8 Page; // 页面

	U8 Flage;	// 模式选择
	U16 people; // 人数
	U16 light;	// 光照
	U16 somg;	// 烟雾浓度

	F32 wight;		   // 变化重量
	F32 git_wight;	   // 累计重量
	U8 wight_time;	   // 重量时间
	F32 average_wight; // 平均重量
	F32 add_wight;	   // 累计重量
	F32 Temp;		   // 温度
	F32 Humi;		   // 湿度

} Data_TypeDef; // 数据参数结构体

typedef struct
{

	U16 somg_value;	 // 气体阈值
	U16 humi_value;	 // 湿度阈值
	U16 temp_value;	 // 温度阈值
	F32 wight_value; // 重量阈值

} Threshold_Value_TypeDef; // 数据参数结构体

typedef struct
{

	U8 door;
	U8 eat;
	U8 water;
	U8 open;
	U8 check_open; // 监测是否由物体

	U8 SG90_Turn;

} Device_Satte_Typedef; // 状态参数结构体

typedef struct
{
	U16 pwd;

} Pwd_Box_Typedef; // 密码

// 获取全局变量
extern Data_TypeDef Data_init;
extern Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
extern Device_Satte_Typedef device_state_init;		 // 设备状态
extern Pwd_Box_Typedef box_pwd_init;				 // 设备密码

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data);
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// 更新OLED显示屏中内容
mySta Update_oled_massage(void);
// 更新设备状态
mySta Update_device_massage(void);
// 解析json数据
mySta massage_parse_json(char *message);
// 验证密码并开锁
void Check_Key_ON_OFF(void);
// 自动关闭舵机
void Automation_Close(void);

#endif

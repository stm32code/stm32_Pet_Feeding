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

// ��������Ϣ
#define SSID "CMCC-KPUD"				 // ·����SSID����
#define PASS "p675eph9"			 // ·��������
#define ServerIP "183.230.40.39" // ������IP��ַ
#define ServerPort 6002			 // ������IP��ַ�˿ں�
// �豸��Ϣ
#define PROID "634589"	  // ��ƷID
#define DEVID "1215496310" // �豸ID
#define AUTH_INFO "11"	  // ��Ȩ��Ϣ

// MQTT����
#define S_TOPIC_NAME "/broadcast/h9sjXmbDhjc/test1" // ��Ҫ���ĵ�����
#define P_TOPIC_NAME "/broadcast/h9sjXmbDhjc/test2" // ��Ҫ����������


                                
//  �豸ʹ�����趨��
#define OLED 1 // �Ƿ�ʹ��OLED
// ������
#define SIZE 10

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���

typedef struct
{
	U8 App;	 // ָ��ģʽ
	U8 Page; // ҳ��

	U8 Flage;	// ģʽѡ��
	U16 people; // ����
	U16 light;	// ����
	U16 somg;	// ����Ũ��

	F32 wight;		   // �仯����
	F32 git_wight;	   // �ۼ�����
	U8 wight_time;	   // ����ʱ��
	F32 average_wight; // ƽ������
	F32 add_wight;	   // �ۼ�����
	F32 Temp;		   // �¶�
	F32 Humi;		   // ʪ��

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U16 somg_value;	 // ������ֵ
	U16 humi_value;	 // ʪ����ֵ
	U16 temp_value;	 // �¶���ֵ
	F32 wight_value; // ������ֵ

} Threshold_Value_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U8 door;
	U8 eat;
	U8 water;
	U8 open;
	U8 check_open; // ����Ƿ�������

	U8 SG90_Turn;

} Device_Satte_Typedef; // ״̬�����ṹ��

typedef struct
{
	U16 pwd;

} Pwd_Box_Typedef; // ����

// ��ȡȫ�ֱ���
extern Data_TypeDef Data_init;
extern Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
extern Device_Satte_Typedef device_state_init;		 // �豸״̬
extern Pwd_Box_Typedef box_pwd_init;				 // �豸����

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data);
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// ����OLED��ʾ��������
mySta Update_oled_massage(void);
// �����豸״̬
mySta Update_device_massage(void);
// ����json����
mySta massage_parse_json(char *message);
// ��֤���벢����
void Check_Key_ON_OFF(void);
// �Զ��رն��
void Automation_Close(void);

#endif

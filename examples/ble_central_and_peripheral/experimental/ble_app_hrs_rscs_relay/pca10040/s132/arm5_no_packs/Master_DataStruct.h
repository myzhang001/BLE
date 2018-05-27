#ifndef __MASTER_DATASTRUCT_H
#define __MASTER_DATASTRUCT_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"





#define MAX_DEVICE_NUM  8							//��������豸����



//08F ���ݽṹ

//08F �����������ݽṹ

typedef enum{
    E_MOTOR_OFF 	= 0X00,
	E_MOTOR_LOW 	= 0X01,
	E_MOTOR_MIDDLE  = 0X02,
	E_MOTOR_HIGH    = 0X03
}_e_08f_motor;


//�豸״̬����
typedef struct{
	uint8_t 		Device_08F_POWER;       //���ػ�
	_e_08f_motor    Motor_Level;            //�����λ
	uint32_t        PM2_5_Value;            //pm2.5 �ȼ�
	uint8_t         Anion;                  //������Ũ��
	uint8_t         Filter;                 //����
}_real_staus_08F;



//�豸��������

typedef struct{
	uint8_t 		Device_08F_POWER;       //���ػ�
	_e_08f_motor    Motor_Level;            //�����λ

}_control_08F;




//�·����ݽṹ
typedef struct{
	uint8_t      Device_09_Power;          //���ػ�
	uint8_t      Motor_Level;              //��ﵵλ	
}_contorl_09;


typedef struct{
	uint8_t      Device_09_Power;          //���ػ�
	uint8_t      Motor_Level;              //��ﵵλ
	uint8_t      Work_Time;                //�豸����ʱ��
}_real_status_09;


















#endif




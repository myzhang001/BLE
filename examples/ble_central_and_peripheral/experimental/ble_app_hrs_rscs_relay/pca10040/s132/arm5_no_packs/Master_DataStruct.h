#ifndef __MASTER_DATASTRUCT_H
#define __MASTER_DATASTRUCT_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"





#define MAX_DEVICE_NUM  8							//最大连接设备个数



//08F 数据结构

//08F 控制数据数据结构

typedef enum{
    E_MOTOR_OFF 	= 0X00,
	E_MOTOR_LOW 	= 0X01,
	E_MOTOR_MIDDLE  = 0X02,
	E_MOTOR_HIGH    = 0X03
}_e_08f_motor;


//设备状态数据
typedef struct{
	uint8_t 		Device_08F_POWER;       //开关机
	_e_08f_motor    Motor_Level;            //风机档位
	uint32_t        PM2_5_Value;            //pm2.5 等级
	uint8_t         Anion;                  //负离子浓度
	uint8_t         Filter;                 //滤网
}_real_staus_08F;



//设备控制数据

typedef struct{
	uint8_t 		Device_08F_POWER;       //开关机
	_e_08f_motor    Motor_Level;            //风机档位

}_control_08F;




//新风数据结构
typedef struct{
	uint8_t      Device_09_Power;          //开关机
	uint8_t      Motor_Level;              //马达档位	
}_contorl_09;


typedef struct{
	uint8_t      Device_09_Power;          //开关机
	uint8_t      Motor_Level;              //马达档位
	uint8_t      Work_Time;                //设备运行时间
}_real_status_09;


















#endif




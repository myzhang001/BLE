#ifndef __MASTER_DATASTRUCT_H
#define __MASTER_DATASTRUCT_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"





#define MAX_DEVICE_NUM  8							//最大连接设备个数
#define MAC_LEN         6                           //MAC 地址长度


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





//mac 地址 和 Array 索引匹配
typedef struct{

	uint8_t mac_addr[6];             //保存mac 地址
	uint8_t index;                   //索引
	
}_mac_index_fix;



typedef struct{
	uint8_t         device_num; 					  //设备个数
	uint8_t         max_index_decice;                 //最大索引
	uint8_t         index_sort[MAX_DEVICE_NUM];       //索引序号排序
	_mac_index_fix  mac_list_array[MAX_DEVICE_NUM];   //设备mac list 和index 列表
}_mac_index_data;



typedef struct{
	uint8_t         Device_Num;                     			//总设备个数
	_real_staus_08F Device_08F_Array[MAX_DEVICE_NUM];    		//08 实时数据
	_control_08F 	Device_Control_08F_Array[MAX_DEVICE_NUM];   //08 控制数据

	_mac_index_data mac_index; 									//mac list index				

}_datastruct_08F;


typedef struct{
	uint8_t         Device_Num;                     			//总设备个数
	_real_status_09 Device_09_Array[MAX_DEVICE_NUM];			//09 实时数据
	_contorl_09    	Device_Control_09_Array[MAX_DEVICE_NUM];	//09 控制数据
}_datastruct_09;




//---------------------rm661 数据结构




typedef struct{
	uint8_t      control_light;              //开关灯
	uint8_t      control_aroma;              //开关香薰	
}_contorl_rm661;


typedef struct{
	uint8_t      real_light;                 //开关机

	                
}_real_status_rm661;



typedef struct{
	uint8_t             Device_Num;                     			//总设备个数
	_real_status_rm661  Device_rm661_Array[MAX_DEVICE_NUM];			//661 实时数据
	_contorl_rm661    	Device_Control_rm661_Array[MAX_DEVICE_NUM];	//661 控制数据
    _mac_index_data     mac_index; 									//mac list index	
    
}_datastruct_rm661;







uint8_t Del_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6]);
uint8_t Add_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6]);


extern _datastruct_08F   System_08F;       //08F  系统变量

extern _datastruct_rm661 rm661_data;       //661 全局变量

extern void Debug_Device_Info(void);       //打印调试信息

extern uint16_t  mac_match_hanle(_mac_index_data *device_list ,uint8_t mac[6]);


#endif




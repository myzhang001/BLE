#include "Master_DataStruct.h"


	



typedef struct{
	uint8_t         Device_Num;                     //总设备个数
	_real_staus_08F Device_08F_Array[MAX_DEVICE_NUM];    		//08 实时数据
	_control_08F 	Device_Control_08F_Array[MAX_DEVICE_NUM];    //08 控制数据
}_datastacut_08F;


typedef struct{
	uint8_t         Device_Num;                     //总设备个数
	_real_status_09 Device_09_Array[MAX_DEVICE_NUM];				//09 实时数据
	_contorl_09    	Device_Control_09_Array[MAX_DEVICE_NUM];	    //09 控制数据
}_datastruct_09;








//清零数据   device_index 0 - 7

void Master_ResetBuffer_08F_RealStatus(uint8_t device_index)
{
	memset(&Device_08F_Array[device_index],0,sizeof(_real_staus_08F));
}


//清零所有数据   all
void Master_ResetBuffer_08F_RealStatus_All(void)
{
	memset(&Device_08F_Array,0,sizeof(_real_staus_08F)*8);
}







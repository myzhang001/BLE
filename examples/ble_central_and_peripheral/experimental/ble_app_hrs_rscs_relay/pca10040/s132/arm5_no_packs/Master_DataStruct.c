#include "Master_DataStruct.h"


	



typedef struct{
	uint8_t         Device_Num;                     //���豸����
	_real_staus_08F Device_08F_Array[MAX_DEVICE_NUM];    		//08 ʵʱ����
	_control_08F 	Device_Control_08F_Array[MAX_DEVICE_NUM];    //08 ��������
}_datastacut_08F;


typedef struct{
	uint8_t         Device_Num;                     //���豸����
	_real_status_09 Device_09_Array[MAX_DEVICE_NUM];				//09 ʵʱ����
	_contorl_09    	Device_Control_09_Array[MAX_DEVICE_NUM];	    //09 ��������
}_datastruct_09;








//��������   device_index 0 - 7

void Master_ResetBuffer_08F_RealStatus(uint8_t device_index)
{
	memset(&Device_08F_Array[device_index],0,sizeof(_real_staus_08F));
}


//������������   all
void Master_ResetBuffer_08F_RealStatus_All(void)
{
	memset(&Device_08F_Array,0,sizeof(_real_staus_08F)*8);
}







#ifndef __NUS_MASTER_DATAHANDLE_H
#define __NUS_MASTER_DATAHANDLE_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"




typedef enum{
	START_INDEX   = 0,					//��ʼ����
	DATA_LENGTH_INDIX_LOW = 1,          //���ݳ��ȵ�λ
	DATA_LENGTH_INDIX_HIGH = 2,         //���ݳ��ȸ�λ
	DATA_COMMOND_WORD = 10              //����������
}data_indix;







bool NUS_C_Filter_Connected_Handle(uint32_t handle);   






#endif




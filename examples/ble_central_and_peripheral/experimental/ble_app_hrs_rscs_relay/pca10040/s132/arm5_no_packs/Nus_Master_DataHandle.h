#ifndef __NUS_MASTER_DATAHANDLE_H
#define __NUS_MASTER_DATAHANDLE_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"


typedef struct{

	uint16_t  Data_Length;    //保存数据长度
	uint16_t Common_World;    //保存命令字
	uint8_t  Device_Type;     //设备类型
	uint8_t  MacAddr_Device[6];	//设备mac 地址
}_s_analsis_word;


typedef enum{
	START_INDEX   = 0,					//起始索引
	DATA_LENGTH_INDIX_LOW = 1,          //数据长度地位
	DATA_LENGTH_INDIX_HIGH = 2,         //数据长度高位
    DATA_DEVICE_TYPE_INDEX = 4,			//设备类型
	DATA_DEVICE_MAC_INDEX = 5,			//设备mac 地址索引
	DATA_COMMOND_WORD = 11,             //控制字索引
	DATA_CONTENT_INDEX = 13			    //数据内容索引
	
}data_indix;


typedef struct
{
	bool 	  receive_start_flag;
	uint8_t   data_index;
	uint16_t  total_len;
	uint8_t   rece_data[180];
}somputon_data_recv_t;




bool NUS_C_Filter_Connected_Handle(uint32_t handle);   
void nus_data_handle(uint32_t nus_c_conn_handle, uint8_t *data, uint8_t length);
unsigned char Crc8( unsigned char *Address, unsigned char Length );




#endif




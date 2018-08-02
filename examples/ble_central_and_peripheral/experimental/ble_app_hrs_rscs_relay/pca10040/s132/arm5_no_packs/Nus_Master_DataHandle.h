#ifndef __NUS_MASTER_DATAHANDLE_H
#define __NUS_MASTER_DATAHANDLE_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"


typedef struct{

	uint16_t  Data_Length;    //�������ݳ���
	uint16_t Common_World;    //����������
	uint8_t  Device_Type;     //�豸����
	uint8_t  MacAddr_Device[6];	//�豸mac ��ַ
}_s_analsis_word;


typedef enum{
	START_INDEX   = 0,					//��ʼ����
	DATA_LENGTH_INDIX_LOW = 1,          //���ݳ��ȵ�λ
	DATA_LENGTH_INDIX_HIGH = 2,         //���ݳ��ȸ�λ
    DATA_DEVICE_TYPE_INDEX = 4,			//�豸����
	DATA_DEVICE_MAC_INDEX = 5,			//�豸mac ��ַ����
	DATA_COMMOND_WORD = 11,             //����������
	DATA_CONTENT_INDEX = 13			    //������������
	
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




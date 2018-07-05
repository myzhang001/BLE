#ifndef __slave_device_data_h
#define __slave_device_data_h


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"

#include "User_MacList.h"
#include "uart_queue.h"
#include "NUS_Master_DataHandle.h"
#include "Protocol_Analysis.h"



typedef enum{

    E_REAL_DATA = 0x00,    //ʵʱ����
    E_CONTROL_ACK = 0x01,  //��������Ӧ��


}_uart_status;




typedef struct{
    uint16_t conn_handle;           //���
    uint8_t  conn_data_flag;        //�·���־
    uint8_t  data_buffer[50];       //���ݻ���
    uint8_t  length;                //���ݳ���
   
}_data_struct_control;

 
extern _data_struct_control control_data;  //�·��������ݽṹ






void WIFI_Decode(void);
void master_cb_init(appRecvHandler_Cb cb);
void Master_App_RecvHandler(uint16_t conn_handle,uint16_t command, uint8_t* data,uint16_t len,_e_machine_model device_type,uint8_t mac_addr[6]);

void master_send_string(uint8_t *p_data,uint8_t len);




#endif




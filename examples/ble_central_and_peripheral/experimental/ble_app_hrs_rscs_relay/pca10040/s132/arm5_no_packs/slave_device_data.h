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

    E_REAL_DATA = 0x00,    //实时数据
    E_CONTROL_ACK = 0x01,  //控制数据应答


}_uart_status;




typedef struct{
    uint16_t conn_handle;           //句柄
    uint8_t  conn_data_flag;        //下发标志
    uint8_t  data_buffer[50];       //数据缓冲
    uint8_t  length;                //数据长度
   
}_data_struct_control;

 
extern _data_struct_control control_data;  //下发控制数据结构






void WIFI_Decode(void);
void master_cb_init(appRecvHandler_Cb cb);
void Master_App_RecvHandler(uint16_t conn_handle,uint16_t command, uint8_t* data,uint16_t len,_e_machine_model device_type,uint8_t mac_addr[6]);

void master_send_string(uint8_t *p_data,uint8_t len);




#endif




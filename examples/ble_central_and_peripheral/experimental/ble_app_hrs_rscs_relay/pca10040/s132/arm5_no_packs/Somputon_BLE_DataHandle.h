#ifndef __SOMPUTON_BLE_HANDLE_H
#define __SOMPUTON_BLE_HANDLE_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"

#include "Protocol_Analysis.h"
#include "User_MacList.h"

#include "slave_device_data.h"


void get_real_time_data_cmd(uint16_t conn_handle,_e_machine_model device_type,uint8_t mac_addr[6],uint8_t* data,uint16_t len);
void bond_cmd(uint16_t conn_handle,_e_machine_model device_type,uint8_t mac_addr[6]);
void clear_history_data_cmd(void);
void set_device_time_cmd(uint8_t *data_info);
void get_device_time_cmd(uint32_t second);
void control_device_cmd(uint8_t *ctrl_data);
void Clife_GetHisData(uint16_t cmd, uint8_t* data, uint16_t len);
void Clife_GenerateHisData(void);



void bond_data_send(uint8_t conn_handle);
void real_data_send(uint8_t conn_handle);

void control_data_send(uint8_t conn_handle,uint8_t device_type,uint8_t *data ,
    uint8_t length,_data_struct_control *control_data);
void send_real_time(uint8_t conn_handle);



#endif


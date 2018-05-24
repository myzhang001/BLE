#ifndef __SOMPUTON_BLE_HANDLE_H
#define __SOMPUTON_BLE_HANDLE_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"

#include "Protocol_Analysis.h"
#include "User_MacList.h"







void get_real_time_data_cmd(void);
void bond_cmd(uint16_t conn_handle,_e_machine_model device_type,uint8_t mac_addr[6]);
void clear_history_data_cmd(void);
void set_device_time_cmd(uint8_t *data_info);
void get_device_time_cmd(uint32_t second);
void control_device_cmd(uint8_t *ctrl_data);
void Clife_GetHisData(uint16_t cmd, uint8_t* data, uint16_t len);
void Clife_GenerateHisData(void);




#endif


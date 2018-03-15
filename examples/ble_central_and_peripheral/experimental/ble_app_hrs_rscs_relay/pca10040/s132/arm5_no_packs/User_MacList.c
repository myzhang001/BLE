#include "User_MacList.h"

#define  MAX_LIST_LENGTH  8      //列表最大个数
#define  MAC_ADDR_LENGTH  6      //MAC 地址长度




typedef struct{
	uint8_t 	mac_addr[MAC_ADDR_LENGTH];        	//设备mac 地址
	uint8_t 	conn_handle;       					//连接句柄
	uint8_t 	connected_flag;   					//连接状态
	int     	rssi;             	  				//rssi 信号强度
	_e_machine_model model;             			//设备型号
}_t_ble_status;


typedef struct{
	_t_ble_status ble_dev[MAX_LIST_LENGTH];
	

}_t_dev_mac_match;









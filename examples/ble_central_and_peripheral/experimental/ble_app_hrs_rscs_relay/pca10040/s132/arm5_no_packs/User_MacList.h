#ifndef __maclist_h
#define __maclist_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"
#include "ble_db_discovery.h"

#define  MAX_LIST_LENGTH  8      //列表最大个数
#define  MAC_ADDR_LENGTH  6      //MAC 地址长度

typedef enum{
	E_RAR312 = 0X00,
	E_RM661 = 0X01,
	E_SLEEP_MONITER = 0X02,
	E_CURTAIN = 0X03,
	E_06H = 0X04,
	E_PMI = 0X05,
	E_08A = 0X06,
	E_08F = 0X07,
	E_09A = 0X08,
	E_09F = 0X0A,
	E_INVALID = 0X0B
}_e_machine_model;



typedef enum{
    E_NOBOND = 0X00,                       //为绑定状态
    E_BONDING = 0X01,                      //正在绑定
    E_BONDING_COMPLT = 0X02                //绑定完成
}_e_bond_status;




//单个设备信息
typedef struct{
	uint8_t 	mac_addr[MAC_ADDR_LENGTH];        	//设备mac 地址
	uint16_t 	conn_handle;       					//连接句柄
	uint8_t 	connected_flag;   					//连接状态
	int8_t     	rssi;             	  				//rssi 信号强度
	_e_machine_model model;             			//设备型号
    uint8_t     hard_version;                       //硬件版本,包含硬件芯片型号和硬件版本号
    uint8_t     software_version;                   //软件版本号
	uint8_t     device_datastruct_index;            //对应设备数据结构索引
	_e_bond_status     bond_stauts;                       //绑定状态
	
}_t_ble_status;


typedef enum{
    E_BIND_NONE = 0X00,     //未绑定状态
    E_BIND_ING  = 0X01,     //正在绑定
    E_BIND_CMPT = 0X02      //绑定完成
}_e_band_status;



typedef struct{
	_t_ble_status   ble_dev[MAX_LIST_LENGTH];  //设备基本信息
    uint8_t         device_num;                //设备连接总数
	_e_band_status  scan_bind_flag;            //允许扫描标志位
    uint8_t         bing_timeout_cnt;          //绑定超时 
    
}_t_dev_mac_match;




typedef enum{
    E_SEND_STATUS = 0x00,
    E_REAL_TIME_DATA = 0X01,
    E_CONTROL_DATA   = 0X02
}_data_step;



//根据设备类型查找设备信息  ,设备个数  设备句柄  设备 conn_handle

typedef struct{
    uint16_t conn_handle;
    _e_machine_model device_type;
}_s_avaiable_info;


typedef struct{
    uint8_t  total_avaiable_num;      //可连接设备总个数
    _s_avaiable_info dev_info[8];
}_s_avaiable_device;





typedef struct{
    uint8_t mac_addr[6];         //mac 地址
    _e_machine_model device_type;
}_bond_device;



//保存已绑定设备信息

typedef struct{
     uint8_t       device_num;              //设备总数
    _bond_device   bond_device[8];          //绑定设备信息
}_bond_device_info;





#define  BIND_TIMEOUT_CNT           60      //绑定超时时间

//复位超时时间
#define Reset_Timeout_Cnt do{\
dev_info.scan_bind_flag = 0;\
dev_info.bing_timeout_cnt = BIND_TIMEOUT_CNT;\
}while(0)



extern  _t_dev_mac_match     dev_info;                 //连接设备的所有信息
extern  _s_avaiable_device   sys_avaiable_table;   //可用设备列表全局

extern _bond_device_info     bond_device_info;        //绑定设备信息



bool dev_check_empty(void);
void Device_Info_Reset(_t_ble_status *s_ble_info);
void  Debug_Device_match_info(uint16_t  connected_handle,uint8_t *addr_mac);
void Device_Disconnected_handle(uint16_t  connected_handle);
void Debug_Device_match_connected_mac(ble_gap_addr_t mac_peer_addr,uint16_t handle);
void USER_DEBUG_printf(void);
_e_band_status Ret_Device_Bind_status(void);
uint8_t Ret_Device_Bind_Time(void);

void Device_Update_Avaiable_Table(void);

void data_send_proc(void);

void  device_bond_status_update(uint8_t conn_handle);

void  device_bond_status_clear(uint8_t conn_handle);
void device_add_type(uint8_t conn_handle ,_e_machine_model device_type);
void device_del_type(uint8_t conn_handle );

void device_total_add(void);
void device_total_del(void);

void printf_all_dev_info(void);
void sys_avaiable_device_type(void);

uint8_t bond_info_del(_bond_device_info *bond_info , uint8_t mac_addr[6],_e_machine_model device_type);


void connected_bond_list_device(_bond_device_info *bond_info,
    ble_gap_evt_adv_report_t *adv_report);


#endif


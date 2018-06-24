#include "Somputon_BLE_DataHandle.h"

#include "Master_DataStruct.h"
#include "Nus_Master_DataHandle.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "ble_nus_c.h"




extern void send_string_c(uint8_t conn_handle, uint8_t * p_string, uint16_t length);








/**@brief get_real_time_data_cmd     
 *
 *	Function for replying to get real time data.
 *
 * @param[in]   void  
 *
 * @return      void
 */
void get_real_time_data_cmd(uint16_t conn_handle,_e_machine_model device_type,uint8_t mac_addr[6],uint8_t* data,uint16_t len)
{
	uint16_t handle = 0;
	
	
	switch(device_type)
	{
		case E_01G:
            //接收rm661 的实时数据    

			 handle = mac_match_hanle(&rm661_data.mac_index,mac_addr);
			
			 NRF_LOG_INFO("----------01G handle %d",handle);

	         if(handle > 8)return;		
		
			 memcpy(&rm661_data.Device_rm661_Array[handle].real_light,data,len);

			 NRF_LOG_INFO("----------data 0x%02x",rm661_data.Device_rm661_Array[handle].real_light);
		
			break;
		case E_01H:
			break;
		case E_03F:
			break;
		case E_03H:
			break;
		case E_06H:
			break;
		case E_06F:
			break;
		case E_08A:
			break;
		case E_08F:

			break;
		case E_09A:
            //接收实时数据  
            //memcpy(&System_08F.Device_08F_Array[conn_handle].Device_08F_POWER,data,len); 
		
			handle = mac_match_hanle(&System_08F.mac_index,mac_addr);
		
			NRF_LOG_INFO("----------E_09A handle %d",handle);
			if(handle >8)return;	
		
			memcpy(&System_08F.Device_08F_Array[handle].Device_08F_POWER,data,len);	
		
			NRF_LOG_INFO("----------data 0x%02x",System_08F.Device_08F_Array[handle].Device_08F_POWER);
			break;
		case E_09F:
			break;
		 
		 default:
			 break;
	}
        
    
    
}


/**@brief clear_history_data_cmd     
 *
 *	Function for replying to the clear history data command from the APP and wouldn't contain any data.
 *
 * @param[in]   void   Point to the time struct data.
 *
 * @return      void
 */
void bond_cmd(uint16_t conn_handle,_e_machine_model device_type,uint8_t mac_addr[6])
{
	//dev_info.ble_dev[conn_handle - 1].device_datastruct_index
	
	switch(device_type)
	{
		case E_01G:
            Add_Device_List(&rm661_data.mac_index,mac_addr);    //为设备添加匹配关系
        
			break;
		case E_01H:
			break;
		case E_03F:
			break;
		case E_03H:
			break;
		case E_06H:
			break;
		case E_06F:
			break;
		case E_08A:
			break;
		case E_08F:
		
			break;
		case E_09A:
              Add_Device_List(&System_08F.mac_index,mac_addr);    //为设备添加匹配关系
        
			break;
		case E_09F:
			break;
		 
		 default:
			 break;
	}
}

/**@brief clear_history_data_cmd     
 *
 *	Function for replying to the clear history data command from the APP and wouldn't contain any data.
 *
 * @param[in]   void   Point to the time struct data.
 *
 * @return      void
 */

void clear_history_data_cmd(void)
{


}


/**@brief set_device_time_cmd     
 *
 *	Function for setting time.
 *
 * @param[in]   data_info   Point to the time struct data.
 *
 * @return      void
 */
void set_device_time_cmd(uint8_t *data_info)
{



}


/**@brief get_device_time_cmd     
 *
 *	Function for getting time.
 *
 * @param[in]   second   use second to stand for the current time.
 *
 * @return      void
 */
void get_device_time_cmd(uint32_t second)
{


}


/**@brief control_device_cmd     
 *
 *	Function for dealing with control command.
 *
 * @param[in]   ctrl_data   clife control word data
 *
 * @return      void
 */
void control_device_cmd(uint8_t *ctrl_data)
{




}


/**@brief Clife_GetHisData     
 *
 *	Function for dealing with the process of getting history data command.
 *
 * @param[in]   cmd   clife command word 
 * @param[in]   data  clife command word data
 * @param[in]   len   data length
 *
 * @return      void
 */
void Clife_GetHisData(uint16_t cmd, uint8_t* data, uint16_t len)
{
	
	
}


/**@brief Clife_GenerateHisData     
 *
 *	Function for Generating history data to write to the flash.
 *
 * @param[in]   void
 *
 * @return      void
 */
void Clife_GenerateHisData(void)
{



}


void bond_data_send(uint8_t conn_handle)
{
    uint8_t  send_buffer[50];
    
    
    send_buffer[0] = START_FLAG;                                //数据头
    send_buffer[1] = 0x00;                                      //数据长度
    send_buffer[2] = BOND_CMD_INDEX;
    send_buffer[3] = PROTOCOL_VERSION;                          //协议版本号
    send_buffer[4] = 0x00;                                      //设备类型 
    memset(&send_buffer[5],0,6);     //获取mac 地址
  

    send_buffer[11] =  (uint8_t)(BOND_COMMAND>>8);              //命令控制字
    send_buffer[12] =  (uint8_t)BOND_COMMAND;                   //命令控制字

    send_buffer[13] = Crc8(&send_buffer[1],BOND_CMD_INDEX + 1);                 //crc 校验

    send_string_c(conn_handle,send_buffer,BOND_CMD_INDEX_TOTAL);
}

//获取设备状态数据

void real_data_send(uint8_t conn_handle)
{
    uint8_t  send_buffer[50];
    
    
    send_buffer[0] = START_FLAG;                                //数据头
    send_buffer[1] = 0x00;                                      //数据长度
    send_buffer[2] = REAL_TIME_INDEX;
    send_buffer[3] = PROTOCOL_VERSION;                          //协议版本号
    send_buffer[4] = 0x00;                                      //设备类型 
    memset(&send_buffer[5],0,6);                                //获取mac 地址
  

    send_buffer[11] =  (uint8_t)(GET_REAL_TIME_DATA_COMMAND >> 8);              //命令控制字
    send_buffer[12] =  (uint8_t)GET_REAL_TIME_DATA_COMMAND;                   //命令控制字

    send_buffer[13] = Crc8(&send_buffer[1],REAL_TIME_INDEX + 1);                 //crc 校验

    send_string_c(conn_handle,send_buffer,REAL_TIME_INDEX_TOTAL);
}


void control_data_send(uint8_t conn_handle,uint8_t device_type,uint8_t *data ,uint8_t length)
{
    uint8_t  send_buffer[50];
    
    
    send_buffer[0] = START_FLAG;                                //数据头

    send_buffer[1] = 0x00;                                      //数据长度
    send_buffer[2] = REAL_TIME_INDEX;
    
    
    send_buffer[3] = PROTOCOL_VERSION;                          //协议版本号
    send_buffer[4] = 0x00;                                      //设备类型 
    //memcpy(&send_buffer[5],,6);                               //获取mac 地址


    send_buffer[11] =  (uint8_t)(GET_REAL_TIME_DATA_COMMAND >> 8);    //命令控制字
    send_buffer[12] =  (uint8_t)GET_REAL_TIME_DATA_COMMAND;           //命令控制字

    send_buffer[13] = Crc8(&send_buffer[1],REAL_TIME_INDEX);          //crc 校验

    
    switch(device_type)
    {
        case 0:
            
        
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        default:
            break;
    }
    
    
    send_string_c(conn_handle,send_buffer,BOND_CMD_INDEX_TOTAL);

}



#include "Somputon_BLE_DataHandle.h"

#include "Master_DataStruct.h"
#include "Nus_Master_DataHandle.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"



/**@brief get_real_time_data_cmd     
 *
 *	Function for replying to get real time data.
 *
 * @param[in]   void  
 *
 * @return      void
 */
void get_real_time_data_cmd(void)
{
	
        NRF_LOG_INFO("REALTIME");
    
    
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
			
			Add_Device_List(&System_08F.mac_index,mac_addr);    //为设备添加匹配关系
		
			break;
		case E_09A:
			
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

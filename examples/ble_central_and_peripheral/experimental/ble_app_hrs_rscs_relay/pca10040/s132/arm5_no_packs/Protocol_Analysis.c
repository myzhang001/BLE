#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"
#include "User_MacList.h"



appRecvHandler_Cb  receive_data_from_app;   //���ݽ��ջص�



void regRevAppDataCb(appRecvHandler_Cb cb)
{
		receive_data_from_app = cb;
}



/**@brief Somputon_Init
 * Register a Somputon command word callback function for application
 
 * @param[in] cb Function will be called when data have been received complete from BLE.
 */
void Somputon_Init(appRecvHandler_Cb cb)
{
	regRevAppDataCb(cb);	
}






/**@brief     Function for handling clife command word in the application
 *
 * @param[in] command 			clife command word
 *         	  src_data    	    source data
 *         	  len 			    data length
 *
 * @return    void
 */
void App_RecvHandler(uint16_t conn_handle,uint16_t command, uint8_t* data,uint16_t len,_e_machine_model device_type,uint8_t mac_addr[6])
{
		switch(command)
		{			
			case ENCRYPTION_REQ_REPLY:
					control_device_cmd(NULL);			      												
					break;
			case CLEAR_HISTORY_DATA_COMMAND:
					 clear_history_data_cmd();                //�����ʷ����
					 break;			
			
			case GET_REAL_TIME_DATA_COMMAND:					 
					 get_real_time_data_cmd();	              //��ȡʵʱʱ��
					 break;		
			
			case BOND_COMMAND:
					bond_cmd(conn_handle,device_type,mac_addr);                              //���Ͱ�ָ��
					 break;		
												
			case GET_HISTORY_DATA_TOTAL_PACKETS:
			case GET_HISTORY_DATA_COMMAND:
			case HISTORY_DATA_REPLY_FROM_APP:	
					 Clife_GetHisData(command,data,len);
					 break;
			
			case SET_DEVICE_TIME_COMMAND:			
					 set_device_time_cmd(data);				//  ��ͬ��APP��ʱ��					 
					 break;		
			
			case GET_DEVICE_TIME_COMMAND:						 
					  get_device_time_cmd(1);			    //  �ϴ�ʱ��
					 break;
			

			case CONTROL_DATA_COMMAND:
					 control_device_cmd(data);				//�����������	   	
					break;			
															
			default:				
					break;
		}		
			
}






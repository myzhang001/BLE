#include "Nus_Master_DataHandle.h"

#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


typedef struct{

	uint16_t  Data_Length;    //�������ݳ���
	uint16_t Common_World;    //����������
	uint8_t  Device_Type;    //�豸����
	uint8_t  MacAddr_Device;	//�豸mac ��ַ
}_s_analsis_word;


_s_analsis_word  Common_Word;




//crc8 У��
unsigned char Crc8( unsigned char *Address, unsigned char Length )
{
		static unsigned char crc;
		static unsigned char crcbuff;
		static unsigned char i;
		crc = 0;
	
		while( Length-- )
		{
			crcbuff = *Address ++;
			for(i = 0; i < 8; i++)
			{
				if((crc ^ crcbuff) & 0x01)
				{
					crc ^= 0x18;
					crc >>= 1;
					crc |= 0x80;
				}
				else
				{
					crc >>= 1;
				}
				crcbuff >>= 1;
			}
		}
		return crc;
}



//������Чhandle
bool NUS_C_Filter_Connected_Handle(uint32_t handle)
{
	if(handle == 0 ||handle == 65535)
	{
		return false;
	}
	else
	{
		return true;
	}
}



//����������
void nus_data_handle(uint32_t nus_c_conn_handle, uint8_t *data, uint8_t length)
{
	
	uint8_t data_buffer[100];       	//�������ݻ�������С
	
	memcpy(data_buffer,data,length);
	memset(data,0,length);
	
    for(uint8_t i = 0; i< length;i++)
    {
        NRF_LOG_INFO("DATA 0x%02x",data_buffer[i]);
    }
    

	if(data_buffer[0]!= START_FLAG)	
	{
		return;
	}
	else
	{
		
		Common_Word.Data_Length  = data_buffer[DATA_LENGTH_INDIX_LOW];    //��ȡ���ݰ�����
		Common_Word.Data_Length = Common_Word.Data_Length>>8;
		Common_Word.Data_Length |= data_buffer[DATA_LENGTH_INDIX_HIGH];
		
        
        NRF_LOG_INFO("DATA LENGHT : %d",Common_Word.Data_Length);
        
		Common_Word.Common_World = data_buffer[DATA_COMMOND_WORD ];	   //��ȡ������
		Common_Word.Common_World = Common_Word.Common_World << 8;
		Common_Word.Common_World |= data_buffer[DATA_COMMOND_WORD +1 ];        
		
        NRF_LOG_INFO("COMMAND WORLD:  0X%04x",Common_Word.Common_World);
        
        
		
		Common_Word.Device_Type = data_buffer[DATA_DEVICE_TYPE_INDEX];     //��ȡ�豸����
		
	    memcpy(&Common_Word.MacAddr_Device,&data_buffer[DATA_DEVICE_MAC_INDEX],6);  //����mac ��ַ		 	  
			
		
		if(data_buffer[Common_Word.Data_Length ]!= Crc8(&data_buffer[1],Common_Word.Data_Length -1))  //crc8У��
		{
			//#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc 0x%02x", data_buffer[Common_Word.Data_Length ]);
			
			NRF_LOG_INFO("CRC ERROR");
			//#endif
			return;
		}
		//������������
		
		if(receive_data_from_app != NULL)
		{
			receive_data_from_app((uint16_t)nus_c_conn_handle,Common_Word.Common_World,&data_buffer[DATA_CONTENT_INDEX],Common_Word.Data_Length - 4,Common_Word.Device_Type,&Common_Word.MacAddr_Device);
		}
		
		
		
	}
}



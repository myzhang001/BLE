#include "Nus_Master_DataHandle.h"

#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"





uint8_t  Data_Length  = 0;    //�������ݳ���
uint16_t Common_World = 0;   //����������




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
	

	if(data_buffer[0]!= START_FLAG)	
	{
		return;
	}
	else
	{
		
		Data_Length  = data_buffer[DATA_LENGTH_INDIX_HIGH];    //��ȡ���ݰ�����
		Data_Length = Data_Length>>8;
		Data_Length |= data_buffer[DATA_LENGTH_INDIX_LOW];
		
		Common_World = data_buffer[DATA_COMMOND_WORD + 1];	   //��ȡ������
		Common_World = Common_World >> 8;
		Common_World |= data_buffer[DATA_COMMOND_WORD];        
		
		
		if(data_buffer[Data_Length -1 ]!= Crc8(&data_buffer[1],data_buffer[Data_Length -2]))  //crc8У��
		{
			#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc %d", data_buffer[Data_Length -1]);
			
			NRF_LOG_INFO("CRC ERROR ERROR");
			#endif
			return;
		}
		//������������
		
		
		
		
		
	}
}



#include "Nus_Master_DataHandle.h"

#include "Protocol_Analysis.h"
#include "Somputon_BLE_DataHandle.h"





uint8_t  Data_Length  = 0;    //保存数据长度
uint16_t Common_World = 0;   //保存命令字




//crc8 校验
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



//过滤无效handle
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



//处理串口数据
void nus_data_handle(uint32_t nus_c_conn_handle, uint8_t *data, uint8_t length)
{
	
	uint8_t data_buffer[100];       	//接收数据缓冲区大小
	
	memcpy(data_buffer,data,length);
	memset(data,0,length);
	

	if(data_buffer[0]!= START_FLAG)	
	{
		return;
	}
	else
	{
		
		Data_Length  = data_buffer[DATA_LENGTH_INDIX_HIGH];    //获取数据包长度
		Data_Length = Data_Length>>8;
		Data_Length |= data_buffer[DATA_LENGTH_INDIX_LOW];
		
		Common_World = data_buffer[DATA_COMMOND_WORD + 1];	   //获取命令字
		Common_World = Common_World >> 8;
		Common_World |= data_buffer[DATA_COMMOND_WORD];        
		
		
		if(data_buffer[Data_Length -1 ]!= Crc8(&data_buffer[1],data_buffer[Data_Length -2]))  //crc8校验
		{
			#ifdef UART_MASTER_TEST 
			NRF_LOG_INFO(" data_buffer crc %d", data_buffer[Data_Length -1]);
			
			NRF_LOG_INFO("CRC ERROR ERROR");
			#endif
			return;
		}
		//处理数据内容
		
		
		
		
		
	}
}



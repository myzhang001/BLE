#include "uart_queue.h"
#include "app_uart.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "queue.h"
//#include "WIFI_Decode.h"



STRUSART2 gUsart2Send;
STRUSART2 gUsart2Recv;


static  uint8_t s_buffer;
static  uint8_t buffer[50];
uint8_t indexbuffer;     



void USART2_init(void)
{
  	//Standard_SetMemory((BYTE *)&gUsart2Send,0x00,(sizeof(gUsart2Send)/sizeof( BYTE)));
	//Standard_SetMemory((BYTE *)&gUsart2Recv,0x00,(sizeof(gUsart2Recv)/sizeof( BYTE)));
}


uint8_t USART2_Transmit(uint8_t *buff, uint8_t len)
{
	uint8_t i;
	
	if(len==0) return 0;

	for( i=0;i<len;i++)
	{
		gUsart2Send.buff[gUsart2Send.uIn++] = buff[i];
	}
	
	if(!gUsart2Send.enable)
	{
		gUsart2Send.enable = 1;
	}
	return 1;
}

static uint8_t sendheadbuff[30]={0};


uint16_t WIFI_Send_Queue_length(void)
{
    return (256 + gUsart2Send.uIn - gUsart2Send.uOut) \
            % 256;
}


uint8_t SendString(uint8_t *buff,uint8_t length)
{
	uint32_t err_code;
	
	for (uint32_t i = 0; i < length; i++)
	{
		do
		{
			err_code = app_uart_put(buff[i]);
			if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
			{
				//NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
				APP_ERROR_CHECK(err_code);
			}
		} while(err_code == NRF_ERROR_BUSY);
	}
	


	return 1;
}	


uint8_t SendBuff[256];



/*串口发送函数*/
uint8_t USART2_SendProc(void)	
{
	uint8_t ch;

	if(gUsart2Send.uIn!=gUsart2Send.uOut)
	{
		ch = gUsart2Send.buff[gUsart2Send.uOut++];
		app_uart_put(ch);
	}
	else
	{
		gUsart2Send.enable = 0;
	}	
}



uint8_t UART2_Read_Queue(uint8_t *aByte)
{
	uint8_t found=0;

	if(gUsart2Recv.uOut!=gUsart2Recv.uIn)
	{
		*aByte = gUsart2Recv.buff[gUsart2Recv.uOut++];

		found=1;
	}
	return found;
}


void USART2_Write_Queue(uint8_t ch)
{
	gUsart2Recv.buff[gUsart2Recv.uIn++]=ch;

	if(gUsart2Recv.uIn==gUsart2Recv.uOut)
	{
		gUsart2Recv.uIn--;
	}
}








//发送串口数据  demo
void RealtimeSendData(void)
{
	if(UART2_Read_Queue(&s_buffer) == 1)      
	{
		buffer[indexbuffer++]= s_buffer;
		if(buffer[indexbuffer-1] == '\n')
		{
			USART2_Transmit(buffer,indexbuffer);
			indexbuffer = 0;
		}
	}
}









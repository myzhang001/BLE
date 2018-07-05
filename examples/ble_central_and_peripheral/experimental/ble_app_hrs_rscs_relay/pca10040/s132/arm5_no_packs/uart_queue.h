#ifndef __uart_queue_h
#define __uart_queue_h


#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"

#define WriteBLEQueue			USART2_Transmit
#define ReadBLEQueue			UART2_Read_Queue

#define USART2_BUFF		        0x100	



typedef struct
{
	uint16_t len;
	uint8_t uIn;
	uint8_t uOut;
	uint8_t enable;
	uint8_t buff[USART2_BUFF];

	uint8_t point;
}STRUSART2;



#define TIMEOUT_N_S(x) (x*1000)


#define RESET_MODE()  do{\
wifi_globas.g_status  = WIFI_MCU_UNLINK_STATE;\
}while(0)



//进入绑定模式
#define ENTER_BOUND_MODE() do{\
	if(wifi_globas.link_status.ModuleLinkF == 0){\
		wifi_globas.link_status.ModuleLinkF = 1;\
	}\
}while(0)







void USART2_init(void);
uint8_t USART2_Transmit( uint8_t *buff, uint8_t len );
uint8_t UART2_Read_Queue(uint8_t *aByte);
void USART2_Write_Queue(uint8_t ch);

void RealtimeSendData(void);
uint8_t USART2_SendProc(void);

#endif


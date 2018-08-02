#include "timer.h"
#include "common_include.h"



_systime  SysTimer;     //系统时钟标准
_timer    Timer;        //时钟计时





static void SystimerTick(void)
{
	if(Tick10msFlag == true)
	{
		Tick10msFlag = false;
	
		SysTimer.s_timer10ms_flag = true;
		
		Timer.Num10ms++;
		if(Timer.Num10ms > 1)
		{
			Timer.Num10ms = 0;
			SysTimer.s_timer20ms_flag = true;
			Timer.Num20ms++;
			if(Timer.Num20ms > 4)
			{
				Timer.Num20ms = 0;
				SysTimer.s_timer100ms_flag = true;
				Timer.Num100ms++;
				if(Timer.Num100ms > 9)
				{
					Timer.Num100ms = 0;
					
					SysTimer.s_timer1s_flag = true;
					
					Timer.Num1s++;
					
					if(Timer.Num1s > 59)
					{
						Timer.Num1s = 0;
						SysTimer.s_timer1min_flag = true;
						
						
						Timer.Num1min++;
						if(Timer.Num1min > 59)
						{
							Timer.Num1min = 0;
							
							SysTimer.s_timer1hour_flag = true;
						}
						
					}
				}
			
			}
			
		}

	}

}



/*
系统10ms 处理
*/
static void Syshand10ms(void)
{
	
    
} 


static void Syshand20ms(void)
{
	
	
	
}

static void Syshand100ms(void)
{
	data_send_proc();                           //设备通信数据流程
}


static void Syshand1s(void)
{
	//NRF_LOG_INFO("hello world");
}

static void Syshand1min(void)
{

	
}


void SysRunningHandler(void)
{
	if(SysTimer.s_timer10ms_flag == true)
	{
		SysTimer.s_timer10ms_flag = false;
		Syshand10ms();
	}

	if(SysTimer.s_timer20ms_flag == true)
	{
		SysTimer.s_timer20ms_flag = false;
		Syshand20ms();
	}
	
	if(SysTimer.s_timer100ms_flag == true)
	{
		SysTimer.s_timer100ms_flag = false;
		Syshand100ms();
	}

	if(SysTimer.s_timer1s_flag == true)
	{
		SysTimer.s_timer1s_flag = false;
		Syshand1s();
	}
	
	if(SysTimer.s_timer1min_flag == true)
	{
		SysTimer.s_timer1min_flag = false;
		Syshand1min();
	}
}




//整个系统systick 处理
void sys_tick_handle(void)
{
    SystimerTick();
    SysRunningHandler();
}



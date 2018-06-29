#ifndef __MASTER_DATASTRUCT_H
#define __MASTER_DATASTRUCT_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nordic_common.h"





#define MAX_DEVICE_NUM  8							//��������豸����
#define MAC_LEN         6                           //MAC ��ַ����


//08F ���ݽṹ

//08F �����������ݽṹ

typedef enum{
    E_MOTOR_OFF 	= 0X00,
	E_MOTOR_LOW 	= 0X01,
	E_MOTOR_MIDDLE  = 0X02,
	E_MOTOR_HIGH    = 0X03
}_e_08f_motor;


//�豸״̬����
typedef struct{
	uint8_t 		Device_08F_POWER;       //���ػ�
}_real_staus_08F;



//�豸��������

typedef struct{
	uint8_t 		Device_08F_POWER;       //���ػ�
	_e_08f_motor    Motor_Level;            //�����λ

}_control_08F;




//�·����ݽṹ
typedef struct{
	uint8_t      Device_09_Power;          //���ػ�
	uint8_t      Motor_Level;              //��ﵵλ	
}_contorl_09;


typedef struct{
	uint8_t      Device_09_Power;          //���ػ�
	uint8_t      Motor_Level;              //��ﵵλ
	uint8_t      Work_Time;                //�豸����ʱ��
}_real_status_09;





//mac ��ַ �� Array ����ƥ��
typedef struct{

	uint8_t mac_addr[6];             //����mac ��ַ
	uint8_t index;                   //����
	
}_mac_index_fix;



typedef struct{
	uint8_t         device_num; 					  //�豸����
	uint8_t         max_index_decice;                 //�������
	uint8_t         index_sort[MAX_DEVICE_NUM];       //�����������
	_mac_index_fix  mac_list_array[MAX_DEVICE_NUM];   //�豸mac list ��index �б�
}_mac_index_data;



typedef struct{
	uint8_t         Device_Num;                     			//���豸����
	_real_staus_08F Device_08F_Array[MAX_DEVICE_NUM];    		//08 ʵʱ����
	_control_08F 	Device_Control_08F_Array[MAX_DEVICE_NUM];   //08 ��������

	_mac_index_data mac_index; 									//mac list index				

}_datastruct_08F;


typedef struct{
	uint8_t         Device_Num;                     			//���豸����
	_real_status_09 Device_09_Array[MAX_DEVICE_NUM];			//09 ʵʱ����
	_contorl_09    	Device_Control_09_Array[MAX_DEVICE_NUM];	//09 ��������
}_datastruct_09;



//-------------------------< start >-------------------------------
//---------------------rm661 ���ݽṹ




typedef struct{
	uint8_t      control_light;              //���ص�
	uint8_t      control_aroma;              //������޹	
}_contorl_rm661;


typedef struct{
	uint8_t      real_light;                 //���ص�
    uint8_t      real_aroma;                 //��޹������
	                
}_real_status_rm661;



typedef struct{
	uint8_t             Device_Num;                     			//���豸����
	_real_status_rm661  Device_rm661_Array[MAX_DEVICE_NUM];			//661 ʵʱ����
	_contorl_rm661    	Device_Control_rm661_Array[MAX_DEVICE_NUM];	//661 ��������
    _mac_index_data     mac_index; 									//mac list index	
    
}_datastruct_rm661;

//-------------------------< end >-------------------------------

//-------------------------< start >-------------------------------
//RAR 312 ���ݽṹ

typedef struct{
	int16_t      real_temp;                 //�¶������ֽ�
    uint8_t      real_humid;                //ʪ��һ���ֽ�
	                
}_real_status_rar312;


typedef struct{
    uint8_t      control_power;                //���ػ���־
    
}_contorl_rar312;



typedef struct{
    uint8_t              Device_Num;                     			//���豸����
    _real_status_rar312  Device_rar312_Array[MAX_DEVICE_NUM];			//661 ʵʱ����
    _contorl_rar312    	 Device_Control_rar312_Array[MAX_DEVICE_NUM];	//661 ��������
    _mac_index_data      mac_index; 	
}_datastruct_rar312;

//-------------------------< end >-------------------------------



//�������ݽṹ

typedef struct{
    uint8_t  real_run;                //ת����־
	uint8_t  reserve_data[2];         //�����ֽ�        
}_real_status_curtain;


typedef struct{
    uint8_t        control_power;                //���ػ���־
    uint8_t        reserve_data[2];
}_contorl_curtain;



typedef struct{
    uint8_t               Device_Num;                     			//���豸����
    _real_status_curtain  Device_curtain_Array[MAX_DEVICE_NUM];			//661 ʵʱ����
    _contorl_curtain      Device_Control_curtain_Array[MAX_DEVICE_NUM];	//661 ��������
    _mac_index_data       mac_index; 	
}_datastruct_curtain;

//-------------------------< end >-------------------------------




//-------------------------< start >-------------------------------

//-------------------------------------------pmi
typedef struct{
	uint8_t      control_light;              //���ص�
	uint8_t      control_aroma;              //������޹	
}_contorl_pmi;


typedef struct{
	uint8_t      real_data[12];                 //���ص�
    
	                
}_real_status_pmi;



typedef struct{
	uint8_t             Device_Num;                     			//���豸����
	_real_status_pmi    Device_pmi_Array[MAX_DEVICE_NUM];			//661 ʵʱ����
	_contorl_pmi    	Device_Control_pmi_Array[MAX_DEVICE_NUM];	//661 ��������
    _mac_index_data     mac_index; 									//mac list index	
    
}_datastruct_pmi;

//-------------------------< end >-------------------------------




uint8_t Del_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6]);
uint8_t Add_Device_List(_mac_index_data *device_list,uint8_t mac_addr[6]);


extern _datastruct_08F     System_08F;       //08F  ϵͳ����
extern _datastruct_rm661   rm661_data;       //661  ȫ�ֱ���
extern _datastruct_rar312  rar312_data;      //rar312_data �����豸
extern _datastruct_curtain curtain_data;     //rar312_data �����豸
extern _datastruct_pmi     pmi_data;         //pmi �����豸



extern void Debug_Device_Info(void);        //��ӡ������Ϣ

extern uint16_t  mac_match_hanle(_mac_index_data *device_list ,uint8_t mac[6]);
extern void disconnect_del_info(uint16_t conn_handle, uint8_t mac_addr[6]);

#endif






#include "Dynamic_distruct.h"


typedef struct node{
     uint8_t mac[6];
     uint16_t handle;
     uint8_t device_type;
     uint8_t bond_info;
    struct node *next;
    
}_dev_infO_test;



_dev_infO_test  dev_info_test;
 

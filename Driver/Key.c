#include "Key.h"
/*按键扫描进程*/
uint8_t Key_Read()
{
    unsigned char temp=0;//局部变量初始化为0
    if (!DL_GPIO_readPins(GPIO_SWITCH_PORT, GPIO_SWITCH_USER_SWITCH_1_PIN)){temp = 1;}
    if (!DL_GPIO_readPins(GPIO_SWITCH_PORT, GPIO_SWITCH_USER_SWITCH_2_PIN)){temp = 2;}
    if (!DL_GPIO_readPins(GPIO_SWITCH_PORT, GPIO_SWITCH_USER_SWITCH_3_PIN)){temp = 3;}
    return temp;
}
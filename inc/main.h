/*
wangguanfu@163.com
QQ:21578936
未经过本人许可禁止任何商业用途
http://shop37116146.taobao.com/
*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include "stm32f10x_it.h"
#include "fifo.h"
#include "Sensor.h"
#include "delay.h"


void SYSTEM_Init(void);

void RCC_Config(void);

void GPIO_Config(void);
void SysTick_Config(void);
void Delay(u32 nCount);
void Decrement_TimingDelay(void);
void Set_SELStatus(void);


#endif /* __MAIN_H */



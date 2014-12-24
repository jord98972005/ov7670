



/*
wangguanfu@163.com
QQ:21578936
未经过本人许可禁止任何商业用途
http://shop37116146.taobao.com/
*/



#ifndef _OV7670_H
#define _OV7670_H


#include "sccb.h"
#include "main.h"
#define OV7670_REG_NUM  167
#define OV7725_REG_NUM  72
#define Sensor_VSYNC_BIT    GPIO_Pin_3




  #define GPIO_VSYNC_CMOS                    GPIOC
  #define RCC_APB2Periph_GPIO_VSYNC_CMOS     RCC_APB2Periph_GPIOC
  #define GPIO_PIN_VSYNC_CMOS                GPIO_Pin_3
  #define EXTI_LINE_VSYNC_CMOS               EXTI_Line3
  #define GPIO_PORT_SOURCE_VSYNC_CMOS        GPIO_PortSourceGPIOC
  #define GPIO_PIN_SOURCE_VSYNC_CMOS         GPIO_PinSource3


/////////////////////////////////////////
void XCLK_init_ON(void);//STM32 mco功能开启 输出时钟给CMOS传感器

unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat);
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat);

unsigned char Sensor_init(void);
void Sensor_GPIO_Init(void);
void Sensor_EXTI_Config(void);
void Sensor_Interrupts_Config(void);

#endif /* _OV7670_H */




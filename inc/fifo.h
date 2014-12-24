
/*
wangguanfu@163.com
QQ:21578936
未经过本人许可禁止任何商业用途
http://shop37116146.taobao.com/
*/

#include "stm32f10x_lib.h"



#define FIFO_CS_BIT     GPIO_Pin_7  // 
#define FIFO_WRST_BIT   GPIO_Pin_8  // 
#define FIFO_RRST_BIT   GPIO_Pin_9 // 
#define FIFO_RD_BIT     GPIO_Pin_10  // 
#define FIFO_WE_BIT     GPIO_Pin_11  //

#define FIFO_CS_H()    GPIOF->BSRR =FIFO_CS_BIT
#define FIFO_CS_L()    GPIOF->BRR  =FIFO_CS_BIT

#define FIFO_WRST_H()  GPIOF->BSRR =FIFO_WRST_BIT
#define FIFO_WRST_L()  GPIOF->BRR  =FIFO_WRST_BIT

#define FIFO_RRST_H()  GPIOF->BSRR =FIFO_RRST_BIT
#define FIFO_RRST_L()  GPIOF->BRR  =FIFO_RRST_BIT

#define FIFO_RD_H()    GPIOF->BSRR =FIFO_RD_BIT
#define FIFO_RD_L()    GPIOF->BRR  =FIFO_RD_BIT

#define FIFO_WE_H()    GPIOF->BSRR =FIFO_WE_BIT
#define FIFO_WE_L()    GPIOF->BRR  =FIFO_WE_BIT

void  FIFO_Set_GPIO_Config(void);
void FIFO_GPIO_INPUT(void);
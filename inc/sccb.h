#ifndef _sccb_H
#define _sccb_H

#define SCCB_SIC_BIT     GPIO_Pin_2
#define SCCB_SID_BIT     GPIO_Pin_3

#define SCCB_SIC_H()     GPIOD->BSRR = SCCB_SIC_BIT;
#define SCCB_SIC_L()     GPIOD->BRR =  SCCB_SIC_BIT;

#define SCCB_SID_H()     GPIOD->BSRR = SCCB_SID_BIT;
#define SCCB_SID_L()     GPIOD->BRR =  SCCB_SID_BIT;
/**/
#define SCCB_SID_IN      SCCB_SID_GPIO_INPUT();
#define SCCB_SID_OUT     SCCB_SID_GPIO_OUTPUT();


#define SCCB_SID_STATE	 GPIOD->IDR&0x0008

///////////////////////////////////////////
void SCCB_GPIO_Config(void);
void SCCB_SID_GPIO_OUTPUT(void);
void SCCB_SID_GPIO_INPUT(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char SCCBwriteByte(unsigned char m_data);
unsigned char SCCBreadByte(void);


#endif /* _IIC_H */



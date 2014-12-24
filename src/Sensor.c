/*
wangguanfu@163.com
QQ:21578936
δ����������ɽ�ֹ�κ���ҵ��;
http://shop37116146.taobao.com/
*/

#include "Sensor.h"
#include "delay.h"
#include "Sensor_config.h"
#include "stm32f10x_lib.h"

extern const char OV7670_reg[OV7670_REG_NUM][2];
extern const char OV7725_reg[OV7725_REG_NUM][2];
////////////////////////////
//���ܣ��ṩʱ��
//guanfu_wang
void XCLK_init_ON(void)
{
  
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    RCC_MCOConfig(RCC_MCO_HSE  );//hsi
}
/////////////////////////////
//VSYNC GPIO INIT
//guanfu_wang
void Sensor_GPIO_Init(void)//
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
   // Enable GPIOC  clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  Sensor_VSYNC_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;////���� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
}

//////////////////////////////////////////////////
////�������ܣ��ж�����
//guanfu_wang

void Sensor_Interrupts_Config(void)//VSYNC �ж�
{
  NVIC_InitTypeDef NVIC_InitStructure;

  


  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  

    /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the EXTI9_5 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  

  

}
////////////////////////////////////////////////////////////////////////
void Sensor_EXTI_Config(void)//VSYNC �ж�
{
  EXTI_InitTypeDef EXTI_InitStructure;
  

  GPIO_EXTILineConfig(GPIO_PORT_SOURCE_VSYNC_CMOS, GPIO_PIN_SOURCE_VSYNC_CMOS);
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC_CMOS;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_LINE_VSYNC_CMOS);

}


///////////////////////////////////////////////////////////////////////////
//���ܣ�дOV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
//guanfu_wang
unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat)
{
		startSCCB();//����SCCB ���߿�ʼ��������
	if(0==SCCBwriteByte(0x42))//д��ַ
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))//������ID
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regDat))//д���ݵ�������
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
  	stopSCCB();//����SCCB ����ֹͣ��������
	
  	return(1);//�ɹ�����
}
////////////////////////////
//���ܣ���OV7660�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
//guanfu_wang
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat)
{
	//ͨ��д�������üĴ�����ַ
	startSCCB();
	if(0==SCCBwriteByte(0x42))//д��ַ
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))//������ID
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
	stopSCCB();//����SCCB ����ֹͣ��������
	
	delay_us(100);
	
	//���üĴ�����ַ�󣬲��Ƕ�
	startSCCB();
	if(0==SCCBwriteByte(0x43))//����ַ
	{
		stopSCCB();//����SCCB ����ֹͣ��������
		return(0);//���󷵻�
	}
	delay_us(100);
  	*regDat=SCCBreadByte();//���ض�����ֵ
  	noAck();//����NACK����
  	stopSCCB();//����SCCB ����ֹͣ��������
  	return(1);//�ɹ�����
}




/* Sensor_init() */
//����1�ɹ�������0ʧ��
//guanfu_wang
unsigned char Sensor_init(void)
{
	unsigned char temp;
	
	unsigned int i=0;
         XCLK_init_ON();//����MCO���� �ṩʱ�Ӹ�CMOS������
	//uchar ovidmsb=0,ovidlsb=0;
	Sensor_GPIO_Init();
	SCCB_GPIO_Config();//io init..
        
	temp=0x80;
	if(0==wr_Sensor_Reg(0x12, temp)) //Reset SCCB
	{
          return 0 ;//���󷵻�
	}
        Delay(10);
	if(0==rd_Sensor_Reg(0x0b, &temp))//��ID
	{
		return 0 ;//���󷵻�
	}
        
         if(temp==0x73)//OV7670
	 {
	   for(i=0;i<OV7670_REG_NUM;i++)
	   {
		if( 0==wr_Sensor_Reg(OV7670_reg[i][0],OV7670_reg[i][1]))
		{
			return 0;//���󷵻�
		}
		}
   	  
	}
        else if(temp==0x21)//OV7725
	{
	   for(i=0;i<OV7725_REG_NUM;i++)
	   {
		if( 0==wr_Sensor_Reg(OV7725_reg[i][0],OV7725_reg[i][1]))
		{
			return 0;//���󷵻�
		}
	   }
	
	}

	return 0x01; //ok

} 

///////////////////


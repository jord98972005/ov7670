



/*
wangguanfu@163.com
QQ:21578936
未经过本人许可禁止任何商业用途
http://shop37116146.taobao.com/
*/


#include "main.h"

static volatile ErrorStatus HSEStartUpStatus = SUCCESS;
static vu32 TimingDelay = 0;
static vu32 SELStatus = 0;
extern unsigned int  a, bn,c_data;
unsigned int  kkk=0,s=0;

//NVIC_InitTypeDef NVIC_InitStructure;
 
//#define BufferSize  6400
 //unsigned int Buffer[2][BufferSize]={0};

 /*
  void my_DMA(char a)
 {
    DMA_InitTypeDef    DMA_InitStructure;
   DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = 0X6C000002;
  if(a==0)DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Buffer[0][0];    
   if(a==1)DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Buffer[1][0];  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = BufferSize;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  DMA_Cmd(DMA2_Channel5, ENABLE);
  while(!DMA_GetFlagStatus(DMA2_FLAG_TC5));
  DMA_ClearFlag(DMA2_FLAG_TC5);
 }
 */

/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 
 
int main(void)
{
  

#ifdef DEBUG
  debug();
#endif


  /* Intialize the STM32 SYSTEM_Init */
  SYSTEM_Init();
  LCD_write_english_string(5,66+16,"CMOS Sensor Init.....",0Xf800,0X0000);
  LCD_write_english_string(5,66,"GUANFU_WANG 2011-04015",0Xf800,0X0000);
  //kkk=LCD_ReadID();
  FIFO_Set_GPIO_Config();
  FIFO_GPIO_INPUT(); 
  FIFO_CS_L();
  FIFO_WE_H();
  while(1!=Sensor_init());//初始化CMOS Sensor
  LCD_write_english_string(5,66+16,"CMOS Sensor Init...ok",0Xf800,0X0000);
  Sensor_EXTI_Config(); 
  Sensor_Interrupts_Config();
  bn=0;
 ////////////////////////////////////////
  FIFO_RRST_L(); 
  FIFO_RD_L();
  FIFO_RD_H();
  FIFO_RD_L();
  FIFO_RRST_H();
  FIFO_RD_H();
  for(a=0;a<76800;a++)
   {
       FIFO_RD_L(); ;
       FIFO_RD_H(); 
       FIFO_RD_L();
       FIFO_RD_H();  
  }
  ////////////////////////////////////////
  while(1)
  {
    if(bn==2)
    {
        LCD_WriteReg(0x0020,239);//GRAM水平起始位置
        LCD_WriteReg(0x0021,319);      
         LCD_WriteReg(0x0050,0x00);//水平 GRAM起始位置
        LCD_WriteReg(0x0051,239);//水平GRAM终止位置
        LCD_WriteReg(0x0052,0x00);//垂直GRAM起始位置
        LCD_WriteReg(0x0053,319);//垂直GRAM终止位置 
        LCD_WriteReg(0x0003,0x1008);
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */  
        FIFO_RRST_L(); 
        FIFO_RD_L();
        FIFO_RD_H();
        FIFO_RD_L();
        FIFO_RRST_H();
        FIFO_RD_H();
        for(a=0;a<76800;a++)
        {

             FIFO_RD_L(); ;
             c_data=GPIOB->IDR&0xff00;//读高位
             FIFO_RD_H(); 
             FIFO_RD_L();
             c_data|=(GPIOB->IDR>>8)&0x00ff;//读低位
             FIFO_RD_H();  
            // LCD_WriteRAM(c_data);//写RGB数据到TFT GRAM
             LCD_RAM=c_data;
          
         }
         bn=0;
       }
     }
}


/*******************************************************************************
* Function Name  : SYSTEM_Init
* Description    : Intializes the STM32 SYSTEM_Init.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SYSTEM_Init(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
 
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_12);
 
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
 
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
 
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
         | RCC_APB2Periph_AFIO , ENABLE);
  
  /* DMA1 and DMA2 clock enable */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
    
  SysTick_Config();

/*------------------- Drivers Initialization -------------------------------*/
  /* Initialize the LCD */
  LCD_Init();

  /* Clear the LCD */ 
  LCD_Clear(0x0);
}


/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(720000);

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}


/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);

  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}




/*******************************************************************************
* Function Name  : Set_SELStatus
* Description    : Sets the SELStatus variable.
* Input          : None
* Output         : SELStatus
* Return         : None
*******************************************************************************/
void Set_SELStatus(void)
{
  SELStatus = 1;
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description:    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



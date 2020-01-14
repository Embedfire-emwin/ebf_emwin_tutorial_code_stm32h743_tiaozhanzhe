/**
  ******************************************************************
  * @file    bsp_adcd.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   adcӦ�ú����ӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32H743������ 
  * ��˾    :http://www.embedfire.com
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "./adc/bsp_adc.h" 

extern float ADC_vol;

ADC_HandleTypeDef ADC_Handle;
__IO uint16_t ADC_ConvertedValue = 0;
ADC_ChannelConfTypeDef ADC_Config;
DMA_HandleTypeDef DMA_Init_Handle;

/**
  * @brief  ADC�������ú���
  * @param  ��
  * @retval ��
  */  
static void ADC_GPIO_Mode_Config(void)
{
    /* ����һ��GPIO_InitTypeDef���͵Ľṹ�� */
    GPIO_InitTypeDef  GPIO_InitStruct;
    /* ʹ��ADC3���ŵ�ʱ�� */
    RHEOSTAT_ADC_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; 
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = RHEOSTAT_ADC_PIN;
    /* ����Ϊģ�����룬����Ҫ�������� */ 
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStruct);
  
}

/**
  * @brief  ADC����ģʽ���ú���
  * @param  ��
  * @retval ��
  */  
static void ADC_Mode_Config(void)
{
    // ����ADCʱ��
    RHEOSTAT_ADC_CLK_ENABLE();
    // -------------------ADC Init �ṹ�� ���� ��ʼ��------------------------
    // ADC1
    ADC_Handle.Instance = RHEOSTAT_ADC;
    // ʱ��Ϊfpclk 4��Ƶ	
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC �ֱ���
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
    // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
    ADC_Handle.Init.ScanConvMode = DISABLE; 
    // ����ת��	
    ADC_Handle.Init.ContinuousConvMode = ENABLE;
    // ������ת��	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    // ������ת������
    ADC_Handle.Init.NbrOfDiscConversion   = 0;
    //��ֹ�ⲿ���ش���    
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //ʹ������������ⲿ�����������ã�ע�͵�����
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //ת��ͨ�� 1��
    ADC_Handle.Init.NbrOfConversion = 1;
    //ת����ɱ�־
    ADC_Handle.Init.EOCSelection          = DISABLE;    
    // ��ʼ��ADC	                          
    HAL_ADC_Init(&ADC_Handle);
    //---------------------------------------------------------------------------
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL;
    ADC_Config.Rank         = 1;
    // ����ʱ����	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    ADC_Config.Offset       = 0;
    // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);

    HAL_ADC_Start_IT(&ADC_Handle);
}
/**
  * @brief  ADC�ж����ȼ����ú���
  * @param  ��
  * @retval ��
  */  
static void Rheostat_ADC_NVIC_Config(void)
{
  HAL_NVIC_SetPriority(Rheostat_ADC_IRQ, 2, 0);
  HAL_NVIC_EnableIRQ(Rheostat_ADC_IRQ);
}
/**
  * @brief  ADC��ʼ������
  * @param  ��
  * @retval ��
  */
void ADC_Init(void)
{
    
    ADC_GPIO_Mode_Config();
  
    ADC_Mode_Config();
    //ADC�ж����ȼ�����
    Rheostat_ADC_NVIC_Config();
}

/**
  * @brief  ת������жϻص�������������ģʽ��
  * @param  AdcHandle : ADC���
  * @retval ��
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* ��ȡ��� */
    ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle); 
}
/*********************************************END OF FILE**********************/



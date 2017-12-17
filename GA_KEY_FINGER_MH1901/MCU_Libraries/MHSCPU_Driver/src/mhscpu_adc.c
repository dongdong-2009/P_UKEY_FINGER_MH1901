/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : mhscpu_adc.c
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 09/12/2016
 * Description          : ADC module functions set.
 *****************************************************************************/
 
/* Include ------------------------------------------------------------------*/
#include "mhscpu_adc.h"

#define FILT_NUM    20      //滤波点数
#define REMOVE_NUM  6       //中位值滤波时头尾各需要去除的数量

/********************************************/
volatile uint32_t g_u32ADC_50K_REF_1V = 0;
volatile uint32_t g_u32ADC_300K_REF_1V = 0;
volatile uint32_t g_u32ADC_REF_VDD = 0;
volatile uint32_t g_u32ADC_CR = 0;
/********************************************/

/******************************************************************************
* Function Name  : Get_ADC_Reference
* Description    : 获取FLASH中预存的参考值,失败则获取VDD参考值
* Input          : void
* Return         : OPT1值
******************************************************************************/
uint32_t ADC_GetReference(void)
{
    uint32_t OTP1,OTP2;
    OTP1 = *((uint32_t *)FLASH_OTP1); 
    OTP2 = *((uint32_t *)FLASH_OTP2); 
    if (OTP1 == OTP2 && OTP1 != ~0UL)
    {
        g_u32ADC_50K_REF_1V = OTP1 & 0x3FF;
        g_u32ADC_300K_REF_1V = (OTP1 >> 16) & 0x3FF;
        return OTP1;
    }
    else
    {
        g_u32ADC_REF_VDD = ADC_Filt(ADC_CHANNEL_VDD);
		return g_u32ADC_REF_VDD;
    }
}

/******************************************************************************
* Function Name  : ADC_Init
* Description    : 初始化ADC,初始化参考值
* Input          : ADC_InitStruct：要初始化的数据结构指针
* Return         : NONE
******************************************************************************/
void ADC_Init(ADC_InitTypeDef *ADC_InitStruct)
{
    uint32_t u32Delay;
    assert_param(IS_ADC_CHANNEL(ADC_InitStruct->ADC_Channel));
    assert_param(IS_ADC_SAMP(ADC_InitStruct->ADC_SampSpeed));
    assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_IRQ_EN));
    /* Select ADC Channel */
    g_u32ADC_CR = ADC_InitStruct->ADC_Channel;
    /* Select ADC Channel Samping */
    g_u32ADC_CR |= ADC_InitStruct->ADC_SampSpeed << 3;
    
    g_u32ADC_CR |= ADC_START_BIT;
    ADC->ADC_CR = g_u32ADC_CR;                  //写ADC_CR必须将ADC_START_BIT位置1
    
    u32Delay = 0xFFF;                           //配置ADC后延时等待模拟部分稳定
    while(u32Delay--);
    
    ADC_GetReference();                         //在设置中断之前需要用查询方法先获取参考值
    /* Set ADC Interrupt */
    if (ENABLE == ADC_InitStruct->ADC_IRQ_EN )
    {
        NVIC_EnableIRQ(ADC_IRQn);
        g_u32ADC_CR |= ADC_IRQ_EN_BIT;
    }
    
    ADC->ADC_CR = g_u32ADC_CR;                  //写ADC_CR必须将ADC_START_BIT位置1
    u32Delay = 0xFFF;                           //配置ADC后延时等待模拟部分稳定
    while(u32Delay--);
}
/******************************************************************************
* Function Name  : Get_ADC_Result
* Description    : 立即获取ADC相应通道的值,有超时检测
* Input          : ADC通道：ADC_CHANNEL_0~5
* Return         : -1:获取超时  Other:ADC值
******************************************************************************/
int32_t ADC_GetResult(ADC_ChxTypeDef ADC_Channel)
{
    uint32_t u32TimeDelay = 0xffff;
    
    ADC->ADC_CR = g_u32ADC_CR | ADC_Channel;
    while((ADC->ADC_CR & ADC_START_BIT) && --u32TimeDelay );
    if (0UL == u32TimeDelay)
    {
        return -1;
    }
    else
    {
        return ADC->ADC_SR & 0x3FF;
    }
}
/******************************************************************************
* Function Name  : Get_ADC_Result_Wait
* Description    : 阻塞获取ADC相应通道的值,直到转换结束才可以返回
* Input          : ADC通道：ADC_CHANNEL_0~5
* Output         : NONE
* Return         : NONE
******************************************************************************/
uint32_t ADC_GetResult_Wait(ADC_ChxTypeDef ADC_Channel)
{
    ADC->ADC_CR = g_u32ADC_CR | ADC_Channel;
    while((ADC->ADC_CR & ADC_START_BIT));
    return ADC->ADC_SR & 0x3FF;
}
/******************************************************************************
* Function Name  : Get_ADC_Voltage
* Description    : 获取ADC相应通道的电压值
* Input          : ADC通道：ADC_CHANNEL_0~5
* Return         : 相应通道的电压值(放大1000倍)
******************************************************************************/
int32_t ADC_GetVoltage(ADC_ChxTypeDef ADC_Channel)
{
    uint32_t u32ADC_Result,u32ADC_Ref_1V;
    if (ADC->ADC_CR & BIT4)                 //不同采样率下1V参考值不同
    {
        u32ADC_Ref_1V = g_u32ADC_50K_REF_1V;
    }
    else
    {
        u32ADC_Ref_1V = g_u32ADC_300K_REF_1V;
    }
    
    if (0UL != u32ADC_Ref_1V)        //Used Ref_1V to Calculate
    {
        u32ADC_Result = ADC_GetResult_Wait(ADC_Channel);
        return (u32ADC_Result * 1000) / u32ADC_Ref_1V;
    }
    else                            //Used Ref_VDD to Calculate
    {     
        u32ADC_Result = ADC_GetResult_Wait(ADC_Channel);
        return (u32ADC_Result * 1000 * 330) / (g_u32ADC_REF_VDD * 309);
    }
}
/******************************************************************************
* Function Name  : Calculate_Voltage
* Description    : 计算ADC值对应的电压值
* Input          : ADC值
* Return         : 电压值(放大1000倍)
******************************************************************************/
uint32_t ADC_CalVoltage(uint32_t u32ADC_Value)
{
    uint32_t u32ADC_Ref_1V;
    if (ADC->ADC_CR & BIT4)                 //不同采样率下1V参考值不同
    {
        u32ADC_Ref_1V = g_u32ADC_50K_REF_1V;
    }
    else
    {
        u32ADC_Ref_1V = g_u32ADC_300K_REF_1V;
    }
    if (0UL != u32ADC_Ref_1V)        
    {
        return (u32ADC_Value * 1000) / u32ADC_Ref_1V;
    }
    else
    {     
        return (u32ADC_Value * 1000 * 330) / (g_u32ADC_REF_VDD * 309);
    }
}
/******************************************************************************
* Function Name  : ADC_Filt
* Description    : 中位值+算术平均滤波
* Input          : ADC通道：ADC_CHANNEL_0~5
* Return         : 滤波后的ADC值
******************************************************************************/
uint32_t ADC_Filt(ADC_ChxTypeDef ADC_Channel)
{
    int8_t i,j;
    uint16_t ADC_Buffer[FILT_NUM],tmp;
    
    if (REMOVE_NUM > FILT_NUM)  return 0;
    
    for(i = 0; i < FILT_NUM; i++)
    {
        ADC_Buffer[i] = ADC_GetResult_Wait(ADC_Channel);
    }
    for(j = 1; j < FILT_NUM; j++) //直接插入排序
    {
        tmp = ADC_Buffer[j];
        i = j - 1;
        //注:两个条件顺序不可调换
        while(i >= 0 && ADC_Buffer[i] > tmp)    
        {
            ADC_Buffer[i + 1] = ADC_Buffer[i];
            i--;
        }
        ADC_Buffer[i + 1] = tmp;
    }
    tmp = 0;
    for(i = REMOVE_NUM; i < FILT_NUM - REMOVE_NUM; i++)
    {
        tmp += ADC_Buffer[i];
    }
    return tmp / (FILT_NUM - 2 * REMOVE_NUM);
}

/******************************************************************************
* Function Name  : ADC_Filt_Average
* Description    : 算术平均滤波,
* Input          : ADC通道：ADC_CHANNEL_0~5
* Return         : 滤波后的ADC值
******************************************************************************/
uint32_t ADC_Filt_Average(ADC_ChxTypeDef ADC_Channel)
{
    int16_t i;
    uint32_t u32Sum = 0;

    uint16_t ADC_Buffer[FILT_NUM];
    for(i = 0; i < FILT_NUM; i++)
    {
        ADC_Buffer[i] = ADC_GetResult_Wait(ADC_Channel);
        u32Sum += ADC_Buffer[i];
    }
    return u32Sum / (FILT_NUM * 100);
}

/************************ (C) COPYRIGHT 2016 Megahuntmicro ****END OF FILE****/




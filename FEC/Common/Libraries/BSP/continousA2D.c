/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : a2d.c
 *
 *  FILE        :
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
 *
 *  PURPOSE     :
 *
 *  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
 *
 *  TARGET      :
 *
 *
 * -----------------------------------------------------------------------------
 *  2012.01     Igor Natachanny   1.0.0   Initial version
 *  2013.06     Maor Avni         2.0.0   Updated for continous conversion
 *
 *******************************************************************************
 *      Copyright and Proprietary Statment.
 *
 *******************************************************************************/

/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */

#include "ll_typedef.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

#include "free_rtos_exp.h"
#include "os_cnfg.h"
//#include "hmi.h"
//#include "bsp_srvcs.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "continousA2D.h"

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

/* --- PRIVATE TYPES -------------------------------------------------------- */
typedef struct
{
	uint16_t rtd_1; /* 1 */
	uint16_t rtd_2; /* 2 */
	uint16_t rtd_3; /* 3 */
	uint16_t rtd_4; /* 4 */
	uint16_t rtd_5; /* 5 */
	uint16_t rtd_6; /* 6 */
	uint16_t rtd_7; /* 7 */
	uint16_t rtd_8; /* 8 */
	uint16_t rtd_9; /* 9 */
	uint16_t hum_sens_1; /* 10 */
	uint16_t hum_sens_2; /* 11 */
	uint16_t in_4_20mA_1; /* 12 */
	uint16_t in_4_20mA_2; /* 13 */
	uint16_t in_4_20mA_3; /* 14 */
	uint16_t rtd_12; /* 15 */
	uint16_t rtd_10; /* 16 */
	uint16_t rtd_11; /* 17 */
} _field;

typedef union _a2d_data_
{
	_field field;
	uint16_t whole[sizeof(_field)];
} A2D_DATA_UT, A2D_DATA_PUT;

typedef enum
{
    READY = 0, BUSY
} A2D_STAT_ET;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
void a2d_task(void* pvParameters);

/* --- PRIVATE_DATA --------------------------------------------------------- */
A2D_DATA_UT a2d_results;
xSemaphoreHandle a2d_sem_1, a2d_sem_2, conv_end_sem;
static volatile uint32_t curr_sample_a2d_1 = 0, curr_sample_a2d_2 = 0;
float a2d_filtered_arr[A2D_NUM];
uint16_t a2d_samples_arr[A2D_NUM];
static float lowPassFilterCoeff;

/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
 * @brief: Retrive the desiried measured value.
 *
 * @param:
 *
 * @retval:
 **/
A2D_RES_ET a2d_read_dig(int param_id, uint16_t *val_ptr)
{

    if ((!val_ptr) || (param_id > A2D_NUM))
        return WRONG_PARAM;

    *val_ptr = a2d_results.whole[param_id];
    return A2D_COMPLETE;
}/**** A2D_RES_ET  a2d_read_dig(A2D_DATA_ET param_id, uint16_t *val_ptr) *****/

/**
 * @brief: Inits the STM32f207 on-chip A2D related HW parst.
 *
 * @param:
 *
 * @retval:
 **/
uint32_t a2d_start(unsigned portSHORT stack_depth, unsigned portBASE_TYPE priority)
{
    signed portBASE_TYPE res;

    for (uint32_t i = 0; i < A2D_NUM; i++)
        a2d_filtered_arr[i] = 0x00000000; /* Accumulator Array cleaning */

    vSemaphoreCreateBinary(a2d_sem_1);
    if (a2d_sem_1)
        xSemaphoreTake(a2d_sem_1, portMAX_DELAY);
    else
        return (-1);

    vSemaphoreCreateBinary(a2d_sem_2);
    if (a2d_sem_2)
        xSemaphoreTake(a2d_sem_2, portMAX_DELAY);
    else
        return (-1);

    vSemaphoreCreateBinary(conv_end_sem);
    if (conv_end_sem)
        xSemaphoreTake(conv_end_sem, portMAX_DELAY);
    else
        return (-1);

    res = xTaskCreate( ( pdTASK_CODE ) a2d_task, "a2d_task",stack_depth, NULL, priority, NULL );
    if (res == 1)
        return (0);
    else
        return (-1);

}/***************************** void a2d_run( ) *****************************/

/**
 * @brief: Inits the STM32f207 on-board A2D related HW parst.
 *
 * @param:
 *
 * @retval:
 **/
void a2d_init_hw(void)
{
#ifndef WIN32
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    uint32_t div_par;

    /* Enable ADC1 ,ADC3, DMA2 and GPIO clocks ****************************************/
    RCC_AHB1PeriphClockCmd(
            RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
                    | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC3, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //already done in board setup

    /* Enable the DMA2 Stream0 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 4; //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the DMA2 Stream1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 4; //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* DMA2 Stream1 channel2 configuration *********ADC3*****************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_2;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & (a2d_samples_arr[ADC3_in4]);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 8;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);

    DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1 );
    DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);

    /* DMA2 Stream0 channel0 configuration ************ADC1**************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & (a2d_samples_arr[ADC1_in0]);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 9;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0 );
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);

    //60Mhz/pres
    if (ADC_SMPLS_NUM < 128)
        div_par = ADC_Prescaler_Div8;
    else
        div_par = ADC_Prescaler_Div4;
    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = div_par;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC3 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 8;
    ADC_Init(ADC3, &ADC_InitStructure);

    /* ADC3 regular channels configuration *************************************/
    ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 2, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 3, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 4, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_8, 5, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_9, 6, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_14, 7, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC3, ADC_Channel_15, 8, ADC_SAMPLE_TIME );

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);

    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 9;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channels configuration *************************************/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 8, ADC_SAMPLE_TIME );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 9, ADC_SAMPLE_TIME );

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    a2d_config_lpf_window(500);
    /* Start  ADC1 ADC3 Software Conversion */

    ADC_SoftwareStartConv(ADC3 );
    ADC_SoftwareStartConv(ADC1 );
#endif
}/************************ void a2d_init_hw(void) *****************************/

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*******************************************************************************
 * @brief :  Task function.
 * @param
 * @retval
 *******************************************************************************/
void a2d_task(void* pvParameters)
{
#ifndef WIN32
    signed portBASE_TYPE res_1, res_2;

    while (1)
    {
        curr_sample_a2d_1 = curr_sample_a2d_2 = 0;
        ADC_SoftwareStartConv(ADC1 );
        ADC_SoftwareStartConv(ADC3 );
        res_1 = xSemaphoreTake(a2d_sem_1, portMAX_DELAY);
        res_2 = xSemaphoreTake(a2d_sem_2, portMAX_DELAY);
        if (res_1 && res_2)
        {
            xSemaphoreGive(conv_end_sem);
            xSemaphoreTake(conv_end_sem, portMAX_DELAY);

            for (uint32_t i = 0; i < A2D_NUM; i++)
            {
                a2d_results.whole[i] = (uint16_t)a2d_filtered_arr[i];
                a2d_filtered_arr[i] = 0x00000000; /* Accumulator Array cleaning */
            }
            xSemaphoreGive(conv_end_sem);
        }
        vTaskDelay(1000);
    }/* while (1) */
#else
	while (1)
	{
		vTaskDelay(1000000);
	}
#endif

}/******************* void a2d_task( void* pvParameters ) *******************/

void a2d_config_lpf_window(int window)
{
    lowPassFilterCoeff = 1 / ((float)window + 1);
}

float a2d_read_filtered_data(int index)
{
    return a2d_filtered_arr[index];
}

float lowPassFilter(float newVal, float prevVal, float coeff)
{
    return coeff * newVal + (1 - coeff) * prevVal;
}


#define M_LowPassFilter(newVal, prevVal, coeff) coeff * newVal + (1 - coeff) * prevVal

/*****************************************************************************
 * @brief :
 * @param
 * @retval
 *****************************************************************************/
float vId;
void DMA2_Stream0_IRQHandler(void)  //ADC1
{
#ifndef WIN32
    static portBASE_TYPE xHigherPriorityTaskWoken;

    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0 );

    for (uint32_t i = FIRST_IN_CONV_1; i < FIRST_IN_CONV_2; i++)
        a2d_filtered_arr[i] = M_LowPassFilter(a2d_samples_arr[i], a2d_filtered_arr[i], lowPassFilterCoeff);

//    led_toggle(LABL_LED7);

    ADC_SoftwareStartConv(ADC1 );
#endif
}

/*******************************************************************************
 * @brief :
 * @param
 * @retval
 *******************************************************************************/

void DMA2_Stream1_IRQHandler(void)  //ADC3
{
#ifndef WIN32
    static portBASE_TYPE xHigherPriorityTaskWoken;

    DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1 );

    for (uint32_t i = FIRST_IN_CONV_2; i < A2D_NUM; i++)
        a2d_filtered_arr[i] = M_LowPassFilter(a2d_samples_arr[i], a2d_filtered_arr[i], lowPassFilterCoeff);

//    led_toggle(LABL_LED8);

    ADC_SoftwareStartConv(ADC3 );
#endif
}/*************** void DMA2_Stream1_IRQHandler(void) ************************/

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
 * @brief:
 *
 * @param:
 *
 * @retval:
 **/

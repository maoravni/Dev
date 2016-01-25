/*****************************************************************************
 * File Name: Digital_IO_Module_Handler.h
 *
 * Version: 02.02
 *
 * Description:
 *   File to handle the Digital Inputs and outputs.
 *
 * Note:
 ********************************************************************************/

/***************************************
 *    Includes
 ***************************************/
#include "main.h"
#include "Digital_IO_Module_Handler.h"
#include "ICDMessageStructs.h"
#include "System_Handler.h"
#include "Temperature_Module_Handler.h"
#include "Monitor_Module_Handler.h"

/***************************************
 *    Global Variables
 ***************************************/
uint8 CurrentPwmValues[NUM_OF_PWM_OUTS];
uint8 RequestedPwmValues[NUM_OF_PWM_OUTS];
uint8 CurrentRampTicks[NUM_OF_PWM_OUTS];

U_StoredDigiIn4MMCU StoredDigiIn4MMCU;

uint8 ApplicatorDripValveEnable;

typedef struct
{
    uint8 riseStepSize;
    uint8 fallStepSize;
    uint8 riseTickCount;
    uint8 fallTickCount;
    uint8 outputRampActive;
} T_OutputRampParameters;

T_OutputRampParameters OutputRampParameters[NUM_OF_PWM_OUTS];

/***************************************
 *    Function
 ***************************************/

/*******************************************************************************
 * Function Name: Init_Digital_Module()
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   none.
 *******************************************************************************/
void Init_Digital_Module(void)
{
    DOD_Start();
    memset(CurrentPwmValues, 0, sizeof(CurrentPwmValues));
    memset(RequestedPwmValues, 0, sizeof(CurrentPwmValues));
    memset(CurrentRampTicks, 0, sizeof(CurrentRampTicks));
    StoredDigiIn4MMCU.Byte = 0;
    memset(OutputRampParameters, 0, sizeof(T_OutputRampParameters) * NUM_OF_PWM_OUTS);
}

/*******************************************************************************
 * Function Name: void DigitalInputs_Read()
 ********************************************************************************
 * Summary:
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   none.
 *******************************************************************************/
void DigitalInputs_Read()
{
    StoredDigiIn4MMCU.Struct.DI1 = Digital_input_1_Read();
    StoredDigiIn4MMCU.Struct.DI2 = Digital_input_2_Read();
    StoredDigiIn4MMCU.Struct.DI3 = Digital_input_3_Read();
    StoredDigiIn4MMCU.Struct.DI4 = Digital_input_4_Read();
    StoredDigiIn4MMCU.Struct.DI5 = Digital_input_5_Read();
    StoredDigiIn4MMCU.Struct.DI6 = Digital_input_6_Read();

    StoredDigiIn4MMCU.Struct.GI1 = Global_input_1_Read();
    StoredDigiIn4MMCU.Struct.GI2 = Global_input_2_Read();
    StoredDigiIn4MMCU.Struct.GI3 = Global_input_3_Read();
    StoredDigiIn4MMCU.Struct.GI4 = Global_input_4_Read();
    StoredDigiIn4MMCU.Struct.GI5 = Global_input_5_Read();
    StoredDigiIn4MMCU.Struct.GI6 = Global_input_6_Read();
}

void ApplicatorEnable(int state)
{
    if (ApplicatorDripValveEnable == FALSE)
    {
        if (state)
        {
            if (!checkForLiquidTimeout())
            {
                ApplicatorDripValveEnable = TRUE;
                memset(No_Liquid_Timer, 0, sizeof(No_Liquid_Timer));
                SystemErrorReg_Write(LIQUID_DETECTOR_ERR_BIT_NUM, DISABLE, _16BIT1);
            }
        }
    }
//    else
    //{
    if (!state)
    {
        CurrentPwmValues[3] = DOD_PWMOut2_Write(state);
        ApplicatorDripValveEnable = FALSE;
        memset(No_Liquid_Timer, 0, sizeof(No_Liquid_Timer));
    }
    //}
    CurrentPwmValues[3] = ApplicatorDripValveEnable;
}

uint8 DigitalOutputsForceWrite(uint8 channelIndex, uint8 outputValue)
{
    uint8 status = E_Success;

    switch (channelIndex)
    {
    case 0:
    {
        if (SystemState.SystemErrors & TEMP_MDL_ERR_MASK)
            CurrentPwmValues[0] = DOD_SlowPWM1_Write(0);
        else
            CurrentPwmValues[0] = DOD_SlowPWM1_Write(outputValue);
        if (!CurrentPwmValues[0])
        {
#ifdef FEC2_PSoC_V01
            memset(CS_Always_OFF_Counter,0,sizeof(CS_Always_OFF_Counter));
#endif
#ifdef FEC2_PSoC_V02
            CS_Always_OFF_Counter = 0;
#endif
        }
        break;
    }
    case 1:
    {
        CurrentPwmValues[1] = DOD_SlowPWM2_Write(outputValue);
        break;
    }
    case 2:
    {
        CurrentPwmValues[2] = DOD_PWMOut1_Write(outputValue);
        break;
    }
    case 3:
    {
        if (GET_BOARD_TYPE == E_PsocPrimaryFunction_Applicator)
        {
            ApplicatorEnable(outputValue);
        }
        else
        {
            CurrentPwmValues[3] = DOD_PWMOut2_Write(outputValue);
        }
        break;
    }
    case 4:
    {
        CurrentPwmValues[4] = DOD_HB_PWMOut1_Write(outputValue);
        break;
    }
    case 5:
    {
        CurrentPwmValues[5] = DOD_HB_PWMOut2_Write(outputValue);
        break;
    }
    default:
    {
        status |= E_InvalidDeviceIndex;
        break;
    }
    }

    return status;
}

void ConfigureSoftStartParameters(uint8 channelIndex, uint8 riseStepSize, uint8 fallStepSize, uint8 riseTickCount,
        uint8 fallTickCount)
{
    if (riseStepSize == 0 && fallStepSize == 0)
        OutputRampParameters[channelIndex].outputRampActive = 0;
    else
        OutputRampParameters[channelIndex].outputRampActive = 1;

    OutputRampParameters[channelIndex].riseStepSize = riseStepSize;
    OutputRampParameters[channelIndex].fallStepSize = fallStepSize;
    OutputRampParameters[channelIndex].riseTickCount = riseTickCount;
    OutputRampParameters[channelIndex].fallTickCount = fallTickCount;
}

void ExecuteRampStep(uint8 i)
{
    int tempOutputValue;
    // check if we need to increment or to decrement the output:
    if (CurrentPwmValues[i] < RequestedPwmValues[i])
    {
        // increment the output:

        // check if the tick count have elapsed:
        if (CurrentRampTicks[i] >= OutputRampParameters[i].riseTickCount)
        {
            CurrentRampTicks[i] = 0;
            tempOutputValue = CurrentPwmValues[i] + OutputRampParameters[i].riseStepSize;

            // check if we have stepped too far:
            if (tempOutputValue > RequestedPwmValues[i])
                tempOutputValue = RequestedPwmValues[i];

            // update the output device:
            DigitalOutputsForceWrite(i, tempOutputValue);

            // todo: compare the tempOutValue to the actual CurrentPwmValue, to make sure that the update have been updated correctly.
        }
    }
    else
    {
        // decrement the output:

        // check if the tick count have elapsed:
        if (CurrentRampTicks[i] >= OutputRampParameters[i].fallTickCount)
        {
            CurrentRampTicks[i] = 0;
            tempOutputValue = CurrentPwmValues[i] - OutputRampParameters[i].fallStepSize;

            // check if we have stepped too far:
            if (tempOutputValue < RequestedPwmValues[i])
                tempOutputValue = RequestedPwmValues[i];

            // update the output device:
            DigitalOutputsForceWrite(i, tempOutputValue);

            // todo: compare the tempOutValue to the actual CurrentPwmValue, to make sure that the update have been updated correctly.
        }
    }
}
void ExecuteOutputRamp()
{
    int i;
    for (i = 0; i < NUM_OF_PWM_OUTS; ++i)
    {
        // second, check if output ramp is active for the current output:
        if (OutputRampParameters[i].outputRampActive == 0)
        {
            // if we're here, it means the requested output and the current output are not the same,
            // so we force an update to output:
            DigitalOutputsForceWrite(i, RequestedPwmValues[i]);
            continue;
        }

        // first, check if the current value and the requested value is the same:
        if (CurrentPwmValues[i] == RequestedPwmValues[i])
            continue;

        // increment the tick count for the current output:
        ++CurrentRampTicks[i];

        ExecuteRampStep(i);
    }
}

uint8 DigitalOutputsWrite(uint8 channelIndex, uint8 outputValue)
{
    if (GET_BOARD_TYPE == E_PsocPrimaryFunction_Applicator && channelIndex == 3)
    {
        ApplicatorEnable(outputValue);
        return E_Success;
    }

    if (channelIndex == 0 && (SystemState.SystemErrors & TEMP_MDL_ERR_MASK))
    {
        outputValue = 0;
        RequestedPwmValues[channelIndex] = outputValue;
        return DigitalOutputsForceWrite(0, 0);
    }

    // first, update the requested values:
    RequestedPwmValues[channelIndex] = outputValue;

    // next, check if the output ramp is active for the current channel:
    if (OutputRampParameters[channelIndex].outputRampActive == 0)
    {
        return DigitalOutputsForceWrite(channelIndex, outputValue);
    }
    else
    {
        // force the first step by setting the current tick count to the maximum between the tick counters:
        CurrentRampTicks[channelIndex] =
                (OutputRampParameters[channelIndex].fallTickCount > OutputRampParameters[channelIndex].riseTickCount) ?
                        OutputRampParameters[channelIndex].fallTickCount :
                        OutputRampParameters[channelIndex].riseTickCount;

        ++CurrentRampTicks[channelIndex];

        // ramp is active for this channel, so execute the first step:
        ExecuteRampStep(channelIndex);

        // reset the tick counter:
        CurrentRampTicks[channelIndex] = 0;
        return E_Success;
    }

    return E_InvalidValue;
}

/* [] END OF FILE */

/*
 * DigitalInputsPeripheral.cpp
 *
 *  Created on: 28 αιεμ 2013
 *      Author: maora
 */

#include "DigitalInputsPeripheral.h"
#include <dg_in_ctrl.h>

DigitalInputsPeripheral::DigitalInputsPeripheral()
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_INPUTS; ++i)
    {
        m_elementsArray[i] = ElementRepository::getInstance().addElementU8();
        m_inputValues[i] = -1;
    }
    // TODO: Implement digital inputs using interrupts.
    setUpdateInterval(10);

    m_boardType = (E_BoardType) Psc_GetBoardType();
}

DigitalInputsPeripheral::~DigitalInputsPeripheral()
{
}

#define M_ASSIGN_VALUE_TO_ELEMENT(i, e) dig_in_get(i, &temp); if (temp != m_inputValues[e]) *m_elementsArray[e] = temp; m_inputValues[e] = temp
void DigitalInputsPeripheral::readInputs()
{
    uint8_t temp;
#ifdef FEC2_BOARD
    if (m_boardType == E_BoardType_Fec3)
    {
        M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_1_FEC3, 0);
        M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_5_FEC3, 4);
    }
    else
    {
        M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_1_FEC2, 0);
        M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_5_FEC2, 4);
    }
    M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_2, 1);
    M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_3, 2);
    M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_4, 3);
    M_ASSIGN_VALUE_TO_ELEMENT(GLOBAL_IN_6, 5);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_1, 6);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_2, 7);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_3, 8);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_4, 9);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_5, 10);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_6, 11);
    M_ASSIGN_VALUE_TO_ELEMENT(SAFETY_FEEDBACK, 12);
#else
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_1, 0);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_2, 1);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_3, 2);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_4, 3);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_5, 4);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_6, 5);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_7, 6);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_8, 7);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_9, 8);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_10, 9);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_11, 10);
    M_ASSIGN_VALUE_TO_ELEMENT(DIG_IN_12, 11);
#endif
}

ElementBase* DigitalInputsPeripheral::getElementByIndex(int index)
{
    if (index >= M_NUM_OF_DIGITAL_INPUTS)
        return NULL;

    return m_elementsArray[index];
}

ElementBase* DigitalInputsPeripheral::getElementByPssId(int pssId)
{
    for (int i = 0; i < M_NUM_OF_DIGITAL_INPUTS; ++i)
    {
        if (m_elementsArray[i]->getPssId() == pssId)
            return m_elementsArray[i];
    }
    return NULL;
}

void DigitalInputsPeripheral::enableElementByIndex(int index, bool enable)
{
}

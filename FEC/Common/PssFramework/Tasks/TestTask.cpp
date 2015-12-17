/*
 * TestTask.cpp
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#if defined STM32F4XX
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#include <Win32/PortAllocations.h>
#else
#include <stm32f2xx.h>
#include <DigTrimmer_ad5292bruz.h>
#endif

//#include <Controls/ControlRepository.h>
#include <opto_out_cnfg.h>
#include <Peripherals/DigitalOutputsPeripheral.h>
#include <Peripherals/InternalTemperatureSensors.h>
#include <Peripherals/Modbus6RTDPeripheral.h>
#include <Peripherals/ModbusPumaPeripheral.h>
#include <Peripherals/ModBus8TCPeripheral.h>
#include <Peripherals/PeripheralRepository.h>
#include <Peripherals/SwPwmOutputPeripheral.h>
#include <stdio.h>
#include <Tasks/TestTask.h>

//#include <Peripherals/ModbusInverterPeripheral.h>
#ifdef FEC2_BOARD
#include <Peripherals/Mi3I2CIrPeripheral.h>
#endif
#include <Controls/PidControl.h>
#include <Tasks/UpdateSchedulerTask.h>
#include <psocSpiWithDma.h>
#include <board_setup.h>
#include <Controls/LiquidLevel3Sensors.h>
#include <opto_out.h>
#include <Peripherals/PSoC/PsocHandler.h>
#include <Peripherals/PSoC/PsocManager.h>
#include <continousA2D.h>
#include <Controls/OrderedShutdownControl.h>
#include <AppLoader.h>
#include <PscServer/PscMessageHandler.h>

TestTask::TestTask()
{

}

TestTask::~TestTask()
{
}

PsocHandler* psocHandler;

float boardVoltage;
float scale;
int cableIdCounts;
int cableIdScaledCounts;
int cableId;

void TestTask::run()
{
    puts("testTask");

    Safety_out_ctrl(ACTIVE);
    (GPIOD->ODR &= (~GPIO_Pin_12));

    delay(1000);

#ifdef FEC2_BOARD
    Mi3I2CIrPeripheral *mi3Periph = new Mi3I2CIrPeripheral();

    PeripheralRepository::getInstance().addPeripheral(mi3Periph);

    Mi3Sensor* mi3Sensor = mi3Periph->getSensorByAddress(0x14);
//    ElementBase* element = mi3Sensor->t;
    mi3Sensor->writeAmbientBackgroundCompensation(true);
    mi3Sensor->setI2CChannel(1);
    mi3Periph->setBoardInReady(true);
    mi3Periph->setUpdateInterval(25);
#endif

//    PsocAppLoader *appLoader = new PsocAppLoader(PscMessageHandler::getInstance()->getPsocManager());
//    appLoader->m_chksumtype = 1;
//    appLoader->m_siliconId = 0x2e120069;
//    appLoader->m_siliconRev = 0;
//    appLoader->setAppTarget(0x1);
//
//    appLoader->writeRowDataFromFlashToPSoC();

    for (;;)
    {
        delay(100000);
    }

}

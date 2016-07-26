/*
 * TestTask.cpp
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#if defined STM32F4XX
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <eeprom_m24m02_dr.h>
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
#include <controls/ConcentrationControl.h>
#include <filesystemdriver.h>
#include <Persistency/PersistencyManager.h>
#include <Peripherals/ModbusInverterSchneiderAtv32.h>
#include <Peripherals/ModbusInverterCommanderSK.h>
#include <Peripherals/ModbusInverterUnidriveM200.h>
#include <Controls/ControlRepository.h>
#include <tm_stm32f4_usart_dma.h>
#include <usart.h>

TestTask::TestTask()
{

}

TestTask::~TestTask()
{
}

/* The number of bytes read/written to the example files at a time. */
#define fsRAM_BUFFER_SIZE               200
#define fsMAX_FILE_NAME_LEN             13
#define fsPUTC_FILE_SIZE                100
static char cRAMBuffer[ fsRAM_BUFFER_SIZE];
static const char *pcRoot = "/", *pcDirectory1 = "SUB1", *pcDirectory2 = "SUB2", *pcFullPath = "/SUB1/SUB2";

static void prvCreateDemoFilesUsing_f_write(void)
{
    BaseType_t xFileNumber, xWriteNumber;
    char cFileName[fsMAX_FILE_NAME_LEN];
    const BaseType_t xMaxFiles = 5;
    long lItemsWritten;
    F_FILE *pxFile;

    /* Create xMaxFiles files.  Each created file will be
     ( xFileNumber * fsRAM_BUFFER_SIZE ) bytes in length, and filled
     with a different repeating character. */
    for (xFileNumber = 1; xFileNumber <= xMaxFiles; xFileNumber++)
    {
        /* Generate a file name. */
        sprintf(cFileName, "root%03d.txt", xFileNumber);

        /* Obtain the current working directory and print out the file name and
         the directory into which the file is being written. */
        f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
        printf("Creating file %s in %s\r\n", cFileName, cRAMBuffer);

        /* Open the file, creating the file if it does not already exist. */
        pxFile = f_open(cFileName, "w");
        configASSERT( pxFile );

        /* Fill the RAM buffer with data that will be written to the file.  This
         is just a repeating ascii character that indicates the file number. */
        memset(cRAMBuffer, (int) ('0' + xFileNumber), fsRAM_BUFFER_SIZE);

        /* Write the RAM buffer to the opened file a number of times.  The
         number of times the RAM buffer is written to the file depends on the
         file number, so the length of each created file will be different. */
        for (xWriteNumber = 0; xWriteNumber < xFileNumber; xWriteNumber++)
        {
            lItemsWritten = f_write(cRAMBuffer, fsRAM_BUFFER_SIZE, 1, pxFile);
            configASSERT( lItemsWritten == 1 );
        }

        /* Close the file so another file can be created. */
        f_close(pxFile);
    }
}
/*-----------------------------------------------------------*/

static void prvVerifyDemoFileUsing_f_read(void)
{
    BaseType_t xFileNumber, xReadNumber;
    char cFileName[fsMAX_FILE_NAME_LEN];
    const BaseType_t xMaxFiles = 5;
    long lItemsRead, lChar;
    F_FILE *pxFile;

    /* Read back the files that were created by
     prvCreateDemoFilesUsing_f_write(). */
    for (xFileNumber = 1; xFileNumber <= xMaxFiles; xFileNumber++)
    {
        /* Generate the file name. */
        sprintf(cFileName, "root%03d.txt", xFileNumber);

        /* Obtain the current working directory and print out the file name and
         the directory from which the file is being read. */
        f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
        printf("Reading file %s from %s\r\n", cFileName, cRAMBuffer);

        /* Open the file for reading. */
        pxFile = f_open(cFileName, "r");
        configASSERT( pxFile );

        /* Read the file into the RAM buffer, checking the file contents are as
         expected.  The size of the file depends on the file number. */
        for (xReadNumber = 0; xReadNumber < xFileNumber; xReadNumber++)
        {
            /* Start with the RAM buffer clear. */
            memset(cRAMBuffer, 0x00, fsRAM_BUFFER_SIZE);

            lItemsRead = f_read(cRAMBuffer, fsRAM_BUFFER_SIZE, 1, pxFile);
            configASSERT( lItemsRead == 1 );

            /* Check the RAM buffer is filled with the expected data.  Each
             file contains a different repeating ascii character that indicates
             the number of the file. */
            for (lChar = 0; lChar < fsRAM_BUFFER_SIZE; lChar++)
            {
                configASSERT( cRAMBuffer[ lChar ] == ( '0' + ( char ) xFileNumber ) );
            }
        }

        /* Close the file. */
        f_close(pxFile);
    }
}
/*-----------------------------------------------------------*/

static void prvCreateDemoFileUsing_f_putc(void)
{
    unsigned char ucReturn;
    int iByte, iReturned;
    F_FILE *pxFile;
    char cFileName[fsMAX_FILE_NAME_LEN];

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("In directory %s\r\n", cRAMBuffer);

    /* Create a sub directory. */
    ucReturn = f_mkdir(pcDirectory1);
    configASSERT( ucReturn == F_NO_ERROR );

    /* Move into the created sub-directory. */
    ucReturn = f_chdir(pcDirectory1);
    configASSERT( ucReturn == F_NO_ERROR );

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("In directory %s\r\n", cRAMBuffer);

    /* Create a subdirectory in the new directory. */
    ucReturn = f_mkdir(pcDirectory2);
    configASSERT( ucReturn == F_NO_ERROR );

    /* Move into the directory just created - now two directories down from
     the root. */
    ucReturn = f_chdir(pcDirectory2);
    configASSERT( ucReturn == F_NO_ERROR );

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("In directory %s\r\n", cRAMBuffer);
    configASSERT( strcmp( cRAMBuffer, pcFullPath ) == 0 );

    /* Generate the file name. */
    sprintf(cFileName, "%s.txt", pcDirectory2);

    /* Print out the file name and the directory into which the file is being
     written. */
    printf("Writing file %s in %s\r\n", cFileName, cRAMBuffer);

    pxFile = f_open(cFileName, "w");

    /* Create a file 1 byte at a time.  The file is filled with incrementing
     ascii characters starting from '0'. */
    for (iByte = 0; iByte < fsPUTC_FILE_SIZE; iByte++)
    {
        iReturned = f_putc(((int ) '0' + iByte), pxFile);
        configASSERT( iReturned == ( ( int ) '0' + iByte ) );
    }

    /* Finished so close the file. */
    f_close(pxFile);

    /* Move back to the root directory. */
    ucReturn = f_chdir("../..");
    configASSERT( ucReturn == F_NO_ERROR );

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("Back in root directory %s\r\n", cRAMBuffer);
    configASSERT( strcmp( cRAMBuffer, pcRoot ) == 0 );
}
/*-----------------------------------------------------------*/

static void prvVerifyDemoFileUsing_f_getc(void)
{
    unsigned char ucReturn;
    int iByte, iReturned;
    F_FILE *pxFile;
    char cFileName[fsMAX_FILE_NAME_LEN];

    /* Move into the directory in which the file was created. */
    ucReturn = f_chdir(pcFullPath);
    configASSERT( ucReturn == F_NO_ERROR );

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("Back in directory %s\r\n", cRAMBuffer);
    configASSERT( strcmp( cRAMBuffer, pcFullPath ) == 0 );

    /* Generate the file name. */
    sprintf(cFileName, "%s.txt", pcDirectory2);

    /* Print out the file name and the directory from which the file is being
     read. */
    printf("Reading file %s in %s\r\n", cFileName, cRAMBuffer);

    /* This time the file is opened for reading. */
    pxFile = f_open(cFileName, "r");

    /* Read the file 1 byte at a time. */
    for (iByte = 0; iByte < fsPUTC_FILE_SIZE; iByte++)
    {
        iReturned = f_getc(pxFile);
        configASSERT( iReturned == ( ( int ) '0' + iByte ) );
    }

    /* Finished so close the file. */
    f_close(pxFile);

    /* Move back to the root directory. */
    ucReturn = f_chdir("../..");
    configASSERT( ucReturn == F_NO_ERROR );

    /* Obtain and print out the working directory. */
    f_getcwd(cRAMBuffer, fsRAM_BUFFER_SIZE);
    printf("Back in root directory %s\r\n", cRAMBuffer);
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

//#ifdef FEC2_BOARD
//    Mi3I2CIrPeripheral *mi3Periph = new Mi3I2CIrPeripheral();
//
//    PeripheralRepository::getInstance().addPeripheral(mi3Periph);
//
//    Mi3Sensor* mi3Sensor = mi3Periph->getSensorByAddress(0x14);
////    ElementBase* element = mi3Sensor->t;
//    mi3Sensor->writeAmbientBackgroundCompensation(true);
//    mi3Sensor->setI2CChannel(1);
//    mi3Periph->setBoardInReady(true);
//    mi3Periph->setUpdateInterval(25);
//
//    ControlBase* conc = new ConcentrationControl();
//#endif

//    PsocAppLoader *appLoader = new PsocAppLoader(PscMessageHandler::getInstance()->getPsocManager());
//    appLoader->m_chksumtype = 1;
//    appLoader->m_siliconId = 0x2e120069;
//    appLoader->m_siliconRev = 0;
//    appLoader->setAppTarget(0x1);
//
//    appLoader->writeRowDataFromFlashToPSoC();

//    in_addr addr;
//    addr.s_addr = 0x631e1eac;
//    CLogger::getInstance().updateOutputUdpIpAddress(addr);

// filesystem test
#define EEPROM_TEST
#ifdef EEPROM_TEST

#define TEST_SER
#ifdef TEST_SER
    PscMessageHandler::getInstance()->getPsocManager()->initTemperaturePeripheralByCableId(1, 100, 100, 10, 4);
    PscMessageHandler::getInstance()->getPsocManager()->initPwmPeripheralByCableId(1, 101, 6);
    PscMessageHandler::getInstance()->getPsocManager()->initAnalogOutputPeripheralByCableId(1, 102, 2);
    PscMessageHandler::getInstance()->getPsocManager()->initDigitalOutputPeripheralByCableId(1, 103, 6);
    PscMessageHandler::getInstance()->getPsocManager()->initDigitalInputPeripheralByCableId(1, 104, 6);
    PscMessageHandler::getInstance()->getPsocManager()->initAnalogInputPeripheralByCableId(1, 555, 200, 5, 2);

    VirtualPeripheral* virtPeriph = new VirtualPeripheral(E_PeripheralType_VirtualFloatPeripheral);
    virtPeriph->getElementByIndex(0);
    virtPeriph->getElementByIndex(1);
    virtPeriph->getElementByIndex(2);
    virtPeriph->getElementByIndex(3);
    virtPeriph->getElementByIndex(4);
    PeripheralRepository::getInstance().addPeripheral(virtPeriph);

    PeripheralRepository::getInstance().initDigitalOutputs(0, 12);
    PeripheralRepository::getInstance().initSwPwmOutput(0, 12);
    PeripheralRepository::getInstance().initDryContactOutput(0);
    PeripheralRepository::getInstance().initDigitalInputs(0, 12);

    ModbusInverterSchneiderAtv32* modbusInverterPeriph = new ModbusInverterSchneiderAtv32(2);
    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)modbusInverterPeriph);
    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)new ModbusInverterCommanderSK(3));
    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)new ModbusInverterUnidriveM200(4));
    PeripheralRepository::getInstance().addPeripheral(new Modbus6RTDPeripheral(5));
    PeripheralRepository::getInstance().addPeripheral(new Modbus8TCPeripheral(6));
    PeripheralRepository::getInstance().addPeripheral(new ModbusPumaPeripheral(7));
    PeripheralRepository::getInstance().addPeripheral(new ModbusDataCardPeripheral(8));

    PidControl* pidControl = new PidControl();
    pidControl->setElementInput(static_cast<ValidationElementFloat*>(PscMessageHandler::getInstance()->getPsocManager()->getTemperaturePeripheralByCableId(1)->getElementByIndex(0)));
    pidControl->setElementOutput(PscMessageHandler::getInstance()->getPsocManager()->getAnalogOutPeripheralByCableId(1)->getElementByIndex(0), false);
    ControlRepository::getInstance().addControl(pidControl);

    DeviceProtectionChecker* deviceProt = ControlRepository::getInstance().getProtectionControl()->createDeviceProtectionChecker();
    deviceProt->setElement(PscMessageHandler::getInstance()->getPsocManager()->getTemperaturePeripheralByCableId(1)->getElementByIndex(1));
    deviceProt->setSoftProtectionRange(0, 1, false, true);
    deviceProt->setHardProtectionRange(1, 2, false, false);
    pidControl->addProtectionChecker(deviceProt);

    ProtectionConstantDeltaChecker* deltaProt = ControlRepository::getInstance().getProtectionControl()->createProtectionConstantDeltaChecker();
    deltaProt->setElement(PscMessageHandler::getInstance()->getPsocManager()->getTemperaturePeripheralByCableId(1)->getElementByIndex(2));
    deltaProt->setAllowedDelta(30, 30);
    deltaProt->setReferenceElement(PscMessageHandler::getInstance()->getPsocManager()->getTemperaturePeripheralByCableId(1)->getElementByIndex(3));
    pidControl->addProtectionChecker(deltaProt);

    ProtectionCurrentLimitsChecker* currentProt = ControlRepository::getInstance().getProtectionControl()->createProtectionCurrentLimitsChecker();
    currentProt->setElement(PscMessageHandler::getInstance()->getPsocManager()->getDigitalInPeripheralByCableId(1)->getElementByIndex(0));
    currentProt->setLimits(10, 20, 30);
    pidControl->addProtectionChecker(currentProt);

    ProtectionProportionalChecker* propProt = ControlRepository::getInstance().getProtectionControl()->createProtectionProportionalChecker();
    propProt->setElement(PscMessageHandler::getInstance()->getPsocManager()->getAnalogInPeripheralByCableId(1)->getElementByIndex(0));
    propProt->setReferenceElement(PscMessageHandler::getInstance()->getPsocManager()->getAnalogInPeripheralByCableId(1)->getElementByIndex(1));
    propProt->setParameters(1, 2, 3, 4);
    pidControl->addProtectionChecker(propProt);

    DeviceThresholdChecker checker;
    checker.setElement(pidControl->getPssId(), PscMessageHandler::getInstance()->getPsocManager()->getDigitalInPeripheralByCableId(1)->getElementByIndex(1));
    checker.m_thresholdValue = 1;
    checker.m_greaterThan = true;
    checker.m_deactivateOnChange = true;
    checker.m_lockChange = true;
    checker.m_dependencyCheckType = (E_DependencyCheckType) E_DependencyCheckType_Activate;
    pidControl->addDependentElement(checker);

    AnalogOutInverterControl* analogOutInverter = new AnalogOutInverterControl();
    analogOutInverter->setOutputEnableElement(static_cast<ElementU8*>(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(1)));
    analogOutInverter->setSetpointElement(static_cast<ValidationElementFloat*>(PscMessageHandler::getInstance()->getPsocManager()->getAnalogOutPeripheralByCableId(1)->getElementByIndex(1)));
    ControlRepository::getInstance().addControl(analogOutInverter);

    ModbusInverterControl* modbusInverter = new ModbusInverterControl();
    modbusInverter->setOutputEnableElement(PeripheralRepository::getInstance().getDigitalOutputsPeripheral()->getElementByIndex(2));
    modbusInverter->setOutputFrequencyElement(modbusInverterPeriph->getElementByIndex(0));
    modbusInverter->setOutputCurrentElement(modbusInverterPeriph->getElementByIndex(1));
    modbusInverter->setOutputSetpointElement(modbusInverterPeriph->getElementByIndex(2));
    ControlRepository::getInstance().addControl(modbusInverter);

    PersistencyManager::getInstance()->serializeConfiguration();
    printf("Obsever Count: %d\n", ElementRepository::getInstance().getObserverCount());

    PersistencyManager::getInstance()->serializeEntity((InputPeripheralBase*)modbusInverterPeriph);
#endif

#define TEST_DESER
#ifdef TEST_DESER
    PersistencyManager::getInstance()->deserializeConfiguration();
    printf("Obsever Count: %d\n", ElementRepository::getInstance().getObserverCount());

    PersistencyManager::getInstance()->deleteAllEntities();
#endif

#endif

//#define ActivationWithFeedbackTest
#ifdef ActivationWithFeedbackTest
    PscMessageHandler::getInstance()->getPsocManager()->initDigitalInputPeripheralByIndex(6, 1, 6);
    PscMessageHandler::getInstance()->getPsocManager()->enablePsoc(6);
    PeripheralRepository::getInstance().initDigitalOutputs(2, 12);

    ActivationWithFeedbackControl signalTest;
    signalTest.setPssId(10);
    ElementBase *tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(2)->getElementByIndex(0);
    tempElement->setPssId(3);
    signalTest.setOutputEnableDevice(tempElement);
    tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(2)->getElementByIndex(1);
    tempElement->setPssId(4);
    signalTest.setOutputDisableDevice(tempElement);
    tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(1)->getElementByIndex(1);
    tempElement->setPssId(5);
    signalTest.addActivateDevice(tempElement, true);
    tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(1)->getElementByIndex(0);
    tempElement->setPssId(6);
    signalTest.addDeactivateDevice(tempElement, true);

    ProtectionControl *protCont = ControlRepository::getInstance().getProtectionControl();
    DeviceProtectionChecker *protChecker = protCont->createDeviceProtectionChecker();

    tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(1)->getElementByIndex(2);
    tempElement->setPssId(7);
    protChecker->setElement(tempElement);
    protChecker->setDebounceTimeout(0);
    protChecker->updateSoftProtectionRange(1, 2, true, false);
    protChecker->updateHardProtectionRange(0, 0, false, false);
    signalTest.addProtectionChecker(protChecker);

    tempElement = PeripheralRepository::getInstance().getPeripheralByPssId(2)->getElementByIndex(5);
    tempElement->setPssId(8);
    signalTest.setFeedbackEnabledDevice(tempElement);


    ControlRepository::getInstance().addControl(&signalTest);

    UpdateSchedulerTask::getInstance()->setBoardInReady(true);

    delay(100);
    signalTest.initControl(0, 0);

#endif

//#define ModbusTest
#ifdef ModbusTest
    CLogger::getInstance().setAllTaskMask(M_LOGGER_LEVEL_TRACE);
    ChangeUsartBaudrate(USART3, 19200);
    ModbusInverterSchneiderAtv32* modbusPeriph = new ModbusInverterSchneiderAtv32(2);
    modbusPeriph->setPssId(1);
//    Modbus8TCPeripheral* modbusPeriph2 = new Modbus8TCPeripheral(3);
//    modbusPeriph2->setUpdateInterval(50);
//    modbusPeriph2->setPssId(2);
//    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)modbusPeriph2);
    PeripheralRepository::getInstance().addPeripheral((InputPeripheralBase*)modbusPeriph);
    UpdateSchedulerTask::getInstance()->setBoardInReady(true);
    ModbusSchedulerTask::getInstance()->setBoardInReady(true);

#endif

    for (;;)
    {
//        usart3_set_dir(true);
//        TM_USART_DMA_Send(USART3, data, 8);
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "test log");
        delay(1000);
    }

}

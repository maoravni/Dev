/*
 * BoardSerializer.h
 *
 *  Created on: 24 Jul 2016
 *      Author: maora
 */

#ifndef BOARDSERIALIZER_H_
#define BOARDSERIALIZER_H_

#include <api/fat_sl.h>
#include <stdint.h>
#include <PscSubsystem.h>
#include "Serializer.h"

class FecBoardConfiguration
{
private:
    E_BoardType m_boardType;
    uint16_t m_boardCableId;
    uint16_t m_numOfPsocs;
    uint16_t m_psocsCableId[10];
    uint32_t m_uartBaudRate;
    uint32_t m_loggerIp;
    uint64_t m_lastConfigurationHash1;
    uint64_t m_lastConfigurationHash2;
    uint64_t m_lastConfigurationTimestamp;

public:
    FecBoardConfiguration();
    virtual ~FecBoardConfiguration();

    void getParametersFromBoard();
    void setParametersToBoard();
    bool compareToCurrentParameters(FecBoardConfiguration& f);
};

template<> class Serializer<FecBoardConfiguration> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return 0;}
    void serialize(F_FILE* f, FecBoardConfiguration &e);
    void deserialize(F_FILE* f, FecBoardConfiguration &e);
};


#endif /* BOARDSERIALIZER_H_ */

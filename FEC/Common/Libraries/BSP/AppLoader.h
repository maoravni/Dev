/*
 * AppLoader.h
 *
 *  Created on: 11 Feb 2015
 *      Author: maora
 */

#ifndef APPLOADER_H_
#define APPLOADER_H_

#include <stdint.h>
#include <cybootloaderutils/cybtldr_parse.h>
#ifndef FEC_BOOTLOADER
#include <Peripherals/PSoC/PsocManager.h>
#endif

using namespace std;

enum E_PacketResult
{
    E_PacketResult_Ok,
    E_PacketResult_FlashWriteFailed,
    E_PacketResult_FlashWriteCompareFailed,
    E_PacketResult_CrcError,
    E_PacketResult_MissingPacket,
};

class AppLoaderBase
{
private:
    uint16_t m_appTarget;

protected:
    uint32_t m_lastMesageId;
    uint32_t m_lastSerialNumber;
    uint32_t m_lastPacketNumber;
    uint32_t m_numOfPackets;

public:
    AppLoaderBase();
    virtual ~AppLoaderBase();

    virtual void startAppLoad(int numOfPackets, uint16_t target, uint32_t msgId, uint32_t sn) = 0;
    virtual E_PacketResult nextPacket(uint32_t packetNumber, uint8_t* data, uint16_t packetLength, uint16_t crc) = 0;
    virtual bool endAppLoad() = 0;
    uint32_t getMsgId() {return m_lastMesageId;}
    uint32_t getSerialNumber() {return m_lastSerialNumber;}

    uint16_t getAppTarget() const
    {
        return m_appTarget;
    }

    void setAppTarget(uint16_t appTarget)
    {
        m_appTarget = appTarget;
    }
};

class StAppLoader : public AppLoaderBase
{
private:
    uint32_t m_lastFlashAddress;

public:
    StAppLoader();
    //virtual ~StAppLoader();

    virtual void startAppLoad(int numOfPackets, uint16_t target, uint32_t msgId, uint32_t sn);
    virtual E_PacketResult nextPacket(uint32_t packetNumber, uint8_t* data, uint16_t packetLength, uint16_t crc);
    virtual bool endAppLoad();
};

struct FlashRowHeader
{
    unsigned short isLast; // this is to align the struct size to 4-byte.
    unsigned short rowNum;
    unsigned short bufSize;
    unsigned char arrayId;
    unsigned char checksum;
};

#ifndef FEC_BOOTLOADER
class PsocAppLoader : public AppLoaderBase
{
private:
    uint8_t m_packetBuffer[MAX_BUFFER_SIZE];
    int m_currentBufferIndex;
    bool m_isFirstLine;
    unsigned long m_siliconId;
    unsigned char m_siliconRev;
    unsigned char m_chksumtype;
    uint32_t m_lastFlashAddress;
    PsocManager* m_pPsocManager;
    bool m_reachedEol;

public:
    PsocAppLoader(PsocManager* psocManager);

    virtual void startAppLoad(int numOfPackets, uint16_t target, uint32_t msgId, uint32_t sn);
    virtual E_PacketResult nextPacket(uint32_t packetNumber, uint8_t* data, uint16_t packetLength, uint16_t crc);
    virtual bool endAppLoad();
private:
    bool isEol(uint8_t c) {return (c == '\n' || c == '\r');}
    int writeRowDataToFlash(bool isLast);
    // TODO: make private
public:
    int writeRowDataFromFlashToPSoC();

    friend class TestTask;
};
#endif

#endif /* APPLOADER_H_ */

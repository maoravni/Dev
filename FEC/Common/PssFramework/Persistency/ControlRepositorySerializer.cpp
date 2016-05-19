/*
 * ControlRepositorySerializer.cpp
 *
 *  Created on: 9 May 2016
 *      Author: maora
 */

#include <Persistency/ControlRepositorySerializer.h>
#include <logger.h>

struct T_ControlRepositoryIndexes
{
    uint16_t m_protectionControl;
    uint16_t m_emergencyInputControl;
    uint16_t m_orderedShutdownControl;
};

void Serializer<ControlRepository>::serialize(F_FILE* f, ControlRepository& cr)
{
    int result;

    storeStartPosition(f);

    serializeVersion(f);

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfControls = 0;

    numOfControls = cr.m_controlList.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d Controls", numOfControls, cr.m_controlList.size());

    M_FWRITE_VARIABLE(numOfControls, f);

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    EntityMapRecord entityMapRecord =
    { 0 };
    for (int i = 0; i < cr.m_controlList.size(); ++i)
        M_FWRITE_VARIABLE(entityMapRecord, f);

    updateRecordSize(f);

    T_EntityMapRecordVector entityMapVec;

    // serialize each of the peripherals:
    for (int i = 0; i < cr.m_controlList.size(); ++i)
    {
        // store the position of the elements in the map record.
        entityMapRecord.pssId = cr.m_controlList[i]->getPssId();
        entityMapRecord.filePos = f_tell(f);
        entityMapVec.push_back(entityMapRecord);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing control %d @ %d", entityMapRecord.pssId,
                entityMapRecord.filePos);

        cr.m_controlList[i]->serialize(f);
    }

    assert(entityMapVec.size() == numOfControls);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";
    for (int i = 0; i < numOfControls; ++i)
    {
        M_FWRITE_VARIABLE(entityMapVec[i], f);
    }

    T_ControlRepositoryIndexes ici;
    ici.m_protectionControl = cr.m_protectionControl->getControlIndex();
    ici.m_emergencyInputControl = cr.m_emergencyInputControl->getControlIndex();
    ici.m_orderedShutdownControl = cr.m_orderedShutdownControl->getControlIndex();

    M_FWRITE_VARIABLE(ici, f);
}

void Serializer<ControlRepository>::deserialize(F_FILE* f, ControlRepository& e)
{
}

void Serializer<ControlRepository>::deserializeControl(F_FILE* f, ControlRepository& e)
{
}


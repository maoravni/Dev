/*
 * JsonHandler.cpp
 *
 *  Created on: 10 Sep 2014
 *      Author: maora
 */

#include "PscServer/JsonHandler.h"

JsonHandler* JsonHandler::m_instance = NULL;

static int BufPrint_sockWrite(BufPrint* o, int sizeRequired)
{
    for (int i = 0; i < sizeRequired; ++i)
        putchar(o->buf[i]);

    return 0; /* OK */
}

JsonHandler::JsonHandler()
{
    m_jErr = new JErr();
    m_bufPrint = new BufPrint(NULL, BufPrint_sockWrite);
    m_jEncoder = new JEncoder(m_jErr, m_bufPrint);
}

JsonHandler::~JsonHandler()
{
    delete m_jEncoder;
    delete m_bufPrint;
    delete m_jErr;
}


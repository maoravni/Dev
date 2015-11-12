/*
 * JsonHandler.h
 *
 *  Created on: 10 Sep 2014
 *      Author: maora
 */

#ifndef JSONHANDLER_H_
#define JSONHANDLER_H_

#include <JEncoder.h>

class JsonHandler
{
private:
    JEncoder *m_jEncoder;
    JErr *m_jErr;
    BufPrint *m_bufPrint;

    static JsonHandler *m_instance;
    JsonHandler();
public:
    virtual ~JsonHandler();

    JsonHandler *getInstance() {if (m_instance == NULL) m_instance = new JsonHandler(); return m_instance;}
};

#endif /* JSONHANDLER_H_ */

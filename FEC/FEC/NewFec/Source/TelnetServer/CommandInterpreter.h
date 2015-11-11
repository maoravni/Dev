/*
 * CommandInterpreter.h
 *
 *  Created on: 16 Jun 2013
 *      Author: maora
 */

#ifndef COMMANDINTERPRETER_H_
#define COMMANDINTERPRETER_H_

class BaseCommand
{
public:
    virtual char* getFormatString() const {return "";}
};

class CommandInterpreter
{
    static const char* m_commandNames[];
    static const char* m_formatString[];

public:
    CommandInterpreter();
    virtual ~CommandInterpreter();

    void interpretCommand(char* command);
    void executeCommand(char* command, int commandIndex);
};

#endif /* COMMANDINTERPRETER_H_ */

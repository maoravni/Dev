/*
 * CommandInterpreter.cpp
 *
 *  Created on: 16 Jun 2013
 *      Author: maora
 */

#include "CommandInterpreter.h"
#include <string.h>
#include <stdio.h>

class GetVersion : public BaseCommand
{
public:
    char boardId;
    virtual char* getFormatString() const {return "GetVersion %d";}
    virtual void parse(const char* command) {sscanf(command, getFormatString(), boardId);}
};


const char *CommandInterpreter::m_commandNames[] = {
        "getTaskList",
        "help",
};

const char *CommandInterpreter::m_formatString[] = {
        "getTaskList",
        "help",
};

CommandInterpreter::CommandInterpreter()
{
}

CommandInterpreter::~CommandInterpreter()
{
}

void CommandInterpreter::interpretCommand(char* command)
{
//    char* tempCommand = command;
//    char* substr;
//
//    // trim leading spaces:
//    while (*tempCommand == 0)
//        ++tempCommand;
//
//    int numOfCommands = sizeof(m_commandNames) / 4;
//
//    //search for the command
//    for (int i = 0; i < numOfCommands; ++i) {
//        substr = strstr(tempCommand, (&m_commands[i])->getFormatString());
//        // if found:
//        if (substr == tempCommand)
//            executeCommand(tempCommand, i);
//    }
}

void CommandInterpreter::executeCommand(char* command, int commandIndex)
{

}

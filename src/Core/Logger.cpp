//
// Created by hindrik on 7-6-17.
//

#include <fstream>
#include <chrono>
#include "Logger.h"

bool Logger::m_logToFile = true;
bool Logger::m_logFileIsOpened = false;

void Logger::log(string msg)
{
    std::stringstream str;
    str << msg << std::endl;
    logToFileAndConsole(str.str(), LogLevel::Information);
}

void Logger::warn(string msg)
{
    std::stringstream str;
    str << msg << std::endl;
    logToFileAndConsole(str.str(), LogLevel::Warning);
}

void Logger::error(string msg)
{
    std::stringstream str;
    str << msg << std::endl;
    logToFileAndConsole(str.str(), LogLevel::Error);

}

void Logger::succes(string msg)
{
    std::stringstream str;
    str << msg << std::endl;
    logToFileAndConsole(str.str(), LogLevel::Succes);
}

void Logger::failure(string msg)
{
    std::stringstream str;
    str << msg << std::endl;
    logToFileAndConsole(str.str(), LogLevel::Failure);
    throw std::runtime_error(msg);
}

void Logger::logNoEndl(string msg)
{
    logToFileAndConsole(msg, LogLevel::Information);
}

void Logger::warnNoEndl(string msg)
{
    logToFileAndConsole(msg, LogLevel::Warning);
}

void Logger::errorNoEndl(string msg)
{
    logToFileAndConsole(msg, LogLevel::Error);
}

void Logger::succesNoEndl(string msg)
{
    logToFileAndConsole(msg, LogLevel::Succes);
}

void Logger::failureNoEndl(string msg)
{
    logToFileAndConsole(msg, LogLevel::Failure);
    throw std::runtime_error(msg);
}

void Logger::logToFileAndConsole(string str, LogLevel e)
{
    time_t currentTime;
    time(&currentTime);
    char timeString[9];
    struct tm* time_info;
    time_info = localtime(&currentTime);
    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
    string timestamp(timeString);

    cout << "[" << timestamp << "]" << tagged(e,str, true);
    if(!m_logToFile) return;

    std::ofstream ofs;

    if(m_logFileIsOpened)
        ofs.open("log.txt", std::ios_base::app);
    else
        ofs.open("log.txt", std::ios_base::trunc);

    if(ofs.is_open())
    {
        if(!m_logFileIsOpened)
        {
            ofs << "This log file has been created on " << ctime(&currentTime) << std::endl;

        }
        ofs << "[" << timestamp << "]" << tagged(e, str);
    }
    else
    {
        cout << "[" << timestamp << "]" << tagged(LogLevel::Error,"Could not open log file!", true);
    }
    m_logFileIsOpened = true;
}
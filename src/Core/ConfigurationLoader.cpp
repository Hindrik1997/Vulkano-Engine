//
// Created by hindrik on 16-6-17.
//

#include <fstream>
#include "ConfigurationLoader.h"
#include "Logger.h"

std::string getCurrentWorkingDir()
{
    char buff[FILENAME_MAX];
    void* c = GetCurrentDir( buff, FILENAME_MAX );
    if(!c)
        Logger::failure("Error getting working directory!");
    std::string current_working_dir(buff);
    return current_working_dir;
}

void ConfigurationLoader::parseFile(string file)
{
    std::ifstream configFile(file);
    std::string line;
    std::regex match("([a-zA-Z]+)\\s*=\\s*([a-zA-Z0-9]+)");

    if(!configFile.is_open())
    {
        Logger::failure("Cannot read config file: " + file);
    }

    while(std::getline(configFile, line))
    {
        std::smatch matchresults;
        if(std::regex_match(line, matchresults, match))
        {
            m_Values.insert(std::pair<string,string>(matchresults[1], matchresults[2]));
        }
    }
    configFile.close();
}

ConfigurationLoader::ConfigurationLoader()
{
    Logger::log("Current working directory is: " + getCurrentWorkingDir());
}

const std::map<string, string>& ConfigurationLoader::map() const
{
    return m_Values;
}

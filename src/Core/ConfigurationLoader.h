//
// Created by hindrik on 16-6-17.
//

#ifndef VULKANOENGINE_CONFIGURATIONMANAGER_H
#define VULKANOENGINE_CONFIGURATIONMANAGER_H

#include <map>
#include <string>
#include <regex>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <bits/ios_base.h>
#include <unistd.h>
#include <cstring>
#include "Logger.h"

#define GetCurrentDir getcwd
#endif

using std::string;

string getCurrentWorkingDir();

class ConfigurationLoader final
{
private:
    std::map<string, string> m_Values;
public:
    ConfigurationLoader();
    ~ConfigurationLoader() = default;

    ConfigurationLoader(const ConfigurationLoader&)   = delete;
    ConfigurationLoader(ConfigurationLoader&&)        = default;

    ConfigurationLoader& operator=(const ConfigurationLoader&)    = delete;
    ConfigurationLoader& operator=(ConfigurationLoader&&)         = default;
public:
    void parseFile(string file);
    const std::map<string,string>& map() const;

};


#endif //VULKANOENGINE_CONFIGURATIONMANAGER_H

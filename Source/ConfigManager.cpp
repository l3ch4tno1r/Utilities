#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

#include "ConfigManager.h"

namespace LCNUtilities
{
	// Accessors

	ConfigManager& ConfigManager::AppSettings()
	{
		static ConfigManager appsettings;
		static std::once_flag flag;

		std::call_once(flag, &ConfigManager::LoadConfig, appsettings);

		return appsettings;
	}

	ConfigManager::Parameter& ConfigManager::operator[](const std::string& key)
	{
		auto it = m_ConfigMap.find(key);

		if (it == m_ConfigMap.end())
			throw std::out_of_range("Cannot find the specified key.");

		return it->second;
	}

	void ConfigManager::LoadConfig()
	{
		std::ifstream configfile("config.txt", std::ios::in);

		if (!configfile)
			throw std::exception("Cannot open config.txt file.");

		std::string line;

		while (std::getline(configfile, line))
		{
			if (line.size() != 0)
			{
				std::stringstream ssdata(line);
				std::string key, value;

				ssdata >> key >> value;

				m_ConfigMap[key].Value(value);
			}
		}
	}

	// Instanciation

	ConfigManager::Parameter::Parameter() :
		value()
	{}

	ConfigManager::Parameter::Parameter(const std::string &astr) :
		value(astr)
	{}
}

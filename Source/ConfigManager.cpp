#include <iostream>
#include <fstream>
#include <sstream>

#include "ConfigManager.h"

namespace LCNUtilities
{
	// Construcor / Destructor
	ConfigManager::ConfigManager()
	{
		std::ifstream configfile("config.txt", std::ios::in);

		if (configfile)
		{
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

			configfile.close();
		}
	}

	ConfigManager::~ConfigManager()
	{}

	// Accessors

	ConfigManager& ConfigManager::AppSettings() noexcept
	{
		static ConfigManager appsettings;
		return appsettings;
	}

	ConfigManager::Parameter& ConfigManager::operator[](const std::string& key)
	{
		auto it = m_ConfigMap.find(key);

		if (it == m_ConfigMap.end())
			throw std::out_of_range("Cannot find the specified key.");

		return it->second;
	}

	// Instanciation

	ConfigManager::Parameter::Parameter() :
		value()
	{}

	ConfigManager::Parameter::Parameter(const std::string &astr) :
		value(astr)
	{}
}

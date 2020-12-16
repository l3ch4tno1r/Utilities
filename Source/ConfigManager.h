#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <unordered_map>
#include <string>
#include <sstream>

namespace LCNUtilities
{
	class ConfigManager
	{
	private:
		class Parameter
		{
		private:
			std::string value;

		public:
			Parameter();

			Parameter(const std::string &astr);

			template<class T>
			T Value() const;

			template<class T>
			void Value(const T &val);
		};

	public:
		static ConfigManager& AppSettings() noexcept;

		Parameter& operator[](const std::string& key);

	private:
		std::unordered_map<std::string, Parameter> m_ConfigMap;

		ConfigManager();

		~ConfigManager();

		ConfigManager(const ConfigManager&) = delete;
		ConfigManager& operator=(ConfigManager&) = delete;
	};

	template<class T>
	inline T ConfigManager::Parameter::Value() const
	{
		T result;
		std::stringstream ss;

		if (value.empty())
			throw std::exception("This parameter is empty.");

		ss << value;
		ss >> result;

		if (ss.fail())
			throw std::exception("Failed to convert parameter to required type.");

		return result;
	}

	template<class T>
	inline void ConfigManager::Parameter::Value(const T &val)
	{
		std::stringstream ss;

		ss << val;
		ss >> value;
	}
}

#endif
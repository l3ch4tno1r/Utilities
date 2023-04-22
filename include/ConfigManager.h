#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <unordered_map>
#include <string>
#include <sstream>

namespace LCN::Utilities
{
	class ConfigManager
	{
	private:
		class Parameter
		{
		public:
			Parameter();

			Parameter(const std::string &astr);

			template<class T>
			T Value() const;

			template<class T>
			void Value(const T &val);

			template<class T>
			inline operator T()
			{
				return Value<T>();
			}

		private:
			std::string m_Value;
		};

	public:
		static ConfigManager& Get();

		Parameter& operator[](const std::string& key);

	private:
		std::unordered_map<std::string, Parameter> m_ConfigMap;

		ConfigManager() = default;
		~ConfigManager() = default;

		ConfigManager(const ConfigManager&) = delete;
		ConfigManager& operator=(ConfigManager&) = delete;

		void LoadConfig();
	};

	template<class T>
	inline T ConfigManager::Parameter::Value() const
	{
		if (m_Value.empty())
			throw std::exception("This parameter is empty.");

		T result;
		std::stringstream ss(m_Value);

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
		ss >> m_Value;
	}
}

#endif
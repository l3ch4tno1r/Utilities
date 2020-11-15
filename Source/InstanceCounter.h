#pragma once

#include <cstdint>

template<typename T>
class Counter
{
public:
	Counter() :
		m_Id(++m_Count)
	{}

	Counter& operator=(const Counter& other)
	{
		return *this;
	}

	uint32_t Id() const
	{
		return m_Id;
	}

	static uint32_t Count()
	{
		return m_Count;
	}

protected:
	~Counter() = default;

private:
	uint32_t m_Id;
	static uint32_t m_Count;
};

template<typename T>
uint32_t Counter<T>::m_Count = 0;
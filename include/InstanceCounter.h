#pragma once

#include <cstdint>
#include <atomic>

template<typename T>
class Counter
{
public:
	Counter& operator=(const Counter& other)
	{
		return *this;
	}

	uint32_t Id() const
	{
		return m_Id;
	}

	static uint32_t CreatedObjects() { return m_CreatedObjects; }

	static uint32_t LiveObjects() { return m_LiveObjects; }

protected:
	Counter() :
		m_Id(++m_CreatedObjects)
	{
		++m_LiveObjects;
	}

	~Counter()
	{
		--m_LiveObjects;
	}

private:
	uint32_t m_Id;

	static std::atomic_uint m_CreatedObjects;
	static std::atomic_uint m_LiveObjects;
};

template<typename T>
std::atomic_uint Counter<T>::m_CreatedObjects = 0;

template<typename T>
std::atomic_uint Counter<T>::m_LiveObjects = 0;
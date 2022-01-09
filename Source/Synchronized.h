#pragma once

#include <type_traits>
#include <mutex>

template<typename _Type>
class Synchronized
{
public:
	using ValType = std::remove_reference<_Type>::type;

	class SyncLock : public std::lock_guard<std::mutex>
	{
	public:
		using BaseType = std::lock_guard<std::mutex>;

		ValType& operator*() { return m_Ref; }

		ValType* operator->() { return &m_Ref; }

	private:
		SyncLock(ValType& ref, std::mutex& mut) :
			m_Ref{ ref },
			BaseType{ mut }
		{}

	private:
		ValType& m_Ref;

		friend Synchronized;
	};

	template<typename ... Args>
	Synchronized(Args&& ... args) :
		m_Data(std::forward<Args>(args)...)
	{}

	SyncLock Lock() { return { m_Data, m_Mut }; }

private:
	_Type m_Data;
	std::mutex m_Mut;
};
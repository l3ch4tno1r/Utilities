#pragma once

#include <type_traits>
#include <mutex>

namespace LCN::Utilities
{
	/**
	 * @class Locker<_ThreadSafe>
	 * @brief Scope based locker class that holds a mutex.
	*/
	template<typename _ThreadSafe>
	class Locker
		: public std::scoped_lock<typename _ThreadSafe::MtxType>
	{
	public:
		using MtxType  = typename _ThreadSafe::MtxType;
		using BaseType = std::scoped_lock<MtxType>;
		using ValType  = typename _ThreadSafe::ValType;
	
	public:
		ValType&
		operator*() const
		{
			return m_target;
		}

		ValType*
		operator->() const
		{
			return &m_target;
		}

	private:
		Locker(ValType& target, MtxType& mutex)
			: m_target{ target }
			, BaseType{ mutex }
		{}

		friend _ThreadSafe;

	private:
		ValType& m_target;
	};

	/**
	 * @class ThreadSafe<_DataType, _MtxType>
	 * @brief Class that holds data in a multi thread environnement.
	*/
	template<
		typename _DataType,
		typename _MtxType = std::mutex>
	class ThreadSafe
	{
	public:
		using ThisType = ThreadSafe<_DataType>;
		using ValType  = std::remove_reference<_DataType>::type;
		using MtxType  = _MtxType;
		using LockType = Locker<ThisType>;

		template<typename ... Args>
		ThreadSafe(Args&& ... args) :
			m_Data(std::forward<Args>(args)...)
		{}

		LockType Lock() { return { m_Data, m_Mut }; }

	private:
		_DataType m_Data;
		_MtxType  m_Mut;
	};

	/**
	 * @brief Convenience function to create a wrapping ThreadSafe object.
	 * @param args Variadic arguments to be passed to the ThreadSafe constructor.
	*/
	template<
		typename _DataType,
		typename _MtxType = std::mutex,
		typename ... _Args>
	ThreadSafe<_DataType, _MtxType>
	MakeThreadSafe(
		_Args&& ... args)
	{
		return { std::forward<_Args>(args)... };
	}
}

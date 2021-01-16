#pragma once

#include <Utilities/Source/DesignPatterns/Observer.h>

#include <functional>

namespace LCN
{
#pragma region Slot
	//////////////////
	//-- SlotBase --//
	//////////////////

	template<class F>
	class SlotBase;

	template<class ...Args>
	class SlotBase<void(Args...)> : public Observer<SlotBase<void(Args...)>>
	{
	public:
		virtual inline void operator()(Args&&...) = 0;

		inline void Update(Args&&... args)
		{
			this->operator()(std::forward<Args>(args)...);
		}

	protected:
		virtual ~SlotBase() = default;
	};

	//////////////
	//-- Slot --//
	//////////////

	template<class O, class F>
	class Slot;

	template<class Owner, class ...Args>
	class Slot<Owner, void(Args...)> : public SlotBase<void(Args...)>
	{
	public:
		inline void operator()(Args&& ...args)
		{
			(m_Owner.*m_Method)(std::forward<Args>(args)...);
		}

	private:
		friend Owner;

		typedef void(Owner::*MethodPtrType)(Args...);

		Slot(Owner& owner, MethodPtrType method) :
			m_Owner(owner),
			m_Method(method)
		{}

		Owner&        m_Owner;
		MethodPtrType m_Method;
	};
#pragma endregion

#pragma region Signal
	////////////////////
	//-- SignalBase --//
	////////////////////

	template<class F>
	class SignalBase;

	template<class ...Args>
	class SignalBase<void(Args...)> : public Subject<SlotBase<void(Args...)>>
	{};

	////////////////
	//-- Signal --//
	////////////////

	template<class O, class F>
	class Signal;

	template<class Owner, class ...Args>
	class Signal<Owner, void(Args...)> : public SignalBase<void(Args...)>
	{
	private:
		inline void Emmit(Args&& ...args)
		{
			this->Notify(std::forward<Args>(args)...);
		}

		friend Owner;
	};
#pragma endregion

#pragma region Utils
	///////////////
	//-- Utils --//
	///////////////

	template<class F>
	void Connect(SignalBase<F>& signal, SlotBase<F>& slot) { signal.AddObserver(slot); }

	#define SLOT_INIT(SlotName, Method) SlotName(*this, &Method)

	#define SLOT(OwnerClass, Method, ...) \
	Slot<OwnerClass, void(__VA_ARGS__)> Slot ## Method;\
	void Method(__VA_ARGS__);

	#define VIRTUAL_SLOT(OwnerClass, Method, ...) \
	Slot<OwnerClass, void(__VA_ARGS__)> Slot ## Method;\
	virtual void Method(__VA_ARGS__);
#pragma endregion
}
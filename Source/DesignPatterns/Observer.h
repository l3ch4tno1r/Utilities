#pragma once

#include <list>
#include <type_traits>
#include <mutex>
#include <exception>

//////////////////////////////
//-- Forward declarations --//
//////////////////////////////

template<class Obs>
class Subject;

//////////////////////
//-- Observer --//
//////////////////////

template<class Obs>
class Observer
{
private:
	using SubjectType = Subject<Obs>;

public:
	virtual ~Observer()
	{
		if (m_Subject)
			m_Subject->RemoveObserver(static_cast<Obs&>(*this));
	}

	void ConnectToSubject(SubjectType* subject)
	{
		if (m_Subject)
			throw std::exception("Already connected !");

		m_Subject = subject; 
	};

	void DisconnectFromSubject() { m_Subject = nullptr; }

private:
	SubjectType* m_Subject = nullptr;
};

/////////////////
//-- Subject --//
/////////////////

template<class Obs>
class Subject
{
	static_assert(std::is_base_of<Observer<Obs>, Obs>::value, "Obs must inherit from Observer<Obs>");

public:
	virtual ~Subject()
	{
		std::lock_guard<std::mutex> lock(m_ObserversMut);

		for (Obs* obs : m_Observers)
			obs->DisconnectFromSubject();
	}

	void AddObserver(Obs& obs)
	{
		std::lock_guard<std::mutex> lock(m_ObserversMut);

		obs.ConnectToSubject(this);

		m_Observers.push_back(&obs);
	}

	void RemoveObserver(Obs& obs)
	{
		std::lock_guard<std::mutex> lock(m_ObserversMut);

		obs.DisconnectFromSubject();

		m_Observers.remove_if([&obs](const Obs* value)
		{
			return value == &obs;
		});
	}

	template<typename ...Args>
	inline void Notify(Args&& ...args)
	{
		std::lock_guard<std::mutex> lock(m_ObserversMut);

		for (Obs* obs : m_Observers)
			obs->Update(std::forward<Args>(args)...);
	}

protected:
	std::list<Obs*> m_Observers;
	std::mutex      m_ObserversMut;
};
#pragma once

#include <vector>
#include <memory>

#include "../ErrorHandling.h"

namespace LCN
{
	//////////////////////
	//-- EntityHandle --//
	//////////////////////

	class EntityHandle
	{
	public:
		EntityHandle() = default;

	private:
		int m_Id = -1;

		EntityHandle(int id) :
			m_Id(id)
		{}

		friend class EntityManager;
		friend class EntityComponentMap;

		template<class Component>
		friend class ComponentPool;
	};
	
	///////////////////////
	//-- Type indexing --//
	///////////////////////

	template<typename Type>
	class TypeIndexer;

	class TypeAutoInc
	{
	private:
		static size_t Next()
		{
			static size_t i = 0;
			return i++;
		}

		template<typename Type>
		friend class TypeIndexer;
	};

	template<typename Type>
	class TypeIndexer
	{
	public:
		static size_t Index()
		{
			static size_t idx = TypeAutoInc::Next();
			return idx;
		}
	};

	////////////////////////
	//-- Component pool --//
	////////////////////////

	class ComponentPoolBase
	{
	public:
		virtual ~ComponentPoolBase() = default;
	};

	template<class Component>
	class ComponentPool : public ComponentPoolBase
	{
	private:
		friend EntityManager;

		std::vector<Component> m_Components;

		size_t Size() const { return m_Components.size(); }

		template<class ...Args>
		Component& Add(Args&& ...args)
		{
			return m_Components.emplace_back(std::forward<Args>(args)...);
		}

		Component& Get(size_t componentidx)
		{
			return m_Components[componentidx];
		}
	};

	////////////////////////////////
	//-- Entity / Component map --//
	////////////////////////////////

	class EntityComponentMap
	{
	private:
		using MapType = std::vector<std::vector<int>>;

		MapType m_Map;

		friend EntityManager;

		template<class Component>
		void MapEntityComponent(EntityHandle entity, size_t componentid)
		{
			if (m_Map.size() <= entity.m_Id)
				m_Map.resize(entity.m_Id + 1, std::vector<int>());

			std::vector<int>& components = m_Map[entity.m_Id];

			size_t componenttypeidx = TypeIndexer<Component>::Index();

			if (components.size() <= componenttypeidx)
				components.resize(componenttypeidx + 1, -1);

			components[componenttypeidx] = componentid;
		}

		template<class Component>
		int GetComponentId(EntityHandle entity) const
		{
			size_t cmptypeidx = TypeIndexer<Component>::Index();

			ASSERT(entity.m_Id < m_Map.size());
			ASSERT(cmptypeidx < m_Map[entity.m_Id].size());

			return m_Map[entity.m_Id][cmptypeidx];
		}
	};

	///////////////////////
	//-- EntityManager --//
	///////////////////////

	class EntityManager
	{
	public:
		EntityHandle CreateEntity()
		{
			EntityHandle entity(m_Entities.size());

			m_Entities.push_back(entity);

			return entity;
		}

		template<class Component, class ...Args>
		Component& Add(EntityHandle entity, Args&& ...args)
		{
			size_t componenttypeidx = TypeIndexer<Component>::Index();

			if (componenttypeidx >= m_ComponentPools.size())
			{
				m_ComponentPools.resize(componenttypeidx + 1);
				m_ComponentPools[componenttypeidx] = std::make_shared<ComponentPool<Component>>();
			}

			ComponentPool<Component>& pool = static_cast<ComponentPool<Component>&>(*m_ComponentPools[componenttypeidx]);

			Component& newcomp = pool.Add(std::forward<Args>(args)...);

			m_Map.MapEntityComponent<Component>(entity, pool.Size() - 1);

			return newcomp;
		}

		template<class Component>
		Component& Get(EntityHandle entity) const
		{
			size_t componenttypeidx = TypeIndexer<Component>::Index();
			int    componentidx     = m_Map.GetComponentId<Component>(entity);

			ASSERT(componentidx >= 0);
			ASSERT(componenttypeidx < m_ComponentPools.size());

			ComponentPool<Component>& pool = static_cast<ComponentPool<Component>&>(*m_ComponentPools[componenttypeidx]);

			return pool.Get(componentidx);
		}

	private:
		using ComponentPoolBasePtr = std::shared_ptr<ComponentPoolBase>;

		std::vector<EntityHandle>         m_Entities;
		std::vector<ComponentPoolBasePtr> m_ComponentPools;

		EntityComponentMap m_Map;
	};
}
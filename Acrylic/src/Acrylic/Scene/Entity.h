#pragma once

#include "Scene.h"

#include <entt.hpp>


namespace Acrylic {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		virtual ~Entity() = default;

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return  m_EntityHandle; }
		operator uint32_t() const { return  (uint32_t)m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			AC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have the provided component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			AC_CORE_ASSERT(!HasComponent<T>(), "Entity already has the provided component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& RemoveComponent()
		{
			AC_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have the provided component!");
			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;  // TODO: Use weak_ref
	};

}
#include "acpch.h"
#include "Scene.h"

#include "Acrylic/Scene/Components.h"
#include "Acrylic/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"


namespace Acrylic {

	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "New Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group) {
			auto& [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, spriteRenderer.Color);
		}
	}

}
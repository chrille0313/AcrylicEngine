#include "acpch.h"
#include "Scene.h"

#include "Acrylic/Scene/Components.h"
#include "Acrylic/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"


namespace Acrylic {

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "New Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nativeScriptComponent)
			{
				if (!nativeScriptComponent.Instance) {
					nativeScriptComponent.Instance = nativeScriptComponent.InstantiateScript();
					nativeScriptComponent.Instance->m_Entity = Entity { entity, this };
					nativeScriptComponent.Instance->OnCreate();
				}

				nativeScriptComponent.Instance->OnUpdate(ts);
			});
		}


		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform;
		{
			auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);

			for (auto entity : group) {
				auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);

				if (camera.Primary) {
					mainCamera = &camera.Camera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}

		{
			if (mainCamera) {
				Renderer2D::BeginScene(*mainCamera, *cameraTransform);

				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group) {
					auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entity);
				}

				Renderer2D::EndScene();
			}
		}

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group) {
			auto [transform, spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), spriteRenderer, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);

			if (camera.Primary)
				return Entity { entity, this };
		}

		return {};
	}


	template<typename T>
	inline void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

}
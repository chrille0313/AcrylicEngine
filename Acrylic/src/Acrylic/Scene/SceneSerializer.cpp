#include "acpch.h"
#include "SceneSerializer.h"

#include "Acrylic/Scene/Entity.h"
#include "Acrylic/Scene/Components.h"

#include <fstream>

#include <yaml-cpp/yaml.h>


namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}

	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Acrylic {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& Scene) : m_Scene(Scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "1234567890";  // TODO: Add EntityID

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			out << YAML::Key << "TagComponent";

			out << YAML::BeginMap; // TagComponent
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap; // End TagComponent
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& transform = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "TransformComponent";

			out << YAML::BeginMap; // TransformComponent
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::EndMap; // End TransformComponent
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
			out << YAML::EndMap; // End Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap; // End CameraComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriterendererCommponent

			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap; // SpriteRendererComponent
		}

		out << YAML::EndMap; // End Entity
	}


	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Unnamed";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };

			if (!entity)
				return;

			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}


	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		AC_CORE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		AC_CORE_TRACE("Deserializing scene '{0}'", sceneName);


		if (auto entities = data["Entities"]) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name;

				if (auto serializedTC = entity["TagComponent"]) {
					name = serializedTC["Tag"].as<std::string>();
				}

				AC_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				if (auto serializedTC = entity["TransformComponent"]) {
					auto& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
					transformComponent.Position = serializedTC["Position"].as<glm::vec3>();
					transformComponent.Rotation = serializedTC["Rotation"].as<glm::vec3>();
					transformComponent.Scale = serializedTC["Scale"].as<glm::vec3>();
				}

				if (auto serializedCC = entity["CameraComponent"]) {
					if (serializedCC) {
						auto& cameraComponent = deserializedEntity.AddComponent<CameraComponent>();

						auto& camera = serializedCC["Camera"];
						cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());

						cameraComponent.Camera.SetPerspectiveFOV(camera["PerspectiveFOV"].as<float>());
						cameraComponent.Camera.SetPerspectiveNear(camera["PerspectiveNear"].as<float>());
						cameraComponent.Camera.SetPerspectiveFar(camera["PerspectiveFar"].as<float>());

						cameraComponent.Camera.SetOrthographicSize(camera["OrthographicSize"].as<float>());
						cameraComponent.Camera.SetOrthographicNear(camera["OrthographicNear"].as<float>());
						cameraComponent.Camera.SetOrthographicFar(camera["OrthographicFar"].as<float>());

						cameraComponent.Primary = serializedCC["Primary"].as<bool>();
						cameraComponent.FixedAspectRatio = serializedCC["FixedAspectRatio"].as<bool>();
					}
				}

				if (auto serializedSRC = entity["SpriteRendererComponent"]) {
					auto& spriteRendererComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();

					spriteRendererComponent.Color = serializedSRC["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}

	bool  SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		AC_CORE_ASSERT(false);
		return false;
	}

}

#pragma once

#include "Acrylic/Scene/Scene.h"


namespace Acrylic {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& Scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};

}

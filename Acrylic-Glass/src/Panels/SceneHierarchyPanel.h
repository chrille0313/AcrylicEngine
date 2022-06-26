#pragma once

#include "EditorLayerPanel.h"

#include "Acrylic/Scene/Scene.h"
#include "Acrylic/Core/Base.h"
#include "Acrylic/Core/Log.h"
#include "Acrylic/Scene/Entity.h"


namespace Acrylic {

	class SceneHierarchyPanel : public EditorLayerPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		virtual ~SceneHierarchyPanel() = default;

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRenderInternal();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
	};

}
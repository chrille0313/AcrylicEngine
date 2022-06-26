#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include "Acrylic/Scene/Components.h"
#include <imgui/imgui_internal.h>


namespace Acrylic {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRenderInternal()
	{
		static ImGuiTextFilter filter;
		filter.Draw("Search");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity { entityID, m_Context.get() };

			if (filter.PassFilter(entity.GetComponent<TagComponent>().Tag.c_str()))
				DrawEntityNode(entity);
		});

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			m_SelectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::MenuItem("Create New Empty Entity"))
				m_Context->CreateEntity();

			ImGui::EndPopup();
		}

		ImGui::Begin("Properties");
		if (m_SelectedEntity) {
			DrawComponents(m_SelectedEntity);
		}

		ImGui::End();
	}

	template<typename Component, typename UIFunc>
	static void DrawComponent(const std::string& name, Entity entity, UIFunc uiFunc, bool removable = true)
	{
		if (entity.HasComponent<Component>()) {
			auto& component = entity.GetComponent<Component>();

			const ImGuiTreeNodeFlags treeNodeFlags =
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_AllowItemOverlap;

			if (ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, name.c_str())) {
				uiFunc(component);

				ImGui::TreePop();
			}

			ImGui::Spacing();
			ImGui::Separator();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (entityDeleted) {
			m_Context->DestroyEntity(entity);

			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
		}

	}

	struct ButtonColors
	{
		ImVec4 Default, Hovered, Active;
		ButtonColors(ImVec4 normal, ImVec4 hovered, ImVec4 active) : Default(normal), Hovered(hovered), Active(active) {}
	};

	static void DrawControl(const std::string& label, ImVec2 buttonSize, float& value, float resetValue, const ButtonColors& colors)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, colors.Default);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors.Hovered);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors.Active);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });

		if (ImGui::Button(label.c_str(), buttonSize))
			value = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(("##" + label).c_str(), &value, std::max(std::abs(value * 0.01f), 0.01f));

		ImGui::PopStyleVar();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 90.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 6, 0 });


		// X

		ButtonColors xColors({ 0.8f, 0.1f, 0.15f, 1.0f }, { 0.9f, 0.2f, 0.2f, 1.0f }, { 0.8f, 0.1f, 0.15f, 0.75f });
		DrawControl("X", buttonSize, values.x, resetValue, xColors);

		ImGui::PopItemWidth();
		ImGui::SameLine();


		// Y

		ButtonColors yColors({ 0.2f, 0.7f, 0.2f, 1.0f }, { 0.3f, 0.8f, 0.3f, 1.0f }, { 0.2f, 0.7f, 0.2f, 0.75f });
		DrawControl("Y", buttonSize, values.y, resetValue, yColors);

		ImGui::PopItemWidth();
		ImGui::SameLine();


		// Z

		ButtonColors zColors({ 0.1f, 0.25f, 0.8f, 1.0f }, { 0.2f, 0.35f, 0.9f, 1.0f }, { 0.1f, 0.25f, 0.8f, 0.75f });
		DrawControl("Z", buttonSize, values.z, resetValue, zColors);

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}

		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("+"))
			ImGui::OpenPopup("AddComponent");

		ImGui::Separator();

		if (ImGui::BeginPopup("AddComponent")) {
			if (ImGui::MenuItem("Camera")) {
				m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("SpriteRenderer")) {
				m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Position", component.Position);
			DrawVec3Control("Rotation", component.Rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionType = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionType)) {

				for (int i = 0; i < sizeof(projectionTypeStrings) / sizeof(const char*); i++) {
					bool isSelected = currentProjectionType == projectionTypeStrings[i];

					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionType = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				float fov = camera.GetPerspectiveFOV();
				if (ImGui::DragFloat("FOV", &fov))
					camera.SetPerspectiveFOV(fov);

				float nearClip = camera.GetPerspectiveNear();
				if (ImGui::DragFloat("Near", &nearClip))
					camera.SetPerspectiveNear(nearClip);

				float farClip = camera.GetPerspectiveFar();
				if (ImGui::DragFloat("Far", &farClip))
					camera.SetPerspectiveFar(farClip);
			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float nearClip = camera.GetOrthographicNear();
				if (ImGui::DragFloat("Near", &nearClip))
					camera.SetOrthographicNear(nearClip);

				float farClip = camera.GetOrthographicFar();
				if (ImGui::DragFloat("Far", &farClip))
					camera.SetOrthographicFar(farClip);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});

	}

}
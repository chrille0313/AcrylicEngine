#include "acpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Acrylic {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		float oldAspectRatio = m_AspectRatio;
		m_AspectRatio = (float)width / (float)height;

		if (m_AspectRatio != oldAspectRatio)
			RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		float oldFOV = m_PerspectiveFOV;
		float oldNear = m_PerspectiveNear;
		float oldFar = m_PerspectiveFar;

		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		if (m_PerspectiveFOV != oldFOV || m_PerspectiveNear != oldNear || m_PerspectiveFar != oldFar)
			RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		float oldSize = m_OrthographicSize;
		float oldNear = m_OrthographicNear;
		float oldFar = m_OrthographicFar;

		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		if (m_OrthographicSize != oldSize || m_OrthographicNear != oldNear || m_OrthographicFar != oldFar)
			RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_ViewProjection = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else if (m_ProjectionType == ProjectionType::Orthographic) {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ViewProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

}
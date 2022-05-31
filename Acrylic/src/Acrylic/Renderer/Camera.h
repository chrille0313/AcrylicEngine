#pragma once

#include <glm/glm.hpp>


namespace Acrylic {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_ViewProjection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_ViewProjection; }

	protected:
		glm::mat4 m_ViewProjection = glm::mat4(1.0f);
	};

}
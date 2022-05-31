#pragma once

#include <glm/glm.hpp>

#include "Acrylic/Core/KeyCodes.h"
#include "Acrylic/Core/MouseCodes.h"

namespace Acrylic {

	class Input
	{
	public:

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
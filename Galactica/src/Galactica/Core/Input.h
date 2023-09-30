#pragma once

#include "Galactica//Core/KeyCodes.h"
#include "Galactica/Core/MouseCodes.h"

//#include <glm/glm.hpp>

namespace Galactica {

	class GALACTICA_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsKeyReleased(KeyCode key);

		static bool IsKeyHeld(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);

		/*static glm::vec2 GetMousePosition();

		static float GetMouseX();

		static float GetMouseY();*/
	};
}
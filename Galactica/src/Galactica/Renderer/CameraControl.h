#pragma once

//#include "Galactica/Core/Timestep.h"
#include "Galactica/Events/Event.h"
#include "Galactica/Events/MouseEvent.h"

#include "Galactica/Vendor/glm/glm/glm.hpp"

#include "Camera.h"
#include "Galactica/Core/Timestep.h"
#include "Galactica/Events/ApplicationEvent.h"

namespace Galactica
{
	class GALACTICA_API CameraControl : public Camera
	{

	public:
		CameraControl() = default;

		CameraControl(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(float ts);

		void OnEvent(Event& e);

		inline float GetDistance() const { return cameraDistance; }

		inline void SetDistance(float distance) { cameraDistance = distance; }

		void SetViewportSize(float width, float height);

		const glm::mat4& GetViewMatrix() const { return cameraViewMatrix; }

		glm::mat4 GetViewProjection() const { return cameraProjection * cameraViewMatrix; }

		glm::vec3 GetUpDirection() const;

		glm::vec3 GetRightDirection() const;

		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetPosition() const { return cameraPosition; }

		glm::quat GetOrientation() const;

		float GetPitch() const { return cameraPitch; }

		float GetYaw() const { return cameraYaw; }

	private:
		void UpdateProjection();

		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		bool OnWindowResized(WindowResizeEvent& e);

		void MousePan(const glm::vec2& delta);

		void MouseRotate(const glm::vec2& delta);

		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;

		float RotationSpeed() const;

		float ZoomSpeed() const;
	
		float cameraFOV = 45.0f;
		float cameraAspectRatio = 1.778f;
		float cameraNearClip = 0.1f;
		float cameraFarClip = 1000.0f;

		glm::mat4 cameraViewMatrix;

		glm::vec3 cameraPosition = { 0.0f, 0.0f, 0.0f };

		glm::vec3 cameraFocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 cameraInitialMousePosition = { 0.0f, 0.0f };

		float cameraDistance = 25.0f;
		float cameraPitch = 0.3f;
		float cameraYaw = 0.0f;
		float cameraViewportWidth = 1280;
		float cameraViewportHeight = 720;
	};

}




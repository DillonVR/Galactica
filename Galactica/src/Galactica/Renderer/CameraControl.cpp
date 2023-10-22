#include "glpch.h"

#include "CameraControl.h"
#include "Galactica/Core/Input.h"

#include "GLFW/glfw3.h"
#include "Galactica/Vendor/glm/glm/gtx/quaternion.hpp"

namespace Galactica
{
	CameraControl::CameraControl(float fov, float aspectRatio, float nearClip, float farClip)
		:	cameraFOV(fov),
			cameraAspectRatio(aspectRatio),
			cameraNearClip(nearClip),
			cameraFarClip(farClip)
	{
		UpdateView();
	}

	void CameraControl::OnUpdate(Timestep ts)
	{

		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - cameraInitialMousePosition ) * 0.003f;
			cameraInitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		if (Input::IsKeyPressed(Key::KP4))
			cameraFocalPoint += GetRightDirection() * 0.01f * cameraDistance;

		if (Input::IsKeyPressed(Key::KP6))
			cameraFocalPoint += -GetRightDirection() * 0.01f * cameraDistance;

		if (Input::IsKeyPressed(Key::KPAdd))
		{
			cameraDistance -= ts * ZoomSpeed();
		}

		if (Input::IsKeyPressed(Key::KPSubtract))
		{
			cameraDistance += ts * ZoomSpeed();
		}

		if (Input::IsKeyPressed(Key::KP0))
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			cameraYaw -= yawSign * 0.03f * RotationSpeed();
		}

		if (Input::IsKeyPressed(Key::KPDecimal))
		{
			float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			cameraYaw += yawSign * 0.03f * RotationSpeed();
		}

		if (Input::IsKeyPressed(Key::KP2))
			cameraFocalPoint += GetUpDirection() * 0.01f * cameraDistance;

		if (Input::IsKeyPressed(Key::KP8))
			cameraFocalPoint += -GetUpDirection() * 0.01f * cameraDistance;

		if (Input::IsKeyPressed(Key::KP5))
		{
			cameraPosition = { 0.0f, 0.0f, 0.0f };
			cameraFocalPoint = { 0.0f, 0.0f, 0.0f };
			cameraDistance = 7.0f;
			cameraPitch = 0.3f;
			cameraYaw = 0.0f;
		}
			

		UpdateView();
		UpdateProjection();
	}

	void CameraControl::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GL_BIND_EVENT_FN(CameraControl::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(GL_BIND_EVENT_FN(CameraControl::OnWindowResized));
	}

	void CameraControl::SetViewportSize(float width, float height)
	{
		cameraViewportWidth = width;
		//printf("\nwidth %f", width);
		cameraViewportHeight = height;
		//printf("\nheight %f", height);
		 
	}

	glm::vec3 CameraControl::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 CameraControl::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 CameraControl::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat CameraControl::GetOrientation() const
	{
		return glm::quat(glm::vec3(-cameraPitch, -cameraYaw, 0.0f));
	}

	void CameraControl::UpdateProjection()
	{
		cameraAspectRatio = cameraViewportWidth / cameraViewportHeight;
		cameraProjection = glm::perspective(glm::radians(cameraFOV), cameraAspectRatio, cameraNearClip, cameraFarClip);
	}

	void CameraControl::UpdateView()
	{
		cameraPosition = CalculatePosition();

		glm::quat orientation = GetOrientation();
		cameraViewMatrix = glm::translate(glm::mat4(1.0f), cameraPosition) * glm::toMat4(orientation);
		cameraViewMatrix = glm::inverse(cameraViewMatrix);
	}

	bool CameraControl::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() *  0.1f;
		MouseZoom(delta);
		UpdateView();

		return false;
	}

	void CameraControl::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		cameraFocalPoint += -GetRightDirection() * delta.x * xSpeed * cameraDistance;
		cameraFocalPoint += GetUpDirection() * delta.y * ySpeed * cameraDistance;

	}

	void CameraControl::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		cameraYaw += yawSign * delta.x * RotationSpeed();
		cameraPitch += delta.y * RotationSpeed();
	}

	void CameraControl::MouseZoom(float delta)
	{
		cameraDistance -= delta * ZoomSpeed();
		printf("delta = %f",cameraDistance);
		if (cameraDistance < 1.0f)
		{
			cameraFocalPoint += GetForwardDirection();
			cameraDistance = 1.0f;
		}
	}

	glm::vec3 CameraControl::CalculatePosition() const
	{

		return cameraFocalPoint - GetForwardDirection() * cameraDistance;
	}

	std::pair<float, float> CameraControl::PanSpeed() const
	{
		float x = std::min(cameraViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(cameraViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float CameraControl::RotationSpeed() const
	{
		return 0.8f;
	}

	float CameraControl::ZoomSpeed() const
	{
		float distance = cameraDistance * 0.2f;

		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100

		return speed;
	}

	bool CameraControl::OnWindowResized(WindowResizeEvent& e)
	{

		SetViewportSize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
		return false;
	}
}

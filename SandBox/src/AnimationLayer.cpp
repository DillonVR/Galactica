#include "glpch.h"

#include "AnimationLayer.h"

#include "Galactica/Renderer/Renderer.h"
#include "Galactica/Vendor/imgui/imgui.h"

	
static Galactica::Model* light;
static std::vector<Galactica::Model*> scene;

	AnimationLayer::AnimationLayer()
	: Layer("Example"), camera(45.0f, 1, 0.1f, 1000)
{
		renderer->Setup("Shaders/shader.vert", "Shaders/shader.frag");

		// Create a light
		light = new Galactica::Model("assets/cube.obj");
		light->pos = glm::vec3(0, 100, 0);
		light->color = glm::vec3(1.0f, 1.0f, 1.0f);
		light->scale = glm::vec3(0.25, 0.25, 0.25);

		
		plane = new Galactica::Model("assets/plane.obj");
		plane->color = glm::vec3(0.21, 0.88, 0.16);
		plane->pos = glm::vec3(0.0f, -0.0f, 0.0f);
		plane->scale = glm::vec3(100.0, -1.0, 100.0);
		scene.push_back(plane);

		// Create soft body with cube model
		softBody = new Galactica::SoftBody("assets/Jello/cube.obj", 0.5, 1, 5, 0.1);
		softBody->color = glm::vec3(0.82, 0.35, 0.15);
		softBody->pos = glm::vec3(0, 5.0, 0.0);
		softBody->scale = glm::vec3(3);
		scene.push_back(softBody);

}

	void AnimationLayer::OnUpdate(Galactica::StepTimer const& timer)
	{

		float ts = static_cast<float> (timer.GetElapsedSeconds());
		
		camera.OnUpdate(ts);

		light->pos = glm::vec3(0, 10, 0);

		//Gravity
		softBody->AddForce(glm::vec3(0.0, -9.81, 0.0));
		
		softBody->Update(ts);

		renderer->Draw(light, scene,&camera, debugMode);

	}

	void AnimationLayer::OnImGuiRender()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Settings");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("SoftBodies");
		/*if (ImGui::Button("Donut"))
		{
			delete softBody;

			softBody = new Galactica::SoftBody("assets/Jello/donut.obj", 0.5, 1, 5, 0.1);
			softBody->color = glm::vec3(0.82, 0.35, 0.15);
			softBody->pos = glm::vec3(0, 5.0, 0.0);
			softBody->scale = glm::vec3(3);

			scene.pop_back();
			scene.push_back(softBody);
			
		}
		if (ImGui::Button("cube"))
		{
			delete softBody;

			softBody = new Galactica::SoftBody("assets/Jello/cube.obj", 0.5, 1, 5, 0.1);
			softBody->color = glm::vec3(0.82, 0.35, 0.15);
			softBody->pos = glm::vec3(0, 5.0, 0.0);
			softBody->scale = glm::vec3(3);

			scene.pop_back();
			scene.push_back(softBody);
			
		}*/
		ImGui::Text("Commands");
		// IamGui button
		if (ImGui::Button("Jump"))
		{
			softBody->AddForce(glm::vec3(0, 250, 0));
		}
		if (ImGui::Button("Right"))
		{
			softBody->AddForce(glm::vec3(50, 0, 0));
		}
		if (ImGui::Button("Left"))
		{
			softBody->AddForce(glm::vec3(-50, 0, 0));
		}
		ImGui::Checkbox("Debug", &debugMode);

		ImGui::Text("Spring Count %.3d", softBody->GetSpringCount());
		
		ImGui::End();
	}

	void AnimationLayer::OnEvent(Galactica::Event& event)
	{
		camera.OnEvent(event);
	}

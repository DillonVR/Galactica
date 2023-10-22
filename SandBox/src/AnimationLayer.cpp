#include "glpch.h"

#include "AnimationLayer.h"

#include <GL/gl.h>

#include "Galactica/Vendor/glm/glm/glm.hpp"
#include "Galactica/Vendor/glm/glm/gtx/transform.hpp"
#include "Galactica/Vendor/imgui/imgui.h"

	AnimationLayer::AnimationLayer()
		:	Layer("Example"),
			AnimationShader("Shaders/animationShader.vert", "Shaders/animationShader.frag"),
			ModelShader("Shaders/modelShader.vert", "Shaders/modelShader.frag"),
			camera(45.0f, 1280.0f / 720.0f, 0.1f, 1000)
	{
		int size = 10;

		// create vertices and indices for a checked floor
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				// create 4 vertices for each quad
				Galactica::Vertex v1, v2, v3, v4;

				v1.Position = glm::vec3(i, 0, j);
				v2.Position = glm::vec3(i + 1, 0, j);
				v3.Position = glm::vec3(i + 1, 0, j + 1);
				v4.Position = glm::vec3(i, 0, j + 1);

				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);
				vertices.push_back(v4);

				// create indices
				int start = (i * size + j) * 4;
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + 2);
				indices.push_back(start + 2);
				indices.push_back(start + 3);
				indices.push_back(start);
			}
		}

		Galactica::Texture texture;
		texture.id = 0;
		texture.path = "assets/Materials/Floor.jpg";
		textures.push_back(texture);

		//int floor mesh
		floormesh.lineMesh(vertices, indices, textures);

		//load the models
		ourModel.LoadModel("assets/Paladin/Paladin.dae", false);
		backpackModel.LoadModel("assets/backpack/backpack.obj", false);

		//load the animations
		animation.LoadAnimation("assets/Animations/SillyDancing.dae", &ourModel);
		Walking.LoadAnimation("assets/Animations/Walking.dae", &ourModel);
		// dWalk.LoadAnimation("assets/Animations/Drunk Walk.dae", &ourModel);
		strafe.LoadAnimation("assets/Animations/Strafe.dae", &ourModel);

		animator.Play(&Walking);
	}

	void AnimationLayer::OnUpdate(Galactica::Timestep ts)
	{
		camera.OnUpdate(ts);

		animator.UpdateAnimation(ts);

		const glm::mat4 projection = camera.GetViewProjection();
		
		
		//animation draw
		AnimationShader.Use();
		AnimationShader.setMat4("projectionView", projection);

		auto model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		AnimationShader.setMat4("animation", model);
		//auto bonemap = ourModel.boneMap;

		const auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			AnimationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		if (showSkin)
			ourModel.DrawModel(AnimationShader);

		//model draw
		ModelShader.Use();

		ModelShader.setMat4("projection", projection);

		auto Backpack = glm::mat4(1.0f);
		Backpack = glm::translate(Backpack, glm::vec3(3.0f, 1.0f, 0.0f));
		Backpack = glm::scale(Backpack, glm::vec3(0.2f, 0.2f, 0.2f));

		ModelShader.setMat4("model", Backpack);

		backpackModel.DrawModel(ModelShader);

		//Draw floor
		floormesh.renderline(true);
		float angleInDegrees = 180.0f;
		float angleInRadians = glm::radians(angleInDegrees);

		auto floor = glm::mat4(1.0f);
		floor = glm::translate(floor, glm::vec3(-10.0f, 0.0f, 20.0f));
		floor = glm::scale(floor, glm::vec3(3.0f, 1.0f, 3.0f));
		floor = glm::rotate(floor, angleInRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		ModelShader.setMat4("model", floor);

		floormesh.DrawMesh(ModelShader);
		floormesh.renderline(false);
		
	}

	void AnimationLayer::OnImGuiRender()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Settings");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Checkbox("Bones", &showBones);
		ImGui::Checkbox("Skin", &showSkin);

		const char* items[] = {"Walking","Dancing","Strafe"};
		static int item_current_idx = 0; // Here we store our selection data as an index.
		static int item_last_idx = 0;
		
		if (ImGui::BeginListBox("Animations"))
		{
			if (item_current_idx != item_last_idx)
			{
				flag = true;
				item_last_idx = item_current_idx;
			}
				
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				if(item_current_idx == 0 && flag == true)
				{
					animator.Play(&Walking);
					flag = false;
				}

				if(item_current_idx == 1 && flag == true)
				{
					animator.Play(&animation);
					flag = false;
				}

				if (item_current_idx == 2 && flag == true)
				{
					animator.Play(&strafe);
					flag = false;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
					
			}
			ImGui::EndListBox();
		}
		ImGui::End();
	}

	void AnimationLayer::OnEvent(Galactica::Event& event)
	{
		camera.OnEvent(event);
	}

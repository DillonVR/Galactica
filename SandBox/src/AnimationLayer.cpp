#include "glpch.h"

#include "AnimationLayer.h"

#include <GL/gl.h>

#include "Galactica/Vendor/glm/glm/glm.hpp"
#include "Galactica/Vendor/glm/glm/gtx/transform.hpp"
#include "Galactica/Vendor/imgui/imgui.h"

	AnimationLayer::AnimationLayer()
	: Layer("Example"),
	AnimationShader("Shaders/animationShader.vert", "Shaders/animationShader.frag"),
	ModelShader("Shaders/modelShader.vert", "Shaders/modelShader.frag"),
	LineShader("Shaders/lineShader.vert", "Shaders/lineShader.frag"),
	camera(45.0f, 1, 0.1f, 1000),
	path(0)

{
	int size = 10;

	// create vertices and indices for a checked floor
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			// create 4 vertices for each quad
			Galactica::Vertex v1, v2, v3, v4, v5, v6, v7, v8;

			v1.Position = glm::vec3(i, 0, j);
			v2.Position = glm::vec3(i + 1, 0, j);
			v3.Position = glm::vec3(i + 1, 0, j + 1);
			v4.Position = glm::vec3(i, 0, j + 1);

			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);
			vertices.push_back(v4);

			// // create indices
			int start = (i * size + j) * 4;
			indices.push_back(start);
			indices.push_back(start + 1);
			indices.push_back(start + 2);
			indices.push_back(start + 2);
			indices.push_back(start + 3);
			indices.push_back(start);
		}
	}

	//int floor mesh

	Galactica::Texture floorTexture;
	floorTexture.id = 2;
	floorTexture.path = "assets/Materials/Floor.jpg";
	floorTexture.type = "texture_diffuse";

	textures.push_back(floorTexture);
	
	floormesh.TexturedMesh(vertices, indices, textures);

	//load the models
	GL_LOGGER_INFO("----------------------Loading model--------------------");
	GL_LOGGER_INFO("Loading model Solider");

	ourModel.LoadModel("assets/Soldier/Soldier.dae", false);
	ball.LoadModel("assets/Ball.obj",false);
	GL_LOGGER_INFO("Setting UP IK");

	GL_LOGGER_INFO("Loading model done");

	//load the animations
	GL_LOGGER_INFO("---------------------Animations----------------------");
	GL_LOGGER_INFO("Loading Dancing Animation");
	idle.LoadAnimation("assets/Animations/Idle.dae", &ourModel);
	GL_LOGGER_INFO("Loading Walking animation");
	Walking.LoadAnimation("assets/Animations/Walking.dae", &ourModel);
	GL_LOGGER_INFO("Loading Strafe");
	running.LoadAnimation("assets/Animations/Running.dae", &ourModel);

	animator.Play(&Walking);

	animator.SetupIK("mixamorig_LeftHandIndex1");
	GL_LOGGER_INFO("done");
}

	void AnimationLayer::OnUpdate(Galactica::StepTimer const& timer)
	{

		float ts = static_cast<float> (timer.GetElapsedSeconds());

		camera.OnUpdate(ts);
		

		

		const glm::mat4 projection = camera.GetViewProjection();

		//-----------------Animation draw-----------------//
		
		AnimationShader.Use();
		AnimationShader.setMat4("projectionView", projection);

		
		auto model = glm::mat4(1.0f);

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		if (IK || movementOnPath)
		{
			model = movement;
		}

		AnimationShader.setMat4("animation", model);

		const auto transforms = animator.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			AnimationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		//get the translation and rotation
		if (movementOnPath)
		{
			if (path.path_completed)
			{
				animator.Play(&idle);
			}
			else
			{
				movement = path.Update(timer);
			}
			animator.UpdateAnimation(ts, path.m_NormalizedTime, path.m_SpeedFactor);
		}
		if (IK)
		{
			distance = path.DistanceXZ(path.GetPos(), targetPos);
			if (path.path_completed)
			{
				if (distance < 0.8f)
				{
					animator.SolveCCDIK(inverse(model) * glm::vec4{ targetPos, 1.0f }, ts);
				}
				else
				{
					animator.Play(&idle);
					animator.UpdateAnimation(ts, path.m_NormalizedTime, path.m_SpeedFactor);
				}
				
			}
			else
			{
				movement = path.Update(timer);
				animator.UpdateAnimation(ts, path.m_NormalizedTime, path.m_SpeedFactor);
			}
		}
		else
		{
			animator.UpdateAnimation(ts, path.m_NormalizedTime, 1);
		}

		//skin model
		if (showSkin)
		{
			ourModel.DrawModel(AnimationShader);
		}
		//-----------------Path draw-----------------//
		if (showPath)
		{
			LineShader.Use();
			LineShader.setMat4("projection", projection);
			auto pathMat = glm::mat4(1.0f);

			pathMat = glm::scale(pathMat, glm::vec3(1.0f, 1.0f, 1.0f));
			LineShader.setMat4("model", pathMat);

			drawPath.SetLine(path.m_PlotPoints);
			drawPath.DrawLine(LineShader, 1);
		}

		//-----------------Bones draw-----------------//
		if (showBones)
		{
			LineShader.Use();
			LineShader.setMat4("projection", projection);
			bones.SetLine(animator.debugBone);
			auto debugBone = glm::mat4(1.0f);

			debugBone = glm::scale(debugBone, glm::vec3(1.0f, 1.0f, 1.0f));
			if (IK || movementOnPath)
			{
				debugBone = movement;
			}

			LineShader.setMat4("model", debugBone);

			bones.DrawLine(LineShader,2);
		}

		//-----------------Floor draw-----------------//
		ModelShader.Use();
		ModelShader.setMat4("projection", projection);

		float angleInDegrees = 180.0f;
		float angleInRadians = glm::radians(angleInDegrees);

		auto floor = glm::mat4(1.0f);
		floor = glm::translate(floor, glm::vec3(-35.0f, 0.0f, 20.0f));
		floor = glm::scale(floor, glm::vec3(7.0f, 1.0f, 7.0f));
		floor = glm::rotate(floor, angleInRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		float floorColor = 1.0f;

		//floormesh.DebugMode(true);
		ModelShader.setFloat("objectcolor", 0.1f);
		ModelShader.setMat4("model", floor);
		floormesh.DrawMesh(ModelShader);

		auto bag = glm::mat4(1.0f);
		bag = glm::translate(bag, targetPos);
		bag = glm::scale(bag, glm::vec3(0.2f, 0.2f,0.2f));
		ModelShader.setFloat("objectcolor", 1.0f);
		ModelShader.setMat4("model", bag);
		ball.DrawModel(ModelShader);

		//Draw debug

		//floormesh.DebugMode(debugMode);
	}

	void AnimationLayer::OnImGuiRender()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("Settings");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("Animations");

		/*const char* items[] = {"Idle","Walking","Running"};
		static int item_current_idx = 0; 
		static int item_last_idx = 0;*/
		
		/*if (ImGui::BeginListBox(" "))
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

				if (item_current_idx == 0 && flag == true)
				{
					animator.Play(&idle);
					movementOnPath = false;
					IK = false;
					flag = false;
				}

				if (item_current_idx == 1 && flag == true)
				{
					animator.Play(&Walking);
					movementOnPath = false;
					IK = false;
					flag = false;
				}

				if (item_current_idx == 2 && flag == true)
				{
					animator.Play(&running);
					movementOnPath = false;
					IK = false;
					flag = false;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndListBox();
		}*/
		
		ImGui::Checkbox("Bones", &showBones);
		//ImGui::Checkbox("Skin", &showSkin);
		
	
		//if (ImGui::Button("Run on  Set path"))
		//{
		//	animator.Play(&running);
		//	/*path.GenerateDefultPath();*/
		//	movementOnPath = true;
		//	path.path_completed = false;
		//	IK = false;
		//	path.reset = true;
		//}

		ImGui::Checkbox("Path", &showPath);
		ImGui::Text("Velocity : %f", path.m_SpeedFactor);
		ImGui::Text("Distance to Target : %f", distance);
		ImGui::Text("Target : X( %f ) Y( %f ) Z( %f ) ", targetPos.x , targetPos.y, targetPos.z );
		ImGui::Text("Player : X( %f ) Y( %f ) Z( %f ) ", path.translateMat[3][0], path.translateMat[3][1], path.translateMat[3][2]);


		if (ImGui::Button("Set Target path for IK"))
        {
			IK = true;
			animator.Play(&Walking);
			glm::vec3 charPos = path.GetPos();
			path.GenerateNewPath(targetPos, charPos);
			
			animator.ResetIK();

			movementOnPath = false;
			path.path_completed = false;
			path.reset = true;
        }
		

		ImGui::Checkbox("Debug", &debugMode);

		ImGui::SliderFloat("X", &targetPos.x, 0.0f, 5.0f);
		ImGui::SliderFloat("Z", &targetPos.z, 0.0f, 5.0f);
		ImGui::SliderFloat("Y", &targetPos.y, 0.5f, 1.5f);
		
		ImGui::End();
	}

	void AnimationLayer::OnEvent(Galactica::Event& event)
	{
		camera.OnEvent(event);
	}

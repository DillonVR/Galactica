#pragma once

#include "imgui.h"
#include "Galactica/Animation/Animation.h"
#include "Galactica/Animation/Animator.h"
#include "Galactica/Renderer/CameraControl.h"
#include "Galactica/Renderer/Model.h"

#include "Galactica/Layer.h"
#include "Galactica/Animation/Movement.h"
#include "Galactica/Math/Spline.h"
#include "Galactica/Renderer/Line.h"

class AnimationLayer : public Galactica::Layer
{
public:

	bool my_tool_active = false;

	std::vector<Galactica::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Galactica::Texture> textures;
	std::vector<Galactica::BoneLine> skelly;

	bool showBones = false;
	bool showSkin = true;
	bool debugMode = false;
	bool animate = false;
	bool showPath = false;
	bool runPath = false;
	bool movementOnPath = false;

	bool flag = false;

	AnimationLayer();

	void OnUpdate(Galactica::StepTimer const& ts) override;

	void OnImGuiRender() override;

	void OnEvent(Galactica::Event& event) override;

private:

	Galactica::Model ourModel;

	Galactica::Model backpackModel;

	Galactica::Shader AnimationShader;

	Galactica::Shader ModelShader;

	Galactica::Shader LineShader;

	Galactica::CameraControl camera;

	Galactica::Animation idle;

	Galactica::Animation Walking;

	Galactica::Animation running;

	Galactica::Animator animator;

	Galactica::Mesh floormesh;

	Galactica::Line bones;

	Galactica::Movement path;

	Galactica::Line drawPath;
};


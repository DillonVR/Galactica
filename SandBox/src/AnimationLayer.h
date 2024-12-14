#pragma once

#include "Galactica/Renderer/Model.h"
#include "Galactica/Layer.h"
#include "Galactica/Math/Spline.h"
#include <Galactica/Physics/Physics.h>

#include "Galactica/Renderer/Renderer.h"

class AnimationLayer : public Galactica::Layer
{
public:

	bool my_tool_active = false;

	std::vector<Galactica::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Galactica::Texture> textures;

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

	Galactica::Model* plane;

	Galactica::Renderer::Renderer* renderer;

	Galactica::SoftBody* softBody;

	Galactica::CameraControl camera;
};


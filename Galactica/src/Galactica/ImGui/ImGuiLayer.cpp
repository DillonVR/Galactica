#include "glpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"

#include "backends/imgui_impl_opengl3.h"
#include "Galactica/Application.h"
#include "GLFW/glfw3.h"

namespace Galactica
{
	ImGuiLayer::ImGuiLayer()
		:	Layer("ImGuiLayer"),
			show_demo_window(false)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app =  Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(),app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Begin("Hello, world!");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		Layer::OnEvent(event);
	}

	void ImGuiLayer::OnAttach()
	{
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		io.ConfigFlags |= ImGuiBackendFlags_HasMouseCursors;      //can use mouse??
		io.ConfigFlags |= ImGuiBackendFlags_HasSetMousePos;      //Set mouse??

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImGuiLayer::OnDetach()
	{
		Layer::OnDetach();
	}
}

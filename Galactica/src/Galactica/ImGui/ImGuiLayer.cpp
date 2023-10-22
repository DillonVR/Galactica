#include "glpch.h"

#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Galactica/Application.h"
#include "GLFW/glfw3.h"

namespace Galactica
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer"),
		show_demo_window(false)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow(&show_demo_window);
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

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}

	void ImGuiLayer::End()
	{

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		// {
		// 	GLFWwindow* backup_current_context = glfwGetCurrentContext();
		// 	ImGui::UpdatePlatformWindows();
		// 	ImGui::RenderPlatformWindowsDefault();
		// 	glfwMakeContextCurrent(backup_current_context);
		// }
	}

}

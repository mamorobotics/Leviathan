#include "UI.hpp"

void UI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//io.Fonts->AddFontFromFileTTF("src/resources/NotoSans-SemiBold.ttf", 16);

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	for (int i = 0; i < 16; i++)
	{
		if (glfwJoystickPresent(i) != NULL)
		{
			controllers.push_back(std::string(glfwGetJoystickName(i)));
		}
	}
}

void UI::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UI::Update()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::Begin("Camera View");
	ImGui::Image((void*)(intptr_t)cameraTexture, ImVec2(cameraWidth, cameraHeight));
	ImGui::End();

	ImGui::Begin("Connection");
	ImGui::Text("IP: 1.1.1.1");
	ImGui::Text("Port: 1111");
	ImGui::Text("FPS: 60");
	ImGui::Text("Connection Status: Starting");
	ImGui::End();

	ImGui::Begin("Controller");
	static const char* current_item = NULL;
	if (ImGui::BeginCombo("##combo", current_item))
	{
		for (int n = 0; n < controllers.size(); n++)
		{
			bool is_selected = (current_item == controllers[n]);
			if (ImGui::Selectable(controllers[n].c_str(), is_selected))
			{
				current_item = controllers[n].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	int count;
	for (int i = 0; i < 16; i++)
	{
		if (glfwJoystickPresent(i) != NULL)
		{
			if (ImGui::TreeNode(("Controller: " + std::string(glfwGetJoystickName(i)) + " (" + std::to_string(i) + ")").c_str()))
			{
				ImGui::Text(("Left Stick Coordinate: (" + std::to_string(glfwGetJoystickAxes(i, &count)[0]) + ", "
					+ std::to_string(glfwGetJoystickAxes(i, &count)[1]) + ")").c_str());
				ImGui::Text(("Right Stick Coordinate: (" + std::to_string(glfwGetJoystickAxes(i, &count)[2]) + ", "
					+ std::to_string(glfwGetJoystickAxes(i, &count)[5]) + ")").c_str());
				ImGui::Text(("Left Trigger: " + std::to_string(glfwGetJoystickAxes(i, &count)[3])).c_str());
				ImGui::Text(("Right Trigger: " + std::to_string(glfwGetJoystickAxes(i, &count)[4])).c_str());
				ImGui::Text(("A: " + std::to_string(glfwGetJoystickButtons(i, &count)[1])).c_str());
				ImGui::Text(("X: " + std::to_string(glfwGetJoystickButtons(i, &count)[0])).c_str());
				ImGui::Text(("B: " + std::to_string(glfwGetJoystickButtons(i, &count)[2])).c_str());
				ImGui::Text(("Y: " + std::to_string(glfwGetJoystickButtons(i, &count)[3])).c_str());
				ImGui::Text(("Left Bumper: " + std::to_string(glfwGetJoystickButtons(i, &count)[4])).c_str());
				ImGui::Text(("Right Bumper: " + std::to_string(glfwGetJoystickButtons(i, &count)[5])).c_str());
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Output");

	if (ImGui::BeginListBox("##Output box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (int n = 0; n < output.size(); n++)
			ImGui::Text(output[n].c_str());
		ImGui::EndListBox();
	}
	ImGui::End();
	

	ImGui::Begin("Telemetry");

	if (ImGui::BeginListBox("##Telemetry box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (int n = 0; n < telemetry.size(); n++)
			ImGui::Text(telemetry[n].c_str());
		ImGui::EndListBox();
	}
	ImGui::End();
	
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("Pause Video Feed", &pauseCamera);
	ImGui::SliderInt("Quality", &quality , 0, 100, "%d%%");
	ImGui::End();
}

void UI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

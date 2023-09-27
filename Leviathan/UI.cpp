#include "UI.hpp"

void UI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontFromFileTTF("NotoSans-SemiBold.ttf", 16);

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

	ImGui::BeginMainMenuBar();
	if (ImGui::MenuItem("Exit"))
		exit(0);
	if (ImGui::MenuItem("Statistics"))
	{
		statisticsOpen = true;
	}
	ImGui::EndMainMenuBar();

	if (statisticsOpen)
	{
		ImGui::Begin("Statistics");
		if (ImGui::Button("Exit"))
			statisticsOpen = false;

		float averageDelta = 0;
		float deltaVals2[64];
		for (int i = 0; i < 64; i++)
		{
			averageDelta += deltaVals[i];
			deltaVals2[i] = deltaVals[(i + 65) % 64];
		}
		averageDelta /= 64;
		std::copy(std::begin(deltaVals2), std::end(deltaVals2), std::begin(deltaVals));
		deltaVals[63] = ImGui::GetIO().DeltaTime;
		ImGui::PlotLines("DeltaTime", deltaVals, IM_ARRAYSIZE(deltaVals), 0, std::to_string(averageDelta).c_str(), -1.0f, 1.0f, ImVec2(0, 80.0f));

		float averageFrame = 0;
		float frameVals2[64];
		for (int i = 0; i < 64; i++)
		{
			averageFrame += frameVals[i];
			frameVals2[i] = frameVals[(i + 65) % 64];
		}
		averageFrame /= 64;
		std::copy(std::begin(frameVals2), std::end(frameVals2), std::begin(frameVals));
		frameVals[63] = ImGui::GetIO().Framerate;
		ImGui::PlotLines("Framerate", frameVals, IM_ARRAYSIZE(frameVals), 0, std::to_string((int)averageFrame).c_str(), -1.0f, 1.0f, ImVec2(0, 80.0f));
		ImGui::End();
	}

	ImGui::Begin("Camera View");
	ImGui::Image((void*)(intptr_t)cameraTexture, ImVec2(cameraWidth, cameraHeight));
	ImGui::End();

	ImGui::Begin("Networking");
	if (ImGui::CollapsingHeader("Host"))
	{
		ImGui::Text(("IP: " + selfIP).c_str());
		ImGui::Text(("Port: " + selfPort).c_str());
	}
	if (ImGui::CollapsingHeader("Connection"))
	{
		ImGui::Text(("IP: " + connectedIP).c_str());
		ImGui::Text(("Port: " + connectedPort).c_str());
		if (ImGui::Button("Connect"))
			output.push_back("Connect");
		ImGui::Text(("Status: " + connectionStatus).c_str());
		
	}	
	ImGui::End();

	ImGui::Begin("Controller");
	static const char* current_item = "";
	if (ImGui::BeginCombo("Controller", current_item))
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
		if (glfwJoystickPresent(i) != NULL && std::string(glfwGetJoystickName(i)) == current_item)
		{
			ImGui::Text(("Controller: " + std::string(glfwGetJoystickName(i)) + " (" + std::to_string(i) + ")").c_str());
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

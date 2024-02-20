#include "UI.hpp"

UI::UI()
{

}

void UI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();

	start = time(0);
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
	if (ImGui::MenuItem("Diagnostics"))
	{
		statisticsOpen = true;
	}
	ImGui::EndMainMenuBar();

	if (statisticsOpen)
	{
		ImGui::Begin("Diagnostics");
		if (ImGui::Button("Exit"))
			statisticsOpen = false;

		std::map<std::string, float[64]>::iterator it = graphs.begin();
		while (it != graphs.end())
		{
			float averageVal = 0;
			for (int i = 0; i < 64; i++)
			{
				averageVal += it->second[i];
			}
			averageVal /= 64;
			ImGui::PlotLines(it->first.c_str(), it->second, IM_ARRAYSIZE(it->second), 0, std::to_string(averageVal).c_str(), -1.0f, 1.0f, ImVec2(0, 80.0f));
		}

		if (ImGui::Button("Test Error"))
			PublishOutput("Error Test", LEV_CODE::TEST);
		if (ImGui::Button("Test Telemetry"))
			PublishTelemetry("Telemetry Test", std::to_string(rand()));
		ImGui::Text(("Main delta time: " + std::to_string(mainDeltaTime)).c_str());
		ImGui::End();
	}

	ImGui::Begin("Camera View");
	ImGui::Image((void*)(intptr_t)cameraTexture, ImVec2(static_cast<float>(cameraWidth), static_cast<float>(cameraHeight)));
	ImGui::End();

	ImGui::Begin("Networking");
	if (ImGui::CollapsingHeader("Host"))
	{
		ImGui::Text(("IP: " + connDetails.selfIP).c_str());
		ImGui::Text(("Port: " + connDetails.selfPort).c_str());
	}
	if (ImGui::CollapsingHeader("Connection"))
	{
		ImGui::Text(("IP: " + connDetails.connectedIP).c_str());
		ImGui::Text(("Port: " + connDetails.connectedPort).c_str());
		if (ImGui::Button("Connect")){
			std::cout << "connect button" << std::endl;
		}
		ImGui::Text(("Status: " + connDetails.connectionStatus).c_str());
		
	}	
	ImGui::End();

	ImGui::Begin("Controller");

	ControllerValues* controllerValues = controller->GetControllerValues();
	ImGui::Text(("Controller: " + std::string(glfwGetJoystickName(controller->getId())) + " (" + std::to_string(controller->getId()) + ")").c_str());
	ImGui::Text(("Left Stick Coordinate: (" + std::to_string(controllerValues->ljoyx) + ", " + std::to_string(controllerValues->ljoyy) + ")").c_str());
	ImGui::Text(("Right Stick Coordinate: (" + std::to_string(controllerValues->rjoyx) + ", " + std::to_string(controllerValues->rjoyy) + ")").c_str());
	ImGui::Text(("Left Trigger: " + std::to_string(controllerValues->ltrigger)).c_str());
	ImGui::Text(("Right Trigger: " + std::to_string(controllerValues->rtrigger)).c_str());
	ImGui::Text(("A: " + std::to_string(controllerValues->a)).c_str());
	ImGui::Text(("B: " + std::to_string(controllerValues->b)).c_str());
	ImGui::Text(("X: " + std::to_string(controllerValues->x)).c_str());
	ImGui::Text(("Y: " + std::to_string(controllerValues->y)).c_str());
	ImGui::Text(("Left Bumper: " + std::to_string(controllerValues->lbumper)).c_str());
	ImGui::Text(("Right Bumper: " + std::to_string(controllerValues->rbumper)).c_str());

	ImGui::End();

	ImGui::Begin("Output");

	if (ImGui::BeginListBox("##Output box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (unsigned int n = 0; n < output.size(); n++) 
		{
			ImGui::Text((output[n]).c_str());
		}
		ImGui::EndListBox();
	}
	ImGui::End();	

	ImGui::Begin("Telemetry");

	if (ImGui::BeginListBox("##Telemetry box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		std::map<std::string, std::string>::iterator it = telemetry.begin();
		while (it != telemetry.end())
		{
			ImGui::Text((it->first + ": " + it->second).c_str());
			++it;
		}
		ImGui::EndListBox();
	}
	ImGui::End();
	
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("Pause Video Feed", &pauseCamera);
	ImGui::SliderInt("Quality", &quality , 0, 100, "%d%%");
	ImGui::End();
}

void UI::Render(GLFWwindow* window)
{
	ImGui::Render();
	glClearColor(1.00f, 1.00f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void UI::PublishOutput(std::string msg, LEV_CODE code)
{
	std::string seconds = std::to_string((int)difftime(time(0), start) % 60);
	std::string minutes = std::to_string((int)difftime(time(0), start) / 60);
	if (seconds.length() == 1)
		seconds = "0" + seconds;
	output.push_back("[" + minutes + ":" + seconds + "] [" + CodeDef(code) + "] " + msg);
}

void UI::PublishTelemetry(std::string id, std::string value)
{
	telemetry[id] = value;
}

void UI::PublishGraph(std::string id, float value)
{
	float values2[64];
	for (int i = 0; i < 64; i++)
	{
		values2[i] = graphs[id][(i + 65) % 64];
	}
	std::copy(std::begin(values2), std::end(values2), std::begin(graphs[id]));
	graphs[id][63] = value;
}

void UI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

UI::~UI()
{
}

UI* UI::Get()
{
	if (ui == nullptr)
		ui = new UI();

	return ui;
}

void UI::setConnectionDetails(ConnDetails details)
{
	connDetails = details;
}
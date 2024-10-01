#include "UI.hpp"

UI::UI()
{
	
}

void UI::CreateCameraTexture()
{
	glGenTextures(1, &cameraTexture);
	glBindTexture(GL_TEXTURE_2D, cameraTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UI::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
	if (ImGui::MenuItem("Float"))
	{
		floatOpen = true;
	}
	ImGui::EndMainMenuBar();

	if (statisticsOpen)
	{
		ImGui::Begin("Diagnostics");
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
		if (ImGui::Button("Test Error"))
			PublishOutput("Error Test", LEV_CODE::TEST);
		if (ImGui::Button("Test Telemetry"))
			PublishTelemetry("Telemetry Test", std::to_string(rand()));
		ImGui::Text(("Main delta time: " + std::to_string(mainDeltaTime)).c_str());
		ImGui::End();
	}

	if (floatOpen) {
		ImGui::Begin("Float");
		if (ImGui::Button("Exit"))
			floatOpen = false;

		ImGui::InputInt("Float Id", &floatId);
		ImGui::PlotLines("Depth", depthVals, IM_ARRAYSIZE(depthVals), 0, NULL, -1.0f, 1.0f, ImVec2(0, 80.0f));
		ImGui::End();
	}

	ImGui::Begin("Camera View");
	ImGui::Image((void*)(intptr_t)cameraTexture, ImVec2(cameraWidth, cameraHeight));
	ImGui::End();

	ImGui::Begin("Networking");
	Connection* conn = Connection::Get();
	if (ImGui::CollapsingHeader("Host"))
	{
		ImGui::Text(("IP: " + connDetails.selfIP).c_str());
		ImGui::Text(("Port: " + connDetails.selfPort).c_str());
	}
	if (ImGui::CollapsingHeader("Connection"))
	{
		ImGui::Text(("IP: " + connDetails.connectedIP).c_str());
		ImGui::Text(("Port: " + connDetails.connectedPort).c_str());
		ImGui::Text(("Status: " + connDetails.connectionStatus).c_str());
	}
	if (ImGui::Button("Reconnect")) conn->Reconnect();
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
	ImGui::Text(("Dpad Up: " + std::to_string(controllerValues->up)).c_str());
	ImGui::Text(("Dpad Down: " + std::to_string(controllerValues->down)).c_str());
	ImGui::Text(("Dpad Left: " + std::to_string(controllerValues->left)).c_str());
	ImGui::Text(("Dpad Right: " + std::to_string(controllerValues->right)).c_str());
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
	ImGui::Checkbox("Main Camera", &mainCamera);
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
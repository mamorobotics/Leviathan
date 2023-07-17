#include "UseImGui.h"

void UseImGui::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
}

void UseImGui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UseImGui::Update()
{
	const char* ip = "1.1.1.1";

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::Begin("Camera View");
	ImGui::Image((void*)(intptr_t)cameraTexture, ImVec2(cameraWidth, cameraHeight));
	ImGui::End();

	ImGui::Begin("Connection Details");
	ImGui::Text("IP: 1.1.1.1");
	ImGui::Text("Port: 1111");
	ImGui::Text("FPS: 60");
	ImGui::Text("Connection Status: Starting");
	ImGui::End();

	ImGui::Begin("Output");
	const char* output[] = { "[STATUS] This is some output", "[STATUS] I promise this output isnt self aware", "[STATUS] Continue along your way", "[ERROR] !(*ASD*HN!@KHA)+@#"};

	if (ImGui::BeginListBox("##Output box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (int n = 0; n < IM_ARRAYSIZE(output); n++)
			ImGui::Text(output[n]);
		ImGui::EndListBox();
	}
	ImGui::End();
	

	ImGui::Begin("Telemetry");
	const char* telemetry[] = { "Angle 1: 90", "Angle 2: 45", "Sin: Cos", "YAY: YAY" };

	if (ImGui::BeginListBox("##Telemetry box", ImVec2(-FLT_MIN, -FLT_MIN)))
	{
		for (int n = 0; n < IM_ARRAYSIZE(telemetry); n++)
			ImGui::Text(telemetry[n]);
		ImGui::EndListBox();
	}
	ImGui::End();
	
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("Pause Video Feed", &pauseCamera);
	ImGui::End();
}

void UseImGui::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UseImGui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

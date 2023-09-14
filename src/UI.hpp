#pragma once

#include <vector>
#include <string>

#include <ImGui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class UI {
public:
	int cameraWidth, cameraHeight;
	GLuint cameraTexture;
	bool pauseCamera = false;
	int quality = 80;

	//Outputs:
	std::vector<std::string> output;
	std::vector<std::string> telemetry;

	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();
};
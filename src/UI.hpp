#pragma once

#include <vector>
#include <string>
#include <iostream>

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
	bool statisticsOpen = false;
	int selectedController = 0;
	int quality = 80;
	float values[64];
	std::vector<std::string> controllers;

	std::string selfIP;
	std::string selfPort;
	std::string connectedIP;
	std::string connectedPort;
	std::string connectionStatus;


	//Outputs:
	std::vector<std::string> output;
	std::vector<std::string> telemetry;

	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();
};
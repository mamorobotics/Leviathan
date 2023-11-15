#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <tuple>
#include <map>

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui.h"
#include "ConnDetails.hpp"
#include "Management.hpp"
#include "Connection.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class UI {
private:
	int cameraWidth, cameraHeight = 0;
	time_t start = time(0);
	GLuint cameraTexture = NULL;
	bool pauseCamera = false;
	bool statisticsOpen = false;
	int selectedController = 0;
	int quality = 80;
	float deltaVals[64];
	float frameVals[64];

	std::vector<std::string> controllers;
	std::vector<std::string> output;
	std::map<std::string, std::string> telemetry;

	ConnDetails connDetails;

	static UI* ui;

public:
	UI();
	UI(const UI& obj) = delete;
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	void Update();
	void Render(GLFWwindow* window);
	void PublishOutput(std::string msg, LEV_CODE code = LEV_CODE::CLEAR);
	void PublishTelemetry(std::string id, std::string value);
	void Shutdown();
	~UI();

	static UI* Get();

	void setConnectionDetails(ConnDetails connDetails);
	GLuint* getCameraTexture() { return &cameraTexture; }
	int* getCameraWidth() { return &cameraHeight; }
	int* getCameraHeight() { return &cameraWidth; }
};
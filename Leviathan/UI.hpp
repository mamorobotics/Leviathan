#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <tuple>
#include <thread>
#include <map>

#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ConnDetails.hpp"
#include "Management.hpp"
#include "Connection.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Controller.hpp"

class UI {
private:
	int cameraWidth, cameraHeight = 0;
	time_t start = time(0);
	GLuint cameraTexture;
	bool pauseCamera = false;
	bool mainCamera = true;
	bool statisticsOpen = false;
	int selectedController = 0;
	int quality = 80;
	float deltaVals[64];
	float frameVals[64];
	float mainDeltaTime;

	std::vector<std::string> output;
	std::map<std::string, std::string> telemetry;

	ConnDetails connDetails;

	static UI* ui;

public:
	Controller* controller;

	UI();
	UI(const UI& obj) = delete;
	void CreateCameraTexture();
	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	void Update();
	void Render(GLFWwindow* window);
	void PublishOutput(std::string msg, LEV_CODE code = LEV_CODE::CLEAR);
	void PublishTelemetry(std::string id, std::string value);
	void Shutdown();
	~UI();

	static UI* Get();

	void setMainDeltaTime(float time) { mainDeltaTime = time; }
	void setConnectionDetails(ConnDetails connDetails);
	bool isCameraPaused() { return pauseCamera; }
	bool isMainCamera() { return mainCamera; }
	int getCameraQuality() { return quality; }
	GLuint getCameraTexture() { return cameraTexture; }
	void setCameraWidth(int width) { cameraWidth = width; }
	void setCameraHeight(int height) { cameraHeight = height; }
};
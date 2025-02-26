#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <time.h>
#include <tuple>
#include <thread>
#include <map>

#include "imgui.h"
#include "implot.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ConnDetails.hpp"
#include "Management.hpp"
#include "Connection.hpp"
#include "Serial.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Controller.hpp"
#include "stb_image_write.h"
#include "LoadTexture.hpp"

class UI {
private:
	int cameraWidth, cameraHeight = 0;
	int floatId = 0;
	int stillNum = 1;
	time_t start = time(0);
	GLuint cameraTexture;
	bool pauseCamera = false;
	bool mainCamera = true;
	bool statisticsOpen = false;
	bool stillsOpen = false;
	int selectedController = 0;
	int quality = 80;
	float deltaVals[64];
	float frameVals[64];
	float depthVals[128];
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
	GLuint LoadStillAsTexture();
	/**
     * @brief This initializes the IMGUI instance, setting IMGUI config variables and setting linking it to the glsl instance.
	 * @param window A GLFWwindow pointer to the window object created by glfw.
	 * @param glsl_version A char array pointer stating the currently used glsl version (recommended "#version 120").
     */
	void Init(GLFWwindow* window, const char* glsl_version);
	/**
     * @brief Creates the imgui instance, seperated out from Init for control flow purposes. 
     */
	void NewFrame();
	/**
     * @brief Update handles the main event loop of the IMGUI application. 
	 * In here you can define all UI elements to be displayed and how they interact with the program around them.
     */
	void Update();
	void Render(GLFWwindow* window);
	void PublishOutput(std::string msg, LEV_CODE code = LEV_CODE::CLEAR);
	void PublishTelemetry(std::string id, std::string value);
	void Shutdown();
	~UI();

	static UI* Get();
	void setMainDeltaTime(float time) { mainDeltaTime = time; }
	void setConnectionDetails(ConnDetails connDetails);
	/**
     * @brief Returns whether or not the camera is currently paused through the camera UI.
	 * @return Boolean stating whether or not the camera is paused.
     */
	bool isCameraPaused() { return pauseCamera; }
	/**
     * @brief Returns whether or not the currently selected camera in the UI is the "main camera" (Going to be deprecated later for multi-camera).
	 * @return Boolean stating whether or not the currently slected camera is "main".
     */
	bool isMainCamera() { return mainCamera; }
	/**
     * @brief Returns the currently requested camera quality within the UI.
	 * @return Integer stating currently requested camera quality.
     */
	int getCameraQuality() { return quality; }
	/**
     * @brief Returns the texture buffer the camera writes to.
	 * @return GLuint representing the camera texture buffer.
     */
	GLuint getCameraTexture() { return cameraTexture; }
	/**
     * @brief Set the camera width throughout the application.
	 * @param width Integrer stating the width.
     */
	void setCameraWidth(int width) { cameraWidth = width; }
	/**
     * @brief Set the camera height throughout the application.
	 * @param height Integer stating the height.
     */
	void setCameraHeight(int height) { cameraHeight = height; }
};
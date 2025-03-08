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

/**
 * @class UI
 * @brief Handles the user interface of leviathan
 */
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
	bool stillsTexturesLoaded = false;
	int selectedController = 0;
	int quality = 80;
	float deltaVals[64];
	float frameVals[64];
	float depthVals[128];
	float mainDeltaTime;

	std::vector<GLuint> stillsTextures;
	std::vector<std::string> output;
	std::map<std::string, std::string> telemetry;

	ConnDetails connDetails;

	static UI* ui;

public:
	Controller* controller;

	UI();
	UI(const UI& obj) = delete;
	/**
     * @brief Creates the camera texture buffer.
     */
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
	 * This code does not specifically display the window, just defines how it looks and behaves.
     */
	void Update();
	/**
     * @brief Handles the final rendering of the window to the screen.
	 * @param window GLFWwindow pointer of the application window to publish to.
     */
	void Render(GLFWwindow* window);
	/**
     * @brief Publishes data to the output box.
	 * @param msg Message string to be published
	 * @param code Output code defined by LEV_CODE within Management.hpp.
     */
	void PublishOutput(std::string msg, LEV_CODE code = LEV_CODE::CLEAR);
	/**
     * @brief Publishes data to the telemetry box.
	 * @param id The string ID of the telemetric data to show.
	 * @param value The string value of the telemetric data to update.
     */
	void PublishTelemetry(std::string id, std::string value);
	/**
     * @brief Shuts down the application smoothly.
     */
	void Shutdown();
	~UI();

	/**
     * @brief Grabs the UI singleton pointer.
     */
	static UI* Get();
	/**
     * @brief Sets the current UI update delta time for debugging purposes.
	 * @param time Current delta time.
     */
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
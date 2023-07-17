#pragma once

#include <ImGui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "glad/glad.h"

class UseImGui {
public:
	int cameraWidth, cameraHeight;
	GLuint cameraTexture;
	bool pauseCamera;

	void Init(GLFWwindow* window, const char* glsl_version);
	void NewFrame();
	virtual void Update();
	void Render();
	void Shutdown();
};
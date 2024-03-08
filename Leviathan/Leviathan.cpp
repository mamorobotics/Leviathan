#pragma once

#include <iostream>
#include <chrono>

#include "UI.hpp"
#include "Connection.hpp"

#include "Controller.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std::chrono;

UI* UI::ui = new UI();
Connection* Connection::connection = new Connection();

void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    UI::Get()->PublishOutput(s, LEV_CODE::GENERAL_ERROR);
}

int main()
{
	UI* gui = UI::Get();
	Connection* conn = Connection::Get();

	Controller::ScanControllers();
	
	//Setup GLFW and Imgui
	glfwSetErrorCallback( error_callback );

	if (!glfwInit())
		return 2;

	const char* glsl_version = "#version 120";
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Leviathan", NULL, NULL);

	if (window == NULL)
		return 2;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("Unable to context to OpenGL");

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	gui->Init(window, glsl_version);

	Controller* controller = new Controller(0);
	gui->controller = controller;

	bool firstFrame = true;

	std::thread networkThread(&Connection::HandleHandshake, conn);
	networkThread.detach();

	while (!glfwWindowShouldClose(window)) {
		auto start = high_resolution_clock::now();
		glfwPollEvents();
		gui->NewFrame();
		gui->Update();
		gui->Render(window);
			
		if (firstFrame) {
			firstFrame = false;
		}
		auto stop = high_resolution_clock::now();
		int duration = duration_cast<microseconds>(stop - start).count();
		gui->setMainDeltaTime((float)duration / 1000000);

		conn->Send(5, controller->GetControllerValues()->toString());

		const float* joys = glfwGetJoystickAxes(id, &joyCount);
		const unsigned char* buttons = glfwGetJoystickButtons(id, &buttonCount);

		for (int i = 0; i < sizeof(joys)/sizeof(joys[0]); i++)
		{
			if (joys[i])
				std::cout << i << std::endl;
		}
		for (int i = 0; i < sizeof(buttons)/sizeof(buttons[0]); i++)
		{
			if (buttons[i])
				std::cout << i << std::endl;
		}
		
	}

	gui->Shutdown();

	glDeleteTextures(1, &gui->getCameraTexture());
	glfwTerminate();
	
	return 0;
}

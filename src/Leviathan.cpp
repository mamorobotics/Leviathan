#pragma once
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>

#include "UI.hpp"
#include "LoadTexture.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include "asio.hpp"
#include "asio/ts/buffer.hpp"
#include "asio/ts/internet.hpp";

int main()
{

	//Setup GLFW and Imgui
	if (!glfwInit())
		return 1;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Leviathan", NULL, NULL);

	if (window == NULL)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("Unable to context to OpenGL");

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	UI gui;
	gui.Init(window, glsl_version);

	bool ret = LoadTexture::LoadTextureFromFile("src/resources/4.2.05.png", &gui.cameraTexture, &gui.cameraWidth, &gui.cameraHeight);
	IM_ASSERT(ret);

	bool firstFrame = true;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(1.00f, 1.00f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		gui.NewFrame();
		gui.Update();
		gui.Render();
		glfwSwapBuffers(window);

		/*if (firstFrame) {
			asio::error_code ec;
			asio::io_context context;
			asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(asio::ip::make_address("192.2.3.4", ec), 1974);
			asio::ip::tcp::socket socket = asio::ip::tcp::socket(context);
			socket.connect(endpoint, ec);
			if (!ec) { gui.output.push_back("Connected!"); }
			else { gui.output.push_back("Failed to connect to address: \n" + ec.message()); }

			firstFrame = false;
		}*/
	}

	gui.Shutdown();

	return 0;

	
}
/**
 * @file Leviathan.cpp
 * @brief This is the main entry point of the program.
 *
 * This file contains the main function and initializes all subsystems.
 */

#pragma once

#include <iostream>
#include <chrono>

#include "UI.hpp"
#include "Connection.hpp"

#include "Controller.hpp"
#include "Serial.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std::chrono;

UI* UI::ui = new UI();
Connection* Connection::connection = new Connection();
Serial* Serial::serial = new Serial();

/**
 * @brief This function publishes the error message to telemetry.
 * @param error the general error code
 * @param msg the error message
 */
void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    UI::Get()->PublishOutput(s, LEV_CODE::GENERAL_ERROR);
}

/**
 * @brief This function loads the texture from the given data buffer and stores it to 
 * the camera texture.
 * @param dataPtr pointer to the data buffer so that texture can be loaded
 */
void LoadTexture(std::vector<char>* dataPtr)
{
	Connection* conn = Connection::Get();
    UI* gui = UI::Get();

	conn->SetDecoding(true);

    if(dataPtr == nullptr || dataPtr->empty()){
        conn->SetDecoding(false);
		conn->SetNewImage(false);
        return;
    }
    std::vector<char> data = *dataPtr;
    
    cv::Mat mat = cv::imdecode(data, cv::IMREAD_UNCHANGED);

    if(mat.empty()){
        gui->PublishOutput("Unable to decode the JPEG image.", LEV_CODE::IMAGE_ERROR);
        conn->SetDecoding(false);
		conn->SetNewImage(false);
        return;
    }

    gui->setCameraWidth(mat.cols);
    gui->setCameraHeight(mat.rows);

	//Flip Camera Texture bc OpenGL reads from bottom left so images are flipped
	cv::flip(mat, mat, 1);

    glBindTexture(GL_TEXTURE_2D, gui->getCameraTexture());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    conn->SetDecoding(false);
	conn->SetNewImage(false);
}

int main()
{
	UI* gui = UI::Get();
	Connection* conn = Connection::Get();
	Serial* ser = Serial::Get();

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

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Leviathan", NULL, NULL);

	if (window == NULL)
		return 2;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw("Unable to context to OpenGL");

	gui->CreateCameraTexture();

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	gui->Init(window, glsl_version);

	Controller* controller = new Controller(1);
	gui->controller = controller;

	bool firstFrame = true;

	//ethernet init
	std::thread networkThread(&Connection::HandleHandshake, conn);
	networkThread.detach();

	//serial init
	std::thread serialThread(&Serial::SendControllerAndGetFloatData, ser);
	serialThread.detach();

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

		int joyCount, buttonCount;
		if(!conn->GetDecoding() && conn->GetNewImage())
		{
			LoadTexture(conn->GetImageBuffer());
		}
	}

	gui->Shutdown();
	
	return 0;
}

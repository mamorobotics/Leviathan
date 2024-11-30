#pragma once

#include <iostream>
#include <chrono>

#include "UI.hpp"
#include "Connection.hpp"

#include "Controller.hpp"
#include "Serial.hpp"
#include "Radio.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std::chrono;

UI* UI::ui = new UI();
Connection* Connection::connection = new Connection();
Serial* Serial::serial = new Serial();
Radio* Radio::radio = new Radio();

void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    UI::Get()->PublishOutput(s, LEV_CODE::GENERAL_ERROR);
}

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
//	Radio* rad = Radio::Get();

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

	//eth
	//std::thread networkThread(&Connection::HandleHandshake, conn);
	//networkThread.detach();

	//serial
	std::thread serialThread(&Serial::SendController, ser);
	serialThread.detach();

	//radio
	// std::thread radioThread(&Radio::startRadioReceive, rad);
	// radioThread.detach();

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

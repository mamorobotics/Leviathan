reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee#pragma once
#include <iostream>
#include <tuple>
#include <sstream>
#include <thread>

#include <asio.hpp>
#include <asio.hpp>
#include "UI.hpp"
#include <unistd.h>
#include <atomic>
#include <thread>
#include <chrono>

#include "Management.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/RS-232/rs232.h"

/**
 * @class Serial
 * @brief Connects via usb Serial to the Arduino nano so send controller data and receive float data.
 */
class Serial
{
private:
	int cport_nr = 16; //GET PORT USING COMMANDS FROM https://www.monocilindro.com/2017/02/19/how-to-connect-arduino-and-raspberry-pi-using-usb-and-c/

	int index = 0;
	int dataPts1 = 0;
	int dataPts2 = 0;

	std::vector<std::string> floatOutputs;
	std::array<std::array<float, 128>, 2> depthProfileOne;
	std::array<std::array<float, 128>, 2> depthProfileTwo;

	static Serial* serial;

public:
	/**
     * @brief Init the seril class and starts connection with Arduino to send controller data
	 * and receive float data.
     */
	Serial(){
		int bdrate=9600; /* 9600 baud */
		char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit

		if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
        {
			//UI* gui = UI::Get();
            //gui->PublishOutput("Can not open comport", LEV_CODE::CONN_ERROR);
			std::cout<<"cannnot open comport"<<std::endl;
        }
	}
	/**
     * @brief Primary function that opperates the call and response between the Arduino 
	 * and the computer.
     */
	void SendControllerAndGetFloatData();
	/**
     * @brief This is a getter for depth values
	 * @param i float ID
	 * @return array of depth values each item contains the time depth
	 * was recorded and the depth value
     */
	std::array<std::array<float, 128>, 2> getDepthVals(int i){
		if(i==1){
			return depthProfileOne;
		}
		return depthProfileTwo;
	}
	/**
     * @brief Get amount of data in the depth profile
	 * @param i float ID
	 * @return number of data points
     */
	int getNumDataPts(int i){
		if(i==1){
			return dataPts1;
		}
		return dataPts2;
	}
	/**
     * @brief returns the float outputs
	 * @return vector of float outputs
     */
	std::vector<std::string> getFloatOutputs(){
		return floatOutputs;
	}
	Serial(const Serial& obj) = delete;
	~Serial();
	static Serial* Get();
};

#pragma once
// #include <iostream>
// #include <tuple>
// #include <sstream>
// #include <thread>
// #include <opencv2/opencv.hpp>

// #include <asio.hpp>
#include "UI.hpp"
// #include <unistd.h>
// #include <atomic>
// #include <thread>

// #include "glad/glad.h"
// #include "GLFW/glfw3.h"
#include "lib/RS-232/rs232.h"

class Serial
{
private:
	int cport_nr = 24; //GET PORT USING COMMANDS FROM https://www.monocilindro.com/2017/02/19/how-to-connect-arduino-and-raspberry-pi-using-usb-and-c/

	static Serial* serial;

public:
	Serial(){
		int bdrate=9600; /* 9600 baud */
		char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit

		if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
        {
            printf("Can not open comport\n");
        }
	}
	void SendController();
	Serial(const Serial& obj) = delete;
	~Serial();
	static Serial* Get();
};

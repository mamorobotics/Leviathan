#pragma once
#include <iostream>
#include <tuple>
#include <sstream>
#include <thread>
#include <opencv2/opencv.hpp>

#include <asio.hpp>
#include "UI.hpp"
#include "ConnDetails.hpp"
#include "Management.hpp"
#include <unistd.h>
#include <atomic>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "lib/RS-232/rs232.h"

#define PORT 8080
#define IP "192.168.1.1"     

using asio::ip::udp;
using asio::ip::address;

class Connection
{
private:
	ConnDetails connDetails;

	asio::io_context io_context;
    udp::socket socket;
    udp::endpoint remote_endpoint;
	udp::endpoint sender_endpoint;

	int cport_nr=16; //GET PORT USING COMMANDS FROM https://www.monocilindro.com/2017/02/19/how-to-connect-arduino-and-raspberry-pi-using-usb-and-c/

	std::vector<char> size_buffer;
	std::vector<char> header_buffer;
	std::vector<char> initial_buffer;
	std::vector<char> data_buffer;
	std::vector<char> image_buffer;
	int numMessages;
	std::string recvLength;
	std::string recvHeader;

	std::atomic<bool> isDecoding = false;
	std::atomic<bool> newImage = false;
	std::atomic<bool> reconnect = false;

	int camQual;
	bool mainCam;

	static Connection* connection;

public:
	Connection() : io_context(), socket(io_context), remote_endpoint(asio::ip::address::from_string(IP), PORT) {
    	//eth
		socket.open(asio::ip::udp::v4());
		socket.bind(remote_endpoint);

		//serial over usb
        int bdrate=57600; /* 9600 baud */
		char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit

		if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
        {
            printf("Can not open comport\n");
        }
    }
	void Connect();
	void ResizeBuffer(int newSize);

	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);
	void Send(std::string header, std::string * message);

	void Recieve();
	void HandleHandshake();
	std::vector<char>* GetImageBuffer(){ return &image_buffer; };
	bool GetDecoding(){ return isDecoding; }
	bool GetNewImage(){ return newImage; }
	void SetDecoding(bool val){ isDecoding = val; }
	void SetNewImage(bool val){ newImage = val; }
	void Reconnect(){ reconnect = true; }
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};

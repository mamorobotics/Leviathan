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

	static Connection* connection;

public:
	Connection() : io_context(), socket(io_context), remote_endpoint(asio::ip::address::from_string(IP), PORT) {
    	socket.open(asio::ip::udp::v4());
		socket.bind(remote_endpoint);
    }
	void Connect();
	void ResizeBuffer(int newSize);

	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);
	void Send(int header, void * message);

	void Recieve();
	void HandleHandshake();
	std::vector<char>* GetImageBuffer(){ return &image_buffer; };
	bool GetDecoding(){ return isDecoding; }
	bool GetNewImage(){ return newImage; }
	void SetDecoding(bool val){ isDecoding = val; }
	void SetNewImage(bool val){ newImage = val; }
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};

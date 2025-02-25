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
#include "Serial.hpp"
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

/**
 * @class Connection
 * @brief Handles connection with submarine via ethernet.
 */

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
	int maxPacketSize = 1472;

	int cport_nr=24; //GET PORT USING COMMANDS FROM https://www.monocilindro.com/2017/02/19/how-to-connect-arduino-and-raspberry-pi-using-usb-and-c/

	std::atomic<bool> isDecoding = false;
	std::atomic<bool> newImage = false;
	//std::atomic<bool> reconnect = false;

	int camQual;
	bool mainCam;

	static Connection* connection;

public:
	/**
     * @brief Initializes the connection with the submarine, by opening the networking socket and binding
	 * the computer the given address and port.
     */
	Connection() : io_context(), socket(io_context), remote_endpoint(asio::ip::address::from_string(IP), PORT) {
    	//eth
		socket.open(asio::ip::udp::v4());
		socket.bind(remote_endpoint);
    }
	void Connect();
	void ResizeBuffer(int newSize);

	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);

	/**
     * @brief This sends the given message with the corresponding header
	 * @param header The header of the message
	 * @param message The message to be sent
     */
	void Send(std::string header, std::string * message);

	/**
     * @brief This receives the message from the submarine and processes it and handles it according
	 * to the received header. Also triggers sending information. Refer to 
     */
	void Recieve();
	/**
     * @brief This handles the handshake with the submarine, by receiving the handshake message and
	 * checking if it is the correct message. Then it triggers the receive function to conditionally send
	 * the camera number and quality
     */
	void HandleHandshake();
	std::vector<char>* GetImageBuffer(){ return &image_buffer; };
	bool GetDecoding(){ return isDecoding; }
	bool GetNewImage(){ return newImage; }
	void SetDecoding(bool val){ isDecoding = val; }
	void SetNewImage(bool val){ newImage = val; }
	//void Reconnect(){ reconnect = true; }
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};

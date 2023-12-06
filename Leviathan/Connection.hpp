#pragma once
#include <iostream>
#include <tuple>
#include <sstream>

#include <asio.hpp>
#include "UI.hpp"
#include "ConnDetails.hpp"
#include "Management.hpp"

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

	std::vector<char> recv_buffer;
	int numMessages;
	std::string recvLength;
	std::string recvHeader;

	static Connection* connection;

public:
	Connection() : io_context(), socket(io_context), remote_endpoint(asio::ip::address::from_string(IP), PORT) {
        socket.open(asio::ip::udp::v4());
		socket.bind(remote_endpoint);
    }
	void StartReceiving(){
		socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint, 
                [this](const asio::error_code& error, std::size_t bytes_transferred) {
					std::cout << "received smth" << std::endl;
                if (!error) {
                    HandleReceive(bytes_transferred);
                } else {
                    std::cout << "Error Code for receiving: " << error.message() << std::endl;
                }

		});
	}
	void Connect();
	void ResizeBuffer(int newSize);
	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);
	void Send(int header, void * message);
	void HandleReceive(std::size_t bytes_received);
	void HandleHandshake();
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


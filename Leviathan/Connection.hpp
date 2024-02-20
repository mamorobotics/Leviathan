#pragma once
#include <iostream>
#include <tuple>
#include <sstream>
<<<<<<< HEAD
#include <thread>
#include <turbojpeg.h>
=======
>>>>>>> parent of 478f7c4 (Merge pull request #4 from mamorobotics/Networking)

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

	std::array<char, 1024> recv_buffer;

	static Connection* connection;

public:
	Connection() : io_context(), socket(io_context), remote_endpoint(asio::ip::address::from_string(IP), PORT) {
        socket.open(asio::ip::udp::v4());
		socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
		io_context.run();
    }
	void Connect();
	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);
	void Send(int header, void * message, int length);
	void HandleReceive(const asio::error_code& error, std::size_t bytes_received);
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


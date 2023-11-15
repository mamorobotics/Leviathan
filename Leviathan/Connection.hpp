#pragma once
#include <iostream>

#include "UI.hpp"
#include "ConnDetails.hpp"
#include "Management.hpp"

class Connection
{
private:
	ConnDetails connDetails;
	

	static Connection* connection;

public:
	Connection();
	void Connect();
	void SendError(std::string message);
	void SendWarning(std::string message);
	void SendTelemetry(std::string key, std::string value);
	void Send(int header, std::string message);
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


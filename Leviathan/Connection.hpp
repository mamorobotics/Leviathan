#pragma once

#include <asio.hpp>

#include "UI.hpp"

class Connection
{
private:
	static Connection* connection;

public:
	Connection();
	void Connect();
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


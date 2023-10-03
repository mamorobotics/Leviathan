#pragma once

#include <asio.hpp>

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
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


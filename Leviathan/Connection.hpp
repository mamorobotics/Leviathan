#pragma once
#include "UI.hpp"

class Connection
{
private:
	static Connection* connection;
	UI* ui;

public:
	Connection();
	Connection(const Connection& obj) = delete;
	~Connection();
	static Connection* Get();
};


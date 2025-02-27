#pragma once

#include <string>

/**
 * @class ConnDetails
 * @brief Handles the details of the current connection.
 */
class ConnDetails
{
public:
	std::string selfIP;
	std::string selfPort;
	std::string connectedIP;
	std::string connectedPort;
	std::string connectionStatus;
};


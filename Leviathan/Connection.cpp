#include "Connection.hpp"

#define SENDSIZE 1024
#define PORT 8080
#define IP "192.168.1.1"

Connection::Connection()
{
}

void Connection::Connect()
{
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != NO_ERROR) {
		connDetails.connectionStatus = "Failed to start socket subroutine";
		UI::Get()->PublishOutput("WSAStartup failed with error number: \n\t" + res, LEV_CODE::CONN_ERROR);
	}

	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET) {
		connDetails.connectionStatus = "Failed to create socket";
		UI::Get()->PublishOutput("Socket creation failed with error: \n\t" + WSAGetLastError());
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = inet_addr(IP);
	
	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::Send(std::string message) 
{
	char sendBuf[SENDSIZE];
	strcpy(sendBuf, message.c_str());
	int clientResult = sendto(clientSocket, sendBuf, SENDSIZE-1, 0, (SOCKADDR*)&clientAddr, (int)sizeof(clientAddr));
}

Connection::~Connection()
{
}

Connection* Connection::Get()
{
	if (connection == nullptr)
		connection = new Connection();

	return connection;
}

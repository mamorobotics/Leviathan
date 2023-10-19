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
	PVOID buf;
	clientAddr.sin_addr.s_addr = inet_pton(AF_INET, IP, &buf);
	
	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::Send(int header, std::string message) 
{
	int clientResult;
	char sizeBuf[32];
	strcpy_s(sizeBuf, std::to_string(sizeof(message)).c_str());
	clientResult = sendto(clientSocket, sizeBuf, 32, 0, (SOCKADDR*)&clientAddr, (int)sizeof(clientAddr));

	char headerBuf[32];
	strcpy_s(headerBuf, std::to_string(sizeof(header)).c_str());
	clientResult = sendto(clientSocket, headerBuf, 32, 0, (SOCKADDR*)&clientAddr, (int)sizeof(clientAddr));

	char messageBuf[sizeof(message)];
	strcpy_s(messageBuf, message.c_str());
	clientResult = sendto(clientSocket, messageBuf, sizeof(message), 0, (SOCKADDR*)&clientAddr, (int)sizeof(clientAddr));
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

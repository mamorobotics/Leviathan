#include "Connection.hpp"

void Connection::Connect()
{
    //maybe stuffs
}

void Connection::ResizeBuffer(int newSize)
{
    recv_buffer.resize(newSize);
}

void Connection::SendError(std::string message)
{
	Send(2, &message);
}

void Connection::SendWarning(std::string message)
{
	Send(1, &message);
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send(3, &message);
}

void Connection::Send(int header, void * message)
{
    std::string msgLength = std::to_string(sizeof(message));
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);
    std::cout << "Sent length --- " << lenSent << "\n";

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);
    std::cout << "Sent header --- " << headerSent << "\n";

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    std::cout << "Sent message --- " << msgSent << "\n";
}

void Connection::Recieve() 
{
    UI* gui = UI::Get();
    while (true)
    {
        asio::error_code error;
        ResizeBuffer(32);
        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        int size = stoi(std::string(recv_buffer.data()));

        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        int header = stoi(std::string(recv_buffer.data()));

        ResizeBuffer(size);
        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        std::string message = recv_buffer.data();
        
        //std::cout << header << " : " << message << "\n";

        if(header==4){
            LoadTextureFromBuffer::LoadTexture(recv_buffer.data(), gui->getCameraTexture());
        }
    }
}

void Connection::HandleHandshake(){
    ResizeBuffer(32);
    asio::error_code error;
    socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
    if(recv_buffer.data() != NULL){
        if(std::string(recv_buffer.data()) != "0110"){
            std::cout << "[WARNING] Handshake with client failed" << std::endl;
        }else{
            std::cout << "got handshake" << std::endl;
            connDetails.connectedIP = remote_endpoint.address().to_string();
            connDetails.connectedPort = "8080";
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
            Recieve();
        }
    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
    }
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

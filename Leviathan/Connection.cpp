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

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
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
        
        if(header==4){
            const unsigned char* src_data = reinterpret_cast<const unsigned char*>(recv_buffer.data());
            int width, height, im_type;
            char* decmp_data = reinterpret_cast<char*>(jpgd::decompress_jpeg_image_from_memory(src_data, sizeof(src_data), &width, &height, &im_type, 3, 0));
            LoadTextureFromBuffer::LoadTexture(decmp_data, gui->getCameraTexture());
            gui->setCameraWidth(width);
            gui->setCameraHeight(height);
            gui->PublishOutput("Recieved photo");
            gui->PublishTelemetry("Photo Width", std::to_string(width));
            gui->PublishTelemetry("Photo Height", std::to_string(height));
        }
    }
}

void Connection::HandleHandshake(){
    UI* gui = UI::Get();
    ResizeBuffer(32);
    asio::error_code error;
    socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
    if(recv_buffer.data() != NULL){
        std::cout << recv_buffer.data() << std::endl;
        if(std::string(recv_buffer.data()) != "0110"){
            gui->PublishOutput("Handshake with client failed", LEV_CODE::CONN_ERROR);
        } else { 
            gui->PublishOutput("Succesful handshake with client", LEV_CODE::CLEAR);
            connDetails.connectedIP = remote_endpoint.address().to_string();
            connDetails.connectedPort = "8080";
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
            Recieve();
        }
    }else{
        gui->PublishOutput("Error Code for receiving: " + error.message(), LEV_CODE::CONN_ERROR);
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

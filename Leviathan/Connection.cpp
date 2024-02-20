#include "Connection.hpp"

void Connection::Connect()
{
	UI::Get()->setConnectionDetails(connDetails);
}

void Connection::SendError(std::string message)
{
	Send(2, &message, message.size());
}

void Connection::SendWarning(std::string message)
{
	Send(1, &message, message.size());
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send(3, &message, message.size());
}

void Connection::Send(int header, void * message, int length)
{
    std::stringstream stream;

    std::string msgLength = std::to_string(length);
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);

    auto msgTup = std::make_tuple(header, message);

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
}

void Connection::HandleReceive(const asio::error_code& error, std::size_t bytes_received){
    if(!error){

<<<<<<< HEAD
        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        int header = stoi(std::string(recv_buffer.data()));

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        ResizeBuffer(0);
        int total_size = 0;
        while (total_size < size)
        {
            std::vector<char> buf;
            buf.resize((size - total_size) > 65500 ? 65500 : (size - total_size));
            socket.receive_from(asio::buffer(buf), remote_endpoint, 0, error);
            total_size += (size - total_size) > 65500 ? 65500 : (size - total_size);
            recv_buffer.insert(recv_buffer.end(), buf.begin(), buf.end());
        }

        const unsigned char* message = reinterpret_cast<const unsigned char*>(recv_buffer.data());

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);
        
        if(header==4){
            int width, height, im_type;;
            //LoadTextureFromBuffer::LoadTexture(decmp_data, 512, 512, gui->getCameraTexture());
        }
    }
}

void Connection::HandleHandshake(){
    UI* gui = UI::Get();
    ResizeBuffer(32);
    asio::error_code error;
    socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
    if(recv_buffer.data() != NULL){
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
=======
    }else{
        std::cout << error.message();
>>>>>>> parent of 478f7c4 (Merge pull request #4 from mamorobotics/Networking)
    }

    socket.async_receive(asio::buffer(recv_buffer), std::bind(&Connection::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
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

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
    int i = 0;
    int j=0;
    bool failedFrame = false;
    while (true)
    {
        j++;
        if(j==50){return;}
        failedFrame = false;
        ResizeBuffer(0);
        std::cout<<"loop"<<std::endl;
        asio::error_code error;
        ResizeBuffer(32);
        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        int size;
        try{
            size = stoi(std::string(recv_buffer.data()));
        }catch(const std::invalid_argument& e){
            std::cerr << "Invalid argument(size): " << e.what() << std::endl;
            std::cout << "size is(size): " << recv_buffer.size() << std::endl;
            std::cout << "data is(size): " << std::string(recv_buffer.data()) << std::endl;

            failedFrame = true;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        ResizeBuffer(0);
        ResizeBuffer(32);

        socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
        int header;
        try{
            header = stoi(std::string(recv_buffer.data()));
        }catch(const std::invalid_argument& e){
            std::cerr << "Invalid argument(header): " << e.what() << std::endl;
            std::cout << "size is(header): " << recv_buffer.size() << std::endl;
            std::cout << "data is(header): " << std::string(recv_buffer.data()) << std::endl;
            std::cout << "data is(size): " << size << std::endl;

            failedFrame = true;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        ResizeBuffer(0);
        int total_size = 0;
        std::string endCase = "";
        while (endCase.compare("") == 0)
        {
            std::vector<char> buf;
            buf.resize((size - total_size) > 65500 ? 65500 : (size - total_size));
            socket.receive_from(asio::buffer(buf), remote_endpoint, 0, error);
            total_size += (size - total_size) > 65500 ? 65500 : (size - total_size);
            recv_buffer.insert(recv_buffer.end(), buf.begin(), buf.end());

            if(std::string(std::vector<char>(recv_buffer.end()-4, recv_buffer.end()).data()).compare("0110")){
                endCase = "f";
                std::cout << "Found end case" << std::endl;
            }

            std::cout << "size: " << size << std::endl;
            std::cout << "total size: " << total_size << std::endl;  
        }

        const unsigned char* message = reinterpret_cast<const unsigned char*>(recv_buffer.data());

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        if(failedFrame){continue;}

        // std::cout << "data is(header): " << std::string(recv_buffer.data()) << std::endl;
        // std::cout << "data is(size): " << size << std::endl;        
        
        if(header==4){
            tjhandle decompressor = tjInitDecompress();

            int width, height, im_type;
            unsigned char *imageData = new unsigned char[512*512*3];

            if(!imageData){
                fprintf(stderr, "Error allocating memory for image data\n");
            }

            int result = tjDecompress2(decompressor, message, size, imageData, 512, 0, 512, TJPF_RGB, TJFLAG_FASTDCT);

            if(result!=0){
                fprintf(stderr, "Error decompressing JPEG image: %s\n", tjGetErrorStr());
                delete[] imageData;
            }

            tjDestroy(decompressor);

            //std::cout<<"finished decomp"<<std::endl;
            if(i==0){
                printf("%s", imageData);
                //std::cout<<imageData<<std::endl;
                i++;
            }
            //printf("%s", imageData);
        
            LoadTextureFromBuffer::LoadTexture(imageData, 512, 512, gui->getCameraTexture());
            //std::cout<<"loaded texture"<<std::endl;
            tjFree(imageData);

            ResizeBuffer(0);
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

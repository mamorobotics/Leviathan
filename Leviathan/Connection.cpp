#include "Connection.hpp"

void Connection::SendError(std::string message)
{
	Send("2", &message);
}

void Connection::SendWarning(std::string message)
{
	Send("1", &message);
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send("3", &message);
}

void Connection::Send(std::string headers, std::string * message)
{
    //auto initSent = socket.send_to(asio::buffer("0220", 32), remote_endpoint, 0);
    std::string messageData = *message;
    std::string initialMsg = std::to_string(messageData.size()) + "!" + headers;
    initialMsg.append(32-initialMsg.size(), ' ');
    auto initSent = socket.send_to(asio::buffer(initialMsg, 32), remote_endpoint, 0);

    while(sizeof(messageData) > maxPacketSize){
        std::string temp = messageData.substr(0, maxPacketSize);
        messageData = messageData.substr(maxPacketSize);
        auto msgSent = socket.send_to(asio::buffer(temp, maxPacketSize), remote_endpoint, 0);
    }
    if(sizeof(messageData) != 0){
        auto msgSent = socket.send_to(asio::buffer(messageData, messageData.size()), remote_endpoint, 0);
    }
}

void Connection::Recieve() 
{
    Serial* ser = Serial::Get();
    UI* gui = UI::Get();
    camQual = gui->getCameraQuality();
    mainCam = gui->isMainCamera();
    Controller* controller = new Controller(1);
    int i = 0;
    int j=0;
    bool failedFrame = false;
    bool camChange = false;
    bool oneIter = false;

    while (true)
    {
        j++;
        failedFrame = false;
        asio::error_code error;
        initial_buffer.resize(32);

        socket.receive_from(asio::buffer(initial_buffer), remote_endpoint, 0, error);

        std::string msg = reinterpret_cast<char*>(initial_buffer.data());
        int index = msg.find("!");
        std::string sizeStr = msg.substr(0, index);
        int size = stoi(sizeStr);
        std::string headerStr = msg.substr(index + 1, msg.length() - (sizeStr.length() + 1));
        int header = stoi(headerStr);

        data_buffer.resize(0);
        if(!isDecoding && header == 4){
            image_buffer.resize(size);
        }

        int total_size = 0;
        while (total_size < size)
        {
            std::vector<char> buf;
            buf.resize((size - total_size) > maxPacketSize ? maxPacketSize : (size - total_size));
            socket.receive_from(asio::buffer(buf), remote_endpoint, 0, error);
            total_size += (size - total_size) > maxPacketSize ? maxPacketSize : (size - total_size);
            data_buffer.insert(data_buffer.end(), buf.begin(), buf.end());
        }

        if(!isDecoding && header == 4){
            image_buffer = data_buffer;
        }

        if (error.value()) gui->PublishOutput(error.message(), LEV_CODE::CONN_ERROR);

        if(header == 4 && !failedFrame && !isDecoding && !gui->isCameraPaused()){
            newImage = true;
        }
        
        if(header == 2)
        {
            gui->PublishOutput(std::string(data_buffer.data()), LEV_CODE::GENERAL_ERROR);
        }

        if(header == 1)
        {
            gui->PublishOutput(std::string(data_buffer.data()), LEV_CODE::WARNING);
        }

        if(header == 3)
        {
            std::string msg = std::string(data_buffer.data());
            int index = msg.find("!");
            std::string id = msg.substr(0, index);
            std::string value = msg.substr(index + 1, msg.length() - (id.length() + 1));
            gui->PublishTelemetry(id, value);
        }

        data_buffer.resize(0);

        //SENDING STARTS HERE...

        std::string msgN = "";
        std::string headers = "";
        
        if(camQual != gui->getCameraQuality()){
            std::string newQual = "qual!" + std::to_string(gui->getCameraQuality());
            camQual = gui->getCameraQuality();
            headers+="6?";
            msgN+=newQual;
            camChange = true;
        }
        if(mainCam != gui->isMainCamera()){
            std::string val;
            if(gui->isMainCamera()){val = "1";}
            else{val = "0";}
            std::string newCam = "cam!" + val;
            mainCam = gui->isMainCamera();
            headers+="6?";
            if(camChange){newCam = newCam+"?";}
            msgN+=newCam;
            camChange = true;
        }
        
        if(camChange){
            //ControllerValues* controllerValues = controller->GetControllerValues();
            //msgN += controllerValues->toString();
            //headers+="5";
            Send(headers, &msgN);
            camChange = false;
        }

        // if(!oneIter){
        //     std::cout<<"once"<<std::endl;
        //     std::thread serialThread(&Serial::SendController, ser);
        //     //std::thread radioThread(&Radio::startRadioReceive, rad);
	    //     serialThread.detach();

        //     oneIter = true;
        // }

        // ControllerValues* controllerValues = controller->GetControllerValues();
        // const char*  msgSerial = controllerValues->toString().c_str();

        // RS232_cputs(cport_nr, msgSerial); // sends string on serial
        // printf("Sent to Arduino: '%s'\n", msgSerial);
    }
}

void Connection::HandleHandshake(){
    UI* gui = UI::Get();
    gui->PublishOutput("Starting handshake with client", LEV_CODE::CLEAR);
    data_buffer.resize(0);
    data_buffer.resize(32);
    asio::error_code error;
    socket.receive_from(asio::buffer(data_buffer), remote_endpoint, 0, error);
    if(data_buffer.data() != NULL){
        if(std::string(data_buffer.data()) != "0110"){
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

#include "Serial.hpp"

void Serial::SendControllerAndGetFloatData() 
{
    Controller* controller = new Controller(1);    
	char buffer[128];
	int profile = 0;
    int n;
	bool notEnd = true;

	while(true){
		notEnd = true;
		ControllerValues* controllerValues = controller->GetControllerValues();
		std::string msgStr = "<" + controllerValues->toStringPartial() + ">";
		//std::cout<<"controller sent"<<std::endl;
		//std::cout<<msgStr<<std::endl;

		RS232_cputs(cport_nr, msgStr.c_str()); // sends string on serial
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// while(notEnd){
		// 	n = RS232_PollComport(cport_nr, (unsigned char*)buffer, sizeof(buffer) - 1);
		// 	while (n <= 0) {
		// 		n = RS232_PollComport(cport_nr, (unsigned char*)buffer, sizeof(buffer) - 1);
		// 		//std::cout<<"waiting for data"<<std::endl;
		// 	}
		// 	buffer[n] = 0; // Null-terminate the string
		// 	printf("Received: %s\n", buffer);
		// 	std::string buffer_str = buffer;
		// 	std::string data = buffer_str.substr(0,n);
		// 	std::cout<<"'"<<data<<"'"<<std::endl;

		// 	if(buffer[0] == '~'){
		// 		notEnd = false;
		// 		break;
		// 	}

		// 	if(data[0] == 'R' && data[1] == 'N'){
		// 		floatOutputs.push_back(data);

		// 		//Data Processing
		// 		std::vector<std::string> data_split;
		// 		std::stringstream ss(data);
		// 		std::string token;
		// 		while(std::getline(ss, token, '!')){
		// 			data_split.push_back(token);
		// 		}

		// 		// std::cout<<"Time "<<data_split[1]<<std::endl;
		// 		// std::cout<<"Depth "<<data_split[2]<<std::endl;

		// 		if(profile == 0){
		// 			depthProfileOne[0][index] = std::stof(data_split[1]);
		// 			depthProfileOne[1][index] = std::stof(data_split[2].substr(0,3));
		// 			dataPts1 += 1;
		// 			dataPts1 = std::min(dataPts1, 128);
		// 		}else{
		// 			depthProfileTwo[0][index] = std::stof(data_split[1]);
		// 			depthProfileTwo[1][index] = std::stof(data_split[2].substr(0,3));
		// 			dataPts2 += 1;
		// 			dataPts2 = std::min(dataPts2, 128);
		// 		}

		// 		index += 1;
		// 		index %= 128;
		// 	} else if(data[0] == 'E'){
		// 		profile++;
		// 		profile %= 2;
		// 	}
		// }
	}
}

Serial::~Serial()
{
}

Serial* Serial::Get()
{
	if (serial == nullptr)
		serial = new Serial();

	return serial;
}

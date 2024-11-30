#include "Serial.hpp"

void Serial::SendControllerAndGetFloatData() 
{
    Controller* controller = new Controller(1);    
	char buffer[128];
    int n;
	bool notEnd = true;

	while(true){
		notEnd = true;
		ControllerValues* controllerValues = controller->GetControllerValues();
		std::string msgStr = "<" + controllerValues->toStringPartial() + ">";

		RS232_cputs(cport_nr, msgStr.c_str()); // sends string on serial

		while(notEnd){
			n = RS232_PollComport(port_number, (unsigned char*)buffer, sizeof(buffer) - 1);
			while (n <= 0) {
				n = RS232_PollComport(port_number, (unsigned char*)buffer, sizeof(buffer) - 1);
			}
			buffer[n] = 0; // Null-terminate the string
			printf("Received: %s\n", buffer);

			if(buffer == '. -. -..'){
				notEnd = false;
				break;
			}

			floatOutputs.push_back(buffer);

			//Data Processing
			// std::vector<std::string> data;
			// for (const auto &token : std::views::split(receivedData, delimiter)) {
			// 	data.push_back(std::string(token.begin(), token.end()));
			// }

			// if(profile == 1){
			// 	depthProfileOne[0][index] = data[1];
			// 	depthProfileOne[1][index] = data[2];
			// }else{
			// 	depthProfileTwo[0][index] = data[1];
			// 	depthProfileTwo[1][index] = data[2];
			// }

			// index += 1;
			// index %= 128;
		}
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

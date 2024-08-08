#include "Serial.hpp"

void Connection::SendController() 
{
    Controller* controller = new Controller(1);    
	while(true){
		ControllerValues* controllerValues = controller->GetControllerValues();
		
		const char*  msgSerial = controllerValues->toString().c_str();

		RS232_cputs(cport_nr, msgSerial); // sends string on serial
		printf("Sent to Arduino: '%s'\n", msgSerial);
	}
}

Serial::~Connection()
{
}

Serial* Serial::Get()
{
	if (connection == nullptr)
		connection = new Serial();

	return connection;
}

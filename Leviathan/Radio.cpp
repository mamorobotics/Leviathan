#include "Radio.hpp"

using namespace std;

#define PIN_CE  17
#define PIN_CSN 0

uint8_t pipeNumber;
uint8_t payloadSize;

void startRadioReceive() {
  while (true) {
    if (radio.available(&pipeNumber)) {

      payloadSize = radio.getDynamicPayloadSize();
      char payload[payloadSize];
      string receivedData;
      radio.read(&payload, sizeof(payload));

      for (uint8_t i = 0; i < payloadSize; i++) {
          receivedData += payload[i];
      }
      cout << "Pipe : " << (int) pipeNumber << " ";
      cout << "Size : " << (int) payloadSize << " ";
      cout << "Data : " << receivedData << endl;

      delay(100);
    }
  }
  return 0;
}

Radio::~Radio()
{
}

Radio* Radio::Get()
{
	if (radio == nullptr)
		radio = new Radio();

	return radio;
}

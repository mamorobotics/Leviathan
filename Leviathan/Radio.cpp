#include "Radio.hpp"

using namespace std;

#define PIN_CE  17
#define PIN_CSN 0

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
		floatOutputs.push_back(receivedData);

		//Data Processing
		std::vector<std::string> data;
		for (const auto &token : std::views::split(receivedData, delimiter)) {
			tokens.push_back(std::string(token.begin(), token.end()));
		}

		std::array<std::atomic<float>, 2> atomicFloats;
		atomicFloats[0] = data[1];
		atomicFloats[1] = data[2];

		if(profile == 1){
			depthProfileOne.push_back(atomicFloats);
		}else{
			depthProfileTwo.push_back(atomicFloats);
		}

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

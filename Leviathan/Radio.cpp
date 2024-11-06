#include "Radio.hpp"

Radio::Radio(){
	
}

void Radio::startRadioReceive() {
	RF24 radioRF(11, 24); //CE, CSN

	radioRF.begin();
	radioRF.setChannel(5);
	radioRF.setPALevel(RF24_PA_HIGH);
	radioRF.setDataRate(RF24_1MBPS);
	radioRF.enableDynamicPayloads();

	radioRF.openReadingPipe(0, 0x7878787878LL);

	radioRF.printDetails();

	radioRF.startListening();

	while (true) {
		if (radioRF.available(&pipeNumber)) {
			payloadSize = radioRF.getDynamicPayloadSize();
			char payload[payloadSize];
			string receivedData;
			radioRF.read(&payload, sizeof(payload));

			for (uint8_t i = 0; i < payloadSize; i++) {
				receivedData += payload[i];
			}
			cout << "Pipe : " << (int) pipeNumber << " ";
			cout << "Size : " << (int) payloadSize << " ";
			cout << "Data : " << receivedData << endl;
			floatOutputs.push_back(receivedData);

			//Data Processing
			// std::vector<std::string> data;
			// for (const auto &token : std::views::split(receivedData, delimiter)) {
			// 	data.push_back(std::string(token.begin(), token.end()));
			// }

			// std::array<std::atomic<float>, 2> atomicFloats;
			// atomicFloats[0] = data[1];
			// atomicFloats[1] = data[2];

			// if(profile == 1){
			// 	depthProfileOne[index] = atomicFloats;
			// }else{
			// 	depthProfileTwo[index] = atomicFloats;
			// }

			// index += 1;
			// index %= 128;
		}
	}
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

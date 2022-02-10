#include <Arduino.h>
#include <SPI.h>

#include "RF24.h"

/*

CE   (yellow)  7
SCK  (green)  13
MISO (purple) 12

CSN  (orange)  8
MOSI (blue)   11
*/

const uint32_t pin_ce = 7;
const uint32_t pin_csn = 8;

RF24 radio(pin_ce, pin_csn, RF24_SPI_SPEED); // CE, CSN

uint8_t radio_local[6] = "local";
uint8_t radio_remote[6] = "remot";

void setup() {
	Serial.begin(115200);

	// start radio.
	if(! radio.begin()) {
		Serial.println(F("radio failed to start."));
		while(1)
			;
	}

	// configure radio.
	radio.setPALevel(RF24_PA_LOW);
	radio.setAutoAck(false);
	radio.setCRCLength(RF24_CRC_8);
	radio.setPayloadSize(32);

	// open read and write pipes.
	#ifdef RADIO_HOST
	radio.openWritingPipe(radio_remote);
	radio.openReadingPipe(1, radio_local);
	#else
	radio.openWritingPipe(radio_local);
	radio.openReadingPipe(1, radio_remote);
	#endif

	#ifdef RADIO_HOST
	radio.stopListening();
	Serial.println(F("started host radio."));
	#else
	radio.startListening();
	Serial.println(F("started robot radio."));
	#endif
}

// #define COMP

#ifdef RADIO_HOST

uint8_t payload[32] = "Hello, world.";

void loop() {
	bool ret = radio.write(payload, sizeof(payload));

	if(! ret) {
		Serial.println("failed to send payload.");
	}
	else {
		Serial.print("sent \"");
		Serial.print((char*) payload);
		Serial.println("\".");
	}

	delay(1000);
}

#else

uint8_t payload[32] = {0};

void loop() {
	if(radio.available()) {
		radio.read((void*) payload, sizeof(payload));

		Serial.print("read \"");
		Serial.print((char*) payload);
		Serial.println("\".");
	}
	else {
		Serial.println("no packets read.");
	}

	delay(1000);
}

#endif

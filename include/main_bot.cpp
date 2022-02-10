#include <Arduino.h>
#include <SPI.h>

#include "RF24.h"

#include "bot.h"

/*
 * CE   (white)   7
 * SCK  (grey)   13
 * MISO (purple) 12
 *
 * CSN  (green)   8
 * MOSI (blue)   11
 */

// radio pins:
static const unsigned int pin_ce = 7;
static const unsigned int pin_csn = 8;

// right motor pins:
static unsigned int pin_ena;
static unsigned int pin_in1;
static unsigned int pin_in2;

// left motor pins:
static unsigned int pin_enb;
static unsigned int pin_in3;
static unsigned int pin_in4;

// auto:
static uint32_t auto_drive_ms;

static bool is_init = false;

void setup() {
	// start serial communications.
	Serial.begin(115200);
	PRINTLN("ROBOT SETUP:");

	PRINT("  radio status...");

	// start radio transmission.
	if( ! bot::net::init(pin_ce, pin_csn, "TDbot", "TDcon")) {
		PRINTLN("BAD");
		return;
	}
	else {
		PRINTLN("OK");
	}
}

void loop() {
	uint8_t buf[32];

	// continue to receive message from the host.
	if(bot::net::recv(buf, sizeof(buf))) {
		switch(buf[0]) {
		case PKT_INPUT_ID: {
			if( ! is_init) {
				break;
			}

			bot::pkt::input* pkt = (bot::pkt::input*)buf;

			PRINT("driving arcade at ");
			PRINT(pkt->drive_y);
			PRINT(", ");
			PRINT(pkt->drive_x);
			PRINTLN(".");

			bot::drive::arcade(pkt->drive_y, pkt->drive_x);
		} break;

		case PKT_INIT_ID: {
			bot::pkt::init* pkt = (bot::pkt::init*)buf;

			// set pins to init packet values.
			pin_ena = pkt->pin_ena;
			pin_in1 = pkt->pin_in1;
			pin_in2 = pkt->pin_in2;

			pin_enb = pkt->pin_enb;
			pin_in3 = pkt->pin_in3;
			pin_in4 = pkt->pin_in4;

			// set next ok packet to start ok packet.

			// initialize drive with pin values:
			bot::drive::init(pin_ena, pin_in1, pin_in2, pin_enb, pin_in3, pin_in4);

			PRINT("initializing drive { ");
			PRINT(pin_ena);
			PRINT(", ");
			PRINT(pin_in1);
			PRINT(", ");
			PRINT(pin_in2);
			PRINT(", ");
			PRINT(pin_enb);
			PRINT(", ");
			PRINT(pin_in3);
			PRINT(", ");
			PRINT(pin_in4);
			PRINTLN(" }");

			is_init = true;

		} break;


		case PKT_START_ID: {
			if( ! is_init) {
				break;
			}

			PRINTLN("got start packet.");

			bot::pkt::start* pkt = (bot::pkt::start*)buf;
			unsigned long current = millis();

			PRINT("starting autonomous for ");
			PRINT(pkt->auto_drive_ms);
			PRINT(" ms at a speed of ");
			PRINT(pkt->auto_left_speed);
			PRINT(", ");
			PRINT(pkt->auto_right_speed);
			PRINTLN(".");

			do {
				bot::drive::tank(pkt->auto_left_speed, pkt->auto_right_speed);
			}
			while(millis() - current <= pkt->auto_drive_ms);

		} break;

		default: break;
		}
	}
}

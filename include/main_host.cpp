#include <Arduino.h>
#include <SPI.h>

#include "RF24.h"

#include "bot.h"

/*
 *
 * CE   (yellow)  7
 * SCK  (green)  13
 * MISO (purple) 12
 *
 * CSN  (orange)  8
 * MOSI (blue)   11
 */

const unsigned int pin_ce = 7;
const unsigned int pin_csn = 8;

const unsigned int pin_vrx1 = A0;
const unsigned int pin_vry1 = A1;
const unsigned int pin_sw1 = 6;

bot::pkt::init pkt_init = {
	PKT_INIT_ID,

	// right motor pins:
	3, // ena
	2, // in1
	4, // in2

	// left motor pins:
	9, // enb
	5, // in3
	6  // in4
};

bot::pkt::start pkt_start = {
	PKT_START_ID,

	// auto time in milliseconds:
	5000,

	// left and right speeds:
	0.5,
	0.5
};

void setup() {
	// start serial communications.
	Serial.begin(115200);
	PRINTLN("CONTROLLER SETUP:");

	PRINT("radio status...");

	// start radio transmission.
	if( ! bot::net::init(pin_ce, pin_csn, "TDcon", "TDbot")) {
		PRINTLN("BAD");
		return;
	}
	else {
		PRINTLN("OK");
	}

	bot::input::init_stick(0, pin_vrx1, pin_vry1, pin_sw1);
	// bot::input::init_stick(1, pin_vrx2, pin_vry2, pin_sw2);

	PRINT("sending robot init packet...");

	// wait for robot to confirm receving init packet.
	while( ! bot::net::send(&pkt_init, sizeof(pkt_init), true)) {
		delay(250);
	}

	PRINTLN("sent!");

	// wait for the joystick button to be pressed and released before starting
	// the robot.
	while( ! bot::input::get_stick_up(0)) {
		bot::input::update();
	}

	PRINT("sending robot start packet...");

	// wait for robot to confirm receving start packet.
	if( ! bot::net::send(&pkt_start, sizeof(pkt_start), true)) {
		PRINTLN("err: could not send start packet.");
		return;
	}

	PRINTLN("sent!");

	PRINTLN("starting control loop:");
}

bot::pkt::input pkt_input = {
	PKT_INPUT_ID,
	0.0f,
	0.0f
};

void loop() {
	// get input packet values.
	// pkt_input.drive_y = (float)random(-1000, 1000) / 1000.0f;
	// pkt_input.drive_x = (float)random(-1000, 1000) / 1000.0f;

	bot::input::update();

	pkt_input.drive_y = bot::input::get_stick_y(0);
	pkt_input.drive_x = bot::input::get_stick_x(0);

	if(bot::input::get_stick_down(0)) {
		PRINTLN("STICK DOWN.");
	}

	// send input packet.
	bot::net::send(&pkt_input, sizeof(pkt_input));

	// sleep for a lil' bit.
	delay(10);
}

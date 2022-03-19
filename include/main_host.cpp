#include "RF24.h"
#include "bot.hpp"

#include <Arduino.h>
#include <SPI.h>

/* NRF24l01+ RADIO
 *
 * CE   (yellow)  9
 * SCK  (green)  13
 * MISO (purple) 12
 *
 * CSN  (orange) 10
 * MOSI (blue)   11
 */

const unsigned int pin_ce = 9;
const unsigned int pin_csn = 10;

/* JOYSTICK RIGHT
 *
 * VRX  (brown)  A0
 * VRY  (grey)   A1
 * SW   (purple)  7
 */

const unsigned int pin_vrx1 = A0;
const unsigned int pin_vry1 = A1;
const unsigned int pin_sw1 = 7;

/* JOYSTICK LEFT
 *
 * VRX  (yellow)  A2
 * VRY  (yellow)  A3
 */

const unsigned int pin_vrx2 = A2;
const unsigned int pin_vry2 = A3;

bot::pkt::init pkt_init = {.id = PKT_INIT_ID,

						   // right motor pins:
						   .mtr1 = {.pin_ena = 10,
									.pin_in1 = A0,
									.pin_in2 = A1,

									// left motor pins:
									.pin_enb = 10,
									.pin_in3 = A2,
									.pin_in4 = A3}};

bot::pkt::start pkt_start = {.id = PKT_START_ID,

							 // auto time in milliseconds:
							 .auto_drive_ms = 5000,

							 // left and right speeds:
							 .auto_left_speed = 1.0,
							 .auto_right_speed = 1.0};

void setup() {
	// start serial communications.
	Serial.begin(115200);
	PRINTLN("CONTROLLER SETUP:");

	PRINT("radio status...");

	// start radio transmission.
	if(! bot::net::init(pin_ce, pin_csn, "TDcon", "TDbot")) {
		PRINTLN("BAD");
		return;
	}
	else {
		PRINTLN("OK");
	}

	bot::input::init_stick(0, pin_vrx1, pin_vry1, pin_sw1);
	bot::input::init_stick(1, pin_vrx2, pin_vry2, -1);

	PRINT("sending robot init packet...");

	// wait for robot to confirm receving init packet.
	while(! bot::net::send(&pkt_init, sizeof(pkt_init), true)) {
		delay(250);
	}

	PRINTLN("sent!");

	// wait for the joystick button to be pressed and released before starting
	// the robot.
	while(! bot::input::get_stick_up(0)) {
		bot::input::update();
	}

	PRINT("sending robot start packet...");

	// wait for robot to confirm receving start packet.
	if(! bot::net::send(&pkt_start, sizeof(pkt_start), true)) {
		PRINTLN("err: could not send start packet.");
		return;
	}

	PRINTLN("sent!");

	delay(pkt_start.auto_drive_ms);

	PRINTLN("starting control loop:");
}

bot::pkt::input pkt_input = {PKT_INPUT_ID, 0.0f, 0.0f};

static float stick_ramp(float value) {
	// deadzone value.
	if(value < 0.08f) {
		return 0.0f;
	}

	// ramp based on bezier curve. function:
	// C_{urve}(a,b,t)=3at+3t^2b+t^3-6at^2+3at^3-3t^3b
	return 3.88 * (value * value * value) - 5.46 * (value * value) +
		   2.58 * value;
}

void loop() {

	// TODO make pressing one joystick button starts autonomous while the other
	//      just starts user driving.

	// get input packet values.
	bot::input::update();

	pkt_input.drive_y = stick_ramp(bot::input::get_stick_y(0));
	pkt_input.drive_x = stick_ramp(bot::input::get_stick_x(1));

	if(bot::input::get_stick_down(0)) {
		PRINTLN("STICK DOWN.");
	}

	// TODO consider if input packet should be reliable or not.
	// send input packet.
	if(! bot::net::send(&pkt_input, sizeof(pkt_input), true)) {
		PRINTLN("lost connection to the robot.");
	}

	// sleep for a lil' bit.
	delay(10);
}

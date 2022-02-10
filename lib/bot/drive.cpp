#include "drive.h"

#include <Arduino.h>

namespace bot {
namespace drive {

static bool is_right_init;
static bool is_left_init;

static unsigned int pin_ena;
static unsigned int pin_in1;
static unsigned int pin_in2;

static unsigned int pin_enb;
static unsigned int pin_in3;
static unsigned int pin_in4;

void init(int pin_ena, int pin_in1, int pin_in2,
		  int pin_enb, int pin_in3, int pin_in4) {
	init_right(pin_ena, pin_in1, pin_in2);
	init_left(pin_enb, pin_in3, pin_in4);
}

void init_left(int pin_enb, int pin_in3, int pin_in4) {
	pinMode(pin_ena, OUTPUT);
	pinMode(pin_in1, OUTPUT);
	pinMode(pin_in2, OUTPUT);

	analogWrite(pin_ena, 0);
	is_left_init = true;
}
void init_right(int pin_ena, int pin_in1, int pin_in2) {
	pinMode(pin_enb, OUTPUT);
	pinMode(pin_in3, OUTPUT);
	pinMode(pin_in4, OUTPUT);

	analogWrite(pin_enb, 0);
	is_right_init = true;
}

void set_left(float speed) {
	if(! is_left_init) {
		return;
	}

	if(speed == 0.0f) {
		digitalWrite(pin_in1, LOW);
		digitalWrite(pin_in2, LOW);
		analogWrite(pin_ena, 0);

		return;
	}
	else if(speed < 0.0f) {
		// set direction of the motor rotation to inverse.
		digitalWrite(pin_in1, LOW);
		digitalWrite(pin_in2, HIGH);

		// make speed absolute.
		speed *= -1.0f;
	}
	else {
		// set direction of the motor rotation to normal.
		digitalWrite(pin_in1, HIGH);
		digitalWrite(pin_in2, LOW);
	}

	analogWrite(pin_ena, (int) (speed * 255.0));
}
void set_right(float speed) {
	if(! is_right_init) {
		return;
	}

	if(speed == 0.0f) {
		digitalWrite(pin_in3, LOW);
		digitalWrite(pin_in4, LOW);
		analogWrite(pin_enb, 0);

		return;
	}
	else if(speed < 0.0f) {
		// set direction of the motor rotation to inverse.
		digitalWrite(pin_in3, LOW);
		digitalWrite(pin_in4, HIGH);

		// make speed absolute.
		speed *= -1.0f;
	}
	else {
		// set direction of the motor rotation to normal.
		digitalWrite(pin_in3, HIGH);
		digitalWrite(pin_in4, LOW);
	}

	analogWrite(pin_enb, (int) (speed * 255.0));
}

float get_left() {
	if(! is_left_init) {
		return 0.0f;
	}

	return (analogRead(pin_ena) / 255.0f - 0.5f) * 2.0f;
}
float get_right() {
	if(! is_right_init) {
		return 0.0f;
	}

	return (analogRead(pin_enb) / 255.0f - 0.5f) * 2.0f;
}

void tank(float left, float right) {
	set_left(left);
	set_right(right);
}

void arcade(float speed, float turn, float range) {
	// clamp speed and turn values to min and max range.
	float x = max(-range, min(turn, range));
	float y = max(-range, min(speed, range));

	// formula gotten from: https://home.kendra.com/mauser/Joystick.html

	// calculate right + left speed.
	const float v = (range - abs(x)) * (y / range) + y;

	// calculate left - right speed.
	const float w = (range - abs(y)) * (x / range) + x;

	// extract both the right and left speed.
	const float left  = (v + w) / 2;
	const float right = (v - w) / 2;

	tank(left, right);

}

} // namespace drive
} // namespace bot

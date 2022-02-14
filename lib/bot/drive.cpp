#include "drive.h"

#include <Arduino.h>
#include "Servo.h"

namespace bot {
namespace drive {

static struct motor {
	int pin_en;
	int pin_in1;
	int pin_in2;

	int last_speed;

	bool used;
} motors[DRIVE_MAX_MOTORS];

static struct servo {
	int pin;

	Servo servo;

	bool used;
} servos[DRIVE_MAX_SERVOS];

static int left_id = -1;
static int right_id = -1;

static bool is_motor_id_valid(int id) {
	return id >= 0 && id < DRIVE_MAX_MOTORS;
}

static bool is_servo_id_valid(int id) {
	return id >= 0 && id < DRIVE_MAX_SERVOS;
}

bool init_motor(int id, int pin_en, int pin_in1, int pin_in2) {
	if(( ! is_motor_id_valid(id)) || motors[id].used) {
		return false;
	}

	// set motor pins.
	motors[id].pin_en = pin_en;
	motors[id].pin_in1 = pin_in1;
	motors[id].pin_in2 = pin_in2;

	// set pin modes for motor pins.
	pinMode(motors[id].pin_en, OUTPUT);
	pinMode(motors[id].pin_in1, OUTPUT);
	pinMode(motors[id].pin_in2, OUTPUT);

	// sanity check that the motor speed is zero.
	analogWrite(pin_en, 0);

	motors[id].used = true;

	return true;
}

bool init_servo(int id, int pin) {
	if(( ! is_servo_id_valid(id)) || servos[id].used) {
		return false;
	}

	// set servo pins.
	servos[id].pin = pin;

	// attach servo pin to the servo controller.
	servos[id].servo.attach(pin);

	// TODO think about not resetting the servos position as it might not be
//          needed and even counter productive.
	// reset servo to zero.
	servos[id].servo.write(0);

	return true;
}


void set_motor(int id, float speed) {
	if(( ! is_motor_id_valid(id)) || ! motors[id].used) {
		return;
	}

	if(speed == 0.0f) {
		digitalWrite(motors[id].pin_in1, LOW);
		digitalWrite(motors[id].pin_in2, LOW);
		analogWrite(motors[id].pin_en, 0);

		return;
	}
	else if(speed < 0.0f) {
		// set direction of the motor rotation to inverse.
		digitalWrite(motors[id].pin_in1, LOW);
		digitalWrite(motors[id].pin_in2, HIGH);

		// make speed absolute.
		speed *= -1.0f;
	}
	else {
		// set direction of the motor rotation to normal.
		digitalWrite(motors[id].pin_in1, HIGH);
		digitalWrite(motors[id].pin_in2, LOW);
	}

	analogWrite(motors[id].pin_en, (int) (speed * 255.0));
}

void set_servo(int id, int angle) {
	if(( ! is_servo_id_valid(id)) || ! servos[id].used) {
		return;
	}

	// clamp angle between 0 and 180.
	angle = angle < 0 ? 0 : angle > 180 ? 180 : angle;

	// write angle to servo.
	servos[id].servo.write(angle);
}

float get_motor(int id) {
	if(( ! is_motor_id_valid(id)) || ! motors[id].used) {
		return 0.0f;
	}

	// TODO think about doing some analog read here, though it probably isn't
	//      needed.

	return motors[id].last_speed;
}

int get_servo(int id) {
	if(( ! is_servo_id_valid(id)) || ! servos[id].used) {
		return -1;
	}

	return servos[id].servo.read();
}

bool set_drive_motors(int lid, int rid) {
	if( ! is_motor_id_valid(lid) || ! is_motor_id_valid(rid)) {
		return false;
	}

	left_id = lid;
	right_id = rid;

	return true;
}

void tank(float left, float right) {
	set_motor(left_id, left);
	set_motor(right_id, right);
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

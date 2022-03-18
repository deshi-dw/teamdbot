#ifndef BOT_H
#define BOT_H

#ifdef DEBUG
#define PRINT(message) Serial.print(message)
#define PRINTLN(message) Serial.println(message)
#else
#define PRINTLN(message)
#define PRINT(message)
#endif

#include "net.h"
#include "drive.h"
#include "input.h"

namespace bot {
namespace pkt {

#define     PKT_OK_ID 0xd9
#define   PKT_INIT_ID 0xd1
#define  PKT_START_ID 0xd2
#define  PKT_INPUT_ID 0xd4

struct ok {
	uint8_t id;
	uint8_t status;
};

struct init {
	uint8_t id;

	uint8_t pin_ena;
	uint8_t pin_in1;
	uint8_t pin_in2;

	uint8_t pin_enb;
	uint8_t pin_in3;
	uint8_t pin_in4;

	// uint8_t pin_right1
	// uint8_t pin_right2;
	// uint8_t pin_right_en;

	// uint8_t pin_left1;
	// uint8_t pin_left2;
	// uint8_t pin_left_en;

	// uint8_t pin_elev1;
	// uint8_t pin_elev2;

	// uint8_t pin_spin1;
	// uint8_t pin_spin2;

};

struct start {
	uint8_t id;

	uint32_t auto_drive_ms;

	float auto_left_speed;
	float auto_right_speed;
};

struct input {
	uint8_t id;

	float drive_y;
	float drive_x;
};

} // namespace bot
} // namespace pkt

#endif

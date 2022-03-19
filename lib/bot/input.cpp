#include "input.hpp"

#include "Arduino.h"

namespace bot {
namespace input {

static struct stick {
	int id;
	int pin_vrx;
	int pin_vry;
	int pin_sw;

	float value_x;
	float value_y;
	bool is_down;
	;
	bool is_prev_down;

	bool used;
} sticks[INPUT_MAX_STICKS];

static bool is_id_valid(int id) {
	if(id < 0 || id >= INPUT_MAX_STICKS) {
		return false;
	}

	return true;
}

bool init_stick(int id, int pin_vrx, int pin_vry, int pin_sw) {
	if(! is_id_valid(id) || sticks[id].used) {
		return false;
	}

	pinMode(pin_sw, INPUT_PULLUP);

	sticks[id].pin_vrx = pin_vrx;
	sticks[id].pin_vry = pin_vry;
	sticks[id].pin_sw = pin_sw;

	sticks[id].used = true;

	return true;
}

void update() {
	for(int i = 0; i < INPUT_MAX_STICKS; i++) {
		if(! sticks[i].used) {
			continue;
		}

		int x = analogRead(sticks[i].pin_vrx);
		int y = analogRead(sticks[i].pin_vry);
		int sw = sticks[i].pin_sw != -1 ? digitalRead(sticks[i].pin_sw) : 0;

		sticks[i].value_x = (((float) x) / 1023.0f - 0.5f) * 2.0f;
		sticks[i].value_y = (((float) y) / 1023.0f - 0.5f) * 2.0f;
		sticks[i].is_prev_down = sticks[i].is_down;
		sticks[i].is_down = ! sw;
	}
}

float get_stick_x(int id) {
	if(! is_id_valid(id) || ! sticks[id].used) {
		return 0.0f;
	}

	return sticks[id].value_x;
}

float get_stick_y(int id) {
	if(! is_id_valid(id) || ! sticks[id].used) {
		return 0.0f;
	}

	return sticks[id].value_y;
}

bool get_stick_down(int id) {
	if(! is_id_valid(id) || ! sticks[id].used) {
		return 0.0f;
	}

	return sticks[id].is_down && (! sticks[id].is_prev_down);
}

bool get_stick_up(int id) {
	if(! is_id_valid(id) || ! sticks[id].used) {
		return 0.0f;
	}

	return (! sticks[id].is_down) && sticks[id].is_prev_down;
}

bool get_stick_held(int id) {
	if(! is_id_valid(id) || ! sticks[id].used) {
		return 0.0f;
	}

	return sticks[id].is_down && sticks[id].is_prev_down;
}

} // namespace input
} // namespace bot

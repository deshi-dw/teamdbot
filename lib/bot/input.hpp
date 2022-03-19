#ifndef BOT_INPUT_H
#define BOT_INPUT_H

namespace bot {
namespace input {

#define INPUT_MAX_STICKS 2

bool init_stick(int id, int pin_vrx, int pin_vry, int pin_sw);
void update();

float get_stick_x(int id);
float get_stick_y(int id);

bool get_stick_down(int id);
bool get_stick_up(int id);
bool get_stick_held(int id);

} // namespace input
} // namespace bot

#endif

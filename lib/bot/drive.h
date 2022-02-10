namespace bot {
namespace drive {

void init(int pin_ena, int pin_in1, int pin_in2,
		  int pin_enb, int pin_in3, int pin_in4);

void init_left(int pin_enb, int pin_in3, int pin_in4);
void init_right(int pin_ena, int pin_in1, int pin_in2);

void set_left(float speed);
void set_right(float speed);

float get_left();
float get_right();

void tank(float left, float right);
void arcade(float speed, float turn, float range = 1.0f);

} // namespace drive
} // namespace bot

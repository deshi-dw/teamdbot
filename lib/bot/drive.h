namespace bot {
namespace drive {

#define DRIVE_MAX_MOTORS 4
#define DRIVE_MAX_SERVOS 2

bool init_motor(int id, int pin_en, int pin_in1, int pin_in2);
bool init_servo(int id, int pin);

void set_motor(int id, float speed);
void set_servo(int id, int angle);

float get_motor(int id);
int get_servo(int id);

bool set_drive_motors(int lid, int rid)

void tank(float left, float right);
void arcade(float speed, float turn, float range = 1.0f);

} // namespace drive
} // namespace bot

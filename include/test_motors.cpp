// #include "driver.h"

// #include <Arduino.h>
// #include "Servo.h"

// // right motor pins:
// const int pin_enb = 9;
// const int pin_in3 = 5;
// const int pin_in4 = 6;

// // left motor pins:
// const int pin_ena = 3;
// const int pin_in1 = 2;
// const int pin_in2 = 4;

// // potentiometer pins:
// const int pin_pot = A5;

// // servo pins:
// const int pin_servo = 10;

// bot::driver driver(pin_ena, pin_in1, pin_in2, pin_enb, pin_in3, pin_in4);
// Servo servo;

// void setup() {
// 	Serial.begin(115200);
// 	Serial.println("starting bot...");

// 	pinMode(pin_pot, INPUT);

// 	driver.init();
// 	servo.attach(pin_servo);

// 	Serial.println("bot started.");
// }

// #define print_sleep_time 2500;
// int print_sleep = print_sleep_time;

// float prev_speed = 0.0f;

// void loop() {
// 	float speed = (analogRead(pin_pot) / 1024.0f - 0.5f) * 2.0f;

// 	if(--print_sleep <= 0) {
// 		Serial.print("pot: ");
// 		Serial.print(analogRead(pin_pot));
// 		Serial.print(", speed: ");
// 		Serial.println(speed);

// 		Serial.print("servo: ");
// 		Serial.println((int) (speed * 90.0f + 90.0f));

// 		Serial.println();
// 		print_sleep = print_sleep_time;

// 	}

//   if(speed != prev_speed) {
//     Serial.println("servo updated.");
//     servo.write((int) (speed * 180.0f));
//   }

// 	driver.set(speed);

// 	prev_speed = speed;
// }
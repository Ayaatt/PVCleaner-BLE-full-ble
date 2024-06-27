#ifndef GPIO_H
#define GPIO_H

// Pin Motor Stepper
#define PUL1_PIN 27
#define DIR1_PIN 29
#define ENA1_PIN 31
#define PUL2_PIN 30
#define DIR2_PIN 28
#define ENA2_PIN 26

// Pin Sensor Limit Switch 1
#define sensor1DepanPin 54
#define sensor1BelakangPin 55
// Pin Sensor Limit Switch 1
#define sensor2DepanPin 56
#define sensor2BelakangPin 57

// Pin Motor sikat 1
#define RPWM1 3
#define LPWM1 5
#define PWM1 7
// Pin Motor sikat 2
#define RPWM2 2
#define LPWM2 4
#define PWM2 6

#define MAJU 2
#define MUNDUR 1
#define BERHENTI 0

// Pin untuk baca level baterai
int pinBatt = 47;
int EXPbatt = 0;

#endif
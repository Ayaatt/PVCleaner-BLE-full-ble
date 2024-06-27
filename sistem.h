#ifndef SISTEM_H
#define SISTEM_H

#include <AccelStepper.h>
#include "managers.h"

// struct masterManual
// {
//     int arah;
//     int motorSpeed;
//     int brushSpeed;
// };

// struct masterAuto
// {
//     int motorSpeedUp;
//     int motorSpeedDown;
//     int brushSpeedUp;
//     int brushSpeedDown;
//     int nRepeat;
// };

// masterAuto getMasterAuto(String payload);
// masterManual getMasterManual(String payload);

void setupSistem();
void jalanAuto(int direction, int motorSpeedUp, int motorSpeedDown, int brushSpeedUp, int brushSpeedDown);
void jalanManual(int direction, int motorSpeed, int brushSpeed);
int batteryStatus();
void displayRefreshBTBATT();
void refreshCurrentPage();
int speedMenuPage();
void mainMenuPage();
// void masterPage(String load);
void modeMenuPage();
void prosesMenuPage();
void prosesAuto();
void prosesManual();
void slavePage();
void dataAuto(String payload);
void dataManual(String payload);
// void menuAwal(String psn);
void menuAuto(String psn);
void menuManual(String psn);

#endif
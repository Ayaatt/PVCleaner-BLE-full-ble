#ifndef MANAGERS_H
#define MANAGERS_H

// define nextion and software serial
#include "EasyNextionLibrary.h"
#include <HardwareSerial.h>

#define DUMP_AT_COMMANDS

// extern myNex and software serial
// extern SoftwareSerial HM10; // RX,TX
extern EasyNex myNex;

// extern all variable
extern bool isManualMode;
extern bool isAutoMode;
extern String pesan;
extern int Kec;
extern int nCus;
extern int counter;
extern int is_Forward;
extern bool forward_Is_Begin;
extern bool sensor1DepanStatus;        // Baca status sensor infrared pertama
extern bool sensor1BelakangStatus;     // Baca status sensor infrared kedua
extern bool sensor2DepanStatus;        // Baca status sensor infrared pertama
extern bool sensor2BelakangStatus;     // Baca status sensor infrared kedua
extern int DataRefreshRate;            // The time between each Data refresh of the page
extern unsigned long pageRefreshTimer; // Timer for DataRefreshRate
extern bool newPageLoaded;             // true when the page is first loaded ( lastCurrentPageId != currentPageId )
extern int lastPage;                   // last current page ID
extern int currentPage;                // current page ID

#endif
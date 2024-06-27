// https://github.com/Seithan/EasyNextionLibrary
#include <Arduino.h>
#include "managers.h"
#include "sistem.h"
#include <Arduino_FreeRTOS.h>

int sensorDStatus = LOW; // digitalRead(sensorDepanPin);  // Baca status sensor depan
int sensorBStatus = LOW; // digitalRead(sensorBelakangPin);  // Baca status sensor belakang
int lastState = 0;
String pesan = "";
EasyNex myNex(Serial);

// define task handles
TaskHandle_t TaskSerialBLE_Handler;
TaskHandle_t TaskControl_Handler;

void setup()
{
    Serial.begin(115200); // setup Serial Monitor to display information
    Serial3.begin(115200);

    xTaskCreate(
        TaskSerialBLE, "BLUETOOTH" // A name just for humans
        ,
        256 // This stack size can be checked & adjusted by reading the Stack Highwater
        ,
        NULL // Parameters passed to the task function
        ,
        2 // Priority, with 2 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,
        &TaskSerialBLE_Handler); // Task handle

    xTaskCreate(
        TaskControl, "CONTROL", 512 // Stack size
        ,
        NULL // Parameters passed to the task function
        ,
        1 // Priority
        ,
        &TaskControl_Handler); // Task handle
    vTaskStartScheduler();
}

void loop()
{
    // myNex.NextionListen();
    // refreshCurrentPage();
    // mainMenuPage();
    // prosesMenuPage();

    //  bluetooth();
}

void TaskSerialBLE(void *parameter)
{
    (void)parameter;
    while (1)
    {
        while (Serial3.available() > 0)
        {
            char incomingChar = Serial3.read();
            //    payload += incomingChar;
            pesan += incomingChar;
        }

        int dashPosition = pesan.indexOf('-');
        // Extract the substring before the '-' character
        String command = pesan.substring(0, dashPosition);

        if (command == "AUTO")
        {
            menuAuto(pesan);
        }
        else if (command == "MANUAL")
        {
            if (!isManualMode)
            {
                menuManual(pesan);
            }
            else
            {
                dataManual(pesan);
            }
        }
        else if (command = "stop")
        {
            jalanAuto(0, 0, 0, 0, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for 100 milliseconds
    }
}

void TaskControl(void *parameter)
{
    (void)parameter;
    myNex.begin();
    setupSistem();
    myNex.writeStr("page 0");

    // check display nextion
    if (sensorDStatus == LOW && sensorBStatus == LOW)
    {
        myNex.writeStr("t1.txt", "OK");
        myNex.writeStr("t3.txt", "OK");
        myNex.writeStr("t5.txt", "OK");
        delay(3000);
        myNex.writeStr("page 1");
    }
    else
    {
        myNex.writeStr("t1.txt", "ERROR");
        myNex.writeNum("t1.pco", 63488);
        myNex.writeStr("t3.txt", "ERROR");
        myNex.writeNum("t3.pco", 63488);
        myNex.writeStr("t5.txt", "ERROR");
        myNex.writeNum("t5.pco", 63488);
        delay(2000);
        myNex.writeStr("page 0");
    }

    delay(2000);

    myNex.lastCurrentPageId = 5; // aturan untuk mengaktifkan page 1 biar tidak tabrakan dengan current page ID

    while (1)
    {
        myNex.NextionListen();
        refreshCurrentPage();
    }
}
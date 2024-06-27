
#include "sistem.h"
#include "managers.h"
#include "GPIO.h"

// SoftwareTX

int Kec = 0;
int nCus = 0;
int counter = 0;
int is_Forward = true;
bool forward_Is_Begin = true;
bool sensor1DepanStatus = LOW;             // Baca status sensor infrared pertama
bool sensor1BelakangStatus = LOW;          // Baca status sensor infrared kedua
bool sensor2DepanStatus = LOW;             // Baca status sensor infrared pertama
bool sensor2BelakangStatus = LOW;          // Baca status sensor infrared kedua
int DataRefreshRate = 50;                  // The time between each Data refresh of the page
unsigned long pageRefreshTimer = millis(); // Timer for DataRefreshRate
bool newPageLoaded = false;                // true when the page is first loaded ( lastCurrentPageId != currentPageId )
int lastPage = -1;                         // last current page ID
int currentPage = 0;                       // current page ID
int motorSpeed = 0;
int brushSpeed = 0;
int arah = 0;
int motorSpeedUp = 0;
int motorSpeedDown = 0;
int brushSpeedUp = 0;
int brushSpeedDown = 0;
int nRepeat = 0;
bool isManualMode = false;
bool isAutoMode = false;
int new_motorSpeed;
int new_brushSpeed;
int new_arah;

const int stepPerRevolution = 3200; // jumlah langkah pada motor stepper

AccelStepper stepper1(AccelStepper::DRIVER, PUL1_PIN, DIR1_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2_PIN, DIR2_PIN);

/**
 * @brief Inisialisasi pin motor dan konfigurasi sistem.
 */
void setupSistem()
{
    pinMode(sensor1DepanPin, INPUT);    // define pin as Input  sensor
    pinMode(sensor1BelakangPin, INPUT); // define pin as Input  sensor
    pinMode(sensor2DepanPin, INPUT);    // define pin as Input  sensor
    pinMode(sensor2BelakangPin, INPUT); // define pin as Input  sensor
    pinMode(ENA1_PIN, OUTPUT);
    digitalWrite(ENA1_PIN, HIGH); // Aktifkan driver stepper
    pinMode(ENA2_PIN, OUTPUT);
    digitalWrite(ENA2_PIN, HIGH); // Aktifkan driver stepper
    pinMode(RPWM1, OUTPUT);       // Deklarasi RPWM pada Driver BTS7960 sebagai Output
    pinMode(LPWM1, OUTPUT);       // Deklarasi LPWM pada Driver BTS7960 sebagai Output
    pinMode(PWM1, OUTPUT);        // Deklarasi PWM pada Driver BTS7960 sebagai Output
    pinMode(RPWM2, OUTPUT);       // Deklarasi RPWM pada Driver BTS7960 sebagai Output
    pinMode(LPWM2, OUTPUT);       // Deklarasi LPWM pada Driver BTS7960 sebagai Output
    pinMode(PWM2, OUTPUT);        // Deklarasi PWM pada Driver BTS7960 sebagai Output

    stepper1.setMaxSpeed(3200);    // Kecepatan maksimum (steps per second)
    stepper1.setAcceleration(500); // Akselerasi (stepes per second)
    stepper2.setMaxSpeed(3200);    // Kecepatan maksimum (steps per second)
    stepper2.setAcceleration(500); // Akselerasi (stepes per second)
}

void jalan(int directionRobot)
{
    if (directionRobot == MAJU)
    {
        digitalWrite(RPWM1, HIGH);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, 200); // Set kecepatan motor pada PWM 200
        digitalWrite(RPWM2, HIGH);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, 200);       // Set kecepatan motor pada PWM 200
        digitalWrite(ENA1_PIN, HIGH); // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH); // Aktifkan driver stepper

        unsigned long timer = 0;
        while (true)
        {
            stepper1.setSpeed(Kec); //(nKec) untuk putaran CW
            stepper1.runSpeed();
            stepper2.setSpeed(Kec); //(nKec) untuk putaran CW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if ((sensor1BelakangStatus == LOW && sensor1DepanStatus == HIGH) || currentPage != 4)
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW
        }
    }

    else if (directionRobot == MUNDUR)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, HIGH);
        analogWrite(PWM1, 200); // Set kecepatan motor pada PWM 200
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, HIGH);
        analogWrite(PWM2, 200);       // Set kecepatan motor pada PWM 200
        digitalWrite(ENA1_PIN, HIGH); // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH); // Aktifkan driver stepper
        unsigned long timer = 0;
        while (true)
        {
            stepper1.setSpeed(-Kec); //(-nKec) untuk putaran CCW
            stepper1.runSpeed();
            stepper2.setSpeed(-Kec); //(-nKec) untuk putaran CCW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if ((sensor1BelakangStatus == HIGH && sensor1DepanStatus == LOW) || currentPage != 4)
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW
        }
    }

    else if (directionRobot == BERHENTI)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, 0); // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, 0);        // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(ENA1_PIN, LOW); // Menonaktifkan driver stepper
        digitalWrite(ENA2_PIN, LOW); // Menonaktifkan driver stepper
        stepper1.setSpeed(0);        // Untuk menghentikan putaran stepper
        stepper1.runSpeed();
        stepper2.setSpeed(0); // Untuk menghentikan putaran stepper
        stepper2.runSpeed();
    }
}

void jalanAuto(int directionRobot, int motorSpeedUp, int motorSpeedDown, int brushSpeedUp, int brushSpeedDown)
{
    if (directionRobot == MAJU)
    {
        digitalWrite(RPWM1, HIGH);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, brushSpeedUp); // Set kecepatan motor pada PWM brushSpeedUp
        digitalWrite(RPWM2, HIGH);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, brushSpeedUp); // Set kecepatan motor pada PWM brushSpeedUp
        digitalWrite(ENA1_PIN, HIGH);    // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH);    // Aktifkan driver stepper

        unsigned long timer = 0;
        while (true)
        {
            stepper1.setSpeed(motorSpeedUp); //(nmotorSpeedUp) untuk putaran CW
            stepper1.runSpeed();
            stepper2.setSpeed(motorSpeedUp); //(nmotorSpeedUp) untuk putaran CW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if ((sensor1BelakangStatus == LOW && sensor1DepanStatus == HIGH) || currentPage != 5)
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW
        }
    }

    else if (directionRobot == MUNDUR)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, HIGH);
        analogWrite(PWM1, brushSpeedDown); // Set kecepatan motor pada PWM brushSpeedDown
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, HIGH);
        analogWrite(PWM2, brushSpeedDown); // Set kecepatan motor pada PWM brushSpeedDown
        digitalWrite(ENA1_PIN, HIGH);      // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH);      // Aktifkan driver stepper
        unsigned long timer = 0;

        while (true)
        {
            stepper1.setSpeed(-motorSpeedDown); //(-nmotorSpeedDown) untuk putaran CCW
            stepper1.runSpeed();
            stepper2.setSpeed(-motorSpeedDown); //(-nmotorSpeedDown) untuk putaran CCW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if ((sensor1BelakangStatus == HIGH && sensor1DepanStatus == LOW) || currentPage != 5)
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW
        }
    }

    else if (directionRobot == BERHENTI)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, 0); // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, 0);        // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(ENA1_PIN, LOW); // Menonaktifkan driver stepper
        digitalWrite(ENA2_PIN, LOW); // Menonaktifkan driver stepper
        stepper1.setSpeed(0);        // Untuk menghentikan putaran stepper
        stepper1.runSpeed();
        stepper2.setSpeed(0); // Untuk menghentikan putaran stepper
        stepper2.runSpeed();
    }
}

void jalanManual(int directionRobot, int motorSpeed, int brushSpeed)
{
    pesan = "";
    if (directionRobot == MAJU)
    {
        digitalWrite(RPWM1, HIGH);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, brushSpeed); // Set kecepatan motor pada PWM brushSpeed
        digitalWrite(RPWM2, HIGH);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, brushSpeed); // Set kecepatan motor pada PWM brushSpeed
        digitalWrite(ENA1_PIN, HIGH);  // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH);  // Aktifkan driver stepper

        unsigned long timer = 0;
        while (true)
        {
            stepper1.setSpeed(motorSpeed); //(nmotorSpeed) untuk putaran CW
            stepper1.runSpeed();
            stepper2.setSpeed(motorSpeed); //(nmotorSpeed) untuk putaran CW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if (sensor1BelakangStatus == HIGH || sensor1DepanStatus == HIGH || currentPage != 6 || new_arah != MAJU)
            {
                jalanManual(BERHENTI, 0, 0);
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW}
            }
            isManualMode = true;
        }
    }
    else if (directionRobot == MUNDUR)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, HIGH);
        analogWrite(PWM1, brushSpeed); // Set kecepatan motor pada PWM brushSpeed
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, HIGH);
        analogWrite(PWM2, brushSpeed); // Set kecepatan motor pada PWM brushSpeed
        digitalWrite(ENA1_PIN, HIGH);  // Aktifkan driver stepper
        digitalWrite(ENA2_PIN, HIGH);  // Aktifkan driver stepper
        unsigned long timer = 0;

        while (true)
        {
            stepper1.setSpeed(-motorSpeed); //(-nmotorSpeed) untuk putaran CCW
            stepper1.runSpeed();
            stepper2.setSpeed(-motorSpeed); //(-nmotorSpeed) untuk putaran CCW
            stepper2.runSpeed();

            sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
            sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
            sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
            sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua
            if (millis() - timer > 1000)
            {
                currentPage = myNex.readNumber("pg.val");
                timer = millis();
            }
            if (sensor1BelakangStatus == HIGH || sensor1DepanStatus == HIGH || currentPage != 6 || new_arah != MUNDUR)
            {
                jalanManual(BERHENTI, 0, 0);
                break; // Jika prinsip sensor kebalik maka tukar HIGH dan LOW}
            }
            isManualMode = true;
        }
    }

    else if (directionRobot == BERHENTI)
    {
        digitalWrite(RPWM1, LOW);
        digitalWrite(LPWM1, LOW);
        analogWrite(PWM1, 0); // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(RPWM2, LOW);
        digitalWrite(LPWM2, LOW);
        analogWrite(PWM2, 0);        // Set kecepatan motor pada PWM 0 untuk menghentikan motor
        digitalWrite(ENA1_PIN, LOW); // Menonaktifkan driver stepper
        digitalWrite(ENA2_PIN, LOW); // Menonaktifkan driver stepper
        stepper1.setSpeed(0);        // Untuk menghentikan putaran stepper
        stepper1.runSpeed();
        stepper2.setSpeed(0); // Untuk menghentikan putaran stepper
        stepper2.runSpeed();
        isManualMode = false;
    }
}

/**
 * @brief Refreshes the display for BTBATT.
 */
void displayRefreshBTBATT()
{
    int bat = batteryStatus();
    // jika battery habis, maka berkedip
    if (bat == 0)
    {
        myNex.writeNum("j0.bco", 65535);
        delay(250);
        myNex.writeNum("j0.bco", 63488);
        delay(250);
    }
    // else if dibawah 10 dan diatas 0 berkedip
    else if (bat < 10 && bat > 0)
    {
        myNex.writeNum("j0.bco", 65535);
        delay(250);
        myNex.writeNum("j0.bco", 10262);
        delay(250);
    }
    else
    {
        myNex.writeNum("j0.bco", 61342);
    }
    myNex.writeNum("j0.val", bat); // update battery status
}

/**
 * @brief Refreshes the current page.
 */
void refreshCurrentPage()
{
    // In this function we refresh the page currently loaded every DataRefreshRate
    if ((millis() - pageRefreshTimer) > DataRefreshRate)
    {
        currentPage = myNex.readNumber("pg.val");
        if (currentPage != lastPage)
        {
            newPageLoaded = true;
            lastPage = currentPage;
        }

        switch (currentPage)
        {
        case 1:
            mainMenuPage();
            jalan(BERHENTI);
            jalanAuto(BERHENTI, 0, 0, 0, 0);
            jalanManual(BERHENTI, 0, 0);
            forward_Is_Begin = true;
            break;
        case 2:
            speedMenuPage();
            forward_Is_Begin = true;
            break;
        case 3:
            modeMenuPage();
            forward_Is_Begin = true;
            break;
        case 4:
            prosesMenuPage();
            break;
        case 5:
            prosesAuto();
            break;
        case 6:
            prosesManual();
            break;
        }
        if (currentPage == 3)
        {
            DataRefreshRate = 120;
        }
        else
        {
            DataRefreshRate = 50;
        }
        displayRefreshBTBATT();
        pageRefreshTimer = millis();
        newPageLoaded = false;
    }
}

void mainMenuPage()
{
    displayRefreshBTBATT();
    pesan = "";
}

void menuAuto(String psn)
{
    isAutoMode = true;
    dataAuto(psn);
    pesan = "";
    myNex.writeStr("page 5");
}

void menuManual(String psn)
{
    isManualMode = true;
    dataManual(pesan);
    pesan = "";
    myNex.writeStr("page 6");
}

void dataAuto(String payload)
{
    // Find the positions of the '-' and '*' characters
    int dashPosition = payload.indexOf('-');
    int firstAsterisk = payload.indexOf('*');
    int secondAsterisk = payload.indexOf('*', firstAsterisk + 1);
    int thirdAsterisk = payload.indexOf('*', secondAsterisk + 1);
    int fourthAsterisk = payload.indexOf('*', thirdAsterisk + 1);

    // Extract the substrings between the '-' and '*' characters
    String motorSpeedUpStr = payload.substring(dashPosition + 1, firstAsterisk);
    String motorSpeedDownStr = payload.substring(firstAsterisk + 1, secondAsterisk);
    String brushSpeedUpStr = payload.substring(secondAsterisk + 1, thirdAsterisk);
    String brushSpeedDownStr = payload.substring(thirdAsterisk + 1, fourthAsterisk);
    String nRepeatStr = payload.substring(fourthAsterisk + 1);

    // Convert the strings to integers
    motorSpeedUp = motorSpeedUpStr.toInt() < 100 ? (int)(32 * motorSpeedUpStr.toInt()) : 3200;
    motorSpeedDown = motorSpeedDownStr.toInt() < 100 ? (int)(32 * motorSpeedDownStr.toInt()) : 3200;
    brushSpeedUp = brushSpeedUpStr.toInt() < 100 ? (int)(2.55 * brushSpeedUpStr.toInt()) : 255;
    brushSpeedDown = brushSpeedDownStr.toInt() < 100 ? (int)(2.55 * brushSpeedDownStr.toInt()) : 255;
    nRepeat = nRepeatStr.toInt();
}

void dataManual(String payload)
{
    // Find the positions of the '-' and '*' characters
    int dashPosition = payload.indexOf('-');
    int firstAsterisk = payload.indexOf('*');
    int secondAsterisk = payload.indexOf('*', firstAsterisk + 1);

    // Extract the substrings between the '-' and '*' characters
    String directionStr = payload.substring(dashPosition + 1, firstAsterisk);
    String motorSpeedStr = payload.substring(firstAsterisk + 1, secondAsterisk);
    String brushSpeedStr = payload.substring(secondAsterisk + 1);

    // Convert the strings to integers
    arah = directionStr.toInt();
    motorSpeed = motorSpeedStr.toInt() < 100 ? (int)(32 * motorSpeedStr.toInt()) : 3200;
    brushSpeed = brushSpeedStr.toInt() < 100 ? (int)(2.55 * brushSpeedStr.toInt()) : 255;

    // if (arah != 0 && motorSpeed != 0 && brushSpeed != 0)
    // {
    new_arah = arah;
    new_motorSpeed = motorSpeed;
    new_brushSpeed = brushSpeed;
    isManualMode = true;

    // }
}

int speedMenuPage()
{
    int nKec = myNex.readStr("t8.txt").toInt();
    Kec = nKec < 100 ? (int)(32 * nKec) : 3200; // Membatasi nilai input Kec dari LCD max 100%, selebihnya dari 100% dianggap 100%
}

void modeMenuPage()
{
    int START = myNex.readNumber("bt7.val");
    nCus = myNex.readStr("t11.txt").toInt();
}

void prosesMenuPage()
{
    if (forward_Is_Begin)
    {                      // Inisialisasi untuk menjalankan jalan(MAJU); pada saat Start ditekan
        is_Forward = true; // true = jalan(MAJU); , false = jalan(MUNDUR)
        forward_Is_Begin = false;
        counter = 0;
    }

    sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
    sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
    sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
    sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua

    // Jika prinsip sensor kebalik maka tukar HIGH dan LOW(Pastikan sama dengan yang ada di system)
    if (is_Forward && sensor1BelakangStatus == LOW && sensor1DepanStatus == HIGH)
    {
        is_Forward = false;
    }
    else if (!is_Forward && sensor1BelakangStatus == HIGH && sensor1DepanStatus == LOW)
    {
        is_Forward = true;
        counter++;
    }

    if (counter >= nCus)
    {
        jalan(BERHENTI);
    }
    else
    {
        if (!is_Forward)
        {
            jalan(MUNDUR);
        }
        else if (is_Forward)
        {
            jalan(MAJU);
        }
    }
}

void prosesAuto()
{
    Serial3.println("Repeat : " + nRepeat);
    if (forward_Is_Begin)
    {                      // Inisialisasi untuk menjalankan jalan(MAJU); pada saat Start ditekan
        is_Forward = true; // true = jalan(MAJU); , false = jalan(MUNDUR)
        forward_Is_Begin = false;
        counter = 0;
    }

    sensor1DepanStatus = digitalRead(sensor1DepanPin);       // Baca status sensor infrared pertama
    sensor1BelakangStatus = digitalRead(sensor1BelakangPin); // Baca status sensor infrared kedua
    sensor2DepanStatus = digitalRead(sensor2DepanPin);       // Baca status sensor infrared pertama
    sensor2BelakangStatus = digitalRead(sensor2BelakangPin); // Baca status sensor infrared kedua

    // Jika prinsip sensor kebalik maka tukar HIGH dan LOW(Pastikan sama dengan yang ada di system)
    if (is_Forward && sensor1BelakangStatus == LOW && sensor1DepanStatus == HIGH)
    {
        is_Forward = false;
    }
    else if (!is_Forward && sensor1BelakangStatus == HIGH && sensor1DepanStatus == LOW)
    {
        is_Forward = true;
        counter++;
    }

    if (counter >= nRepeat)
    {
        jalanAuto(BERHENTI, 0, 0, 0, 0);

        pesan = "";
        myNex.writeStr("page 1");
    }
    else
    {
        if (!is_Forward)
        {
            jalanAuto(MUNDUR, motorSpeedDown, motorSpeedDown, brushSpeedDown, brushSpeedDown);
        }
        else if (is_Forward)
        {
            jalanAuto(MAJU, motorSpeedUp, motorSpeedUp, brushSpeedUp, brushSpeedUp);
        }
    }
}

void prosesManual()
{

    if (new_arah == 0)
    {
        jalanManual(BERHENTI, 0, 0);
        pesan = "";
        isManualMode = false;
        myNex.writeStr("page 1");
    }
    else if (arah == 2)
    {
        jalanManual(MAJU, new_motorSpeed, new_brushSpeed);
        isManualMode = true;
        // pesan = "";
    }
    else if (arah == 1)
    {
        jalanManual(MUNDUR, new_motorSpeed, new_brushSpeed);
        isManualMode = true;
        // pesan = "";
    }
}

int batteryStatus()
{
    int senBatt = 1024;                            // analogRead(pinBatt);
    EXPbatt = 0.5 * senBatt + (1 - 0.5) * EXPbatt; // 0.5 is the weight of the new reading
    int battPercentage = map(senBatt, 1800, 2000, 0, 100);
    if (EXPbatt > 100)
    {
        EXPbatt = 100;
    }
    else if (EXPbatt < 0)
    {
        EXPbatt = 0;
    }
    return EXPbatt;
}

// masterAuto getMasterAuto(String pesan)
// {
//     masterAuto Auto;

//     Auto.motorSpeedUp = 0;
//     Auto.motorSpeedDown = 0;
//     Auto.brushSpeedUp = 0;
//     Auto.brushSpeedDown = 0;
//     Auto.nRepeat = 0;

//     // Find the positions of the '-' and '*' characters
//     int dashPosition = pesan.indexOf('-');
//     int firstAsterisk = pesan.indexOf('*');
//     int secondAsterisk = pesan.indexOf('*', firstAsterisk + 1);
//     int thirdAsterisk = pesan.indexOf('*', secondAsterisk + 1);
//     int fourthAsterisk = pesan.indexOf('*', thirdAsterisk + 1);

//     // Extract the substrings between the '-' and '*' characters
//     String motorSpeedUpStr = pesan.substring(dashPosition + 1, firstAsterisk);
//     String motorSpeedDownStr = pesan.substring(firstAsterisk + 1, secondAsterisk);
//     String brushSpeedUpStr = pesan.substring(secondAsterisk + 1, thirdAsterisk);
//     String brushSpeedDownStr = pesan.substring(thirdAsterisk + 1, fourthAsterisk);
//     String nRepeatStr = pesan.substring(fourthAsterisk + 1);

//     // Convert the strings to integers
//     Auto.motorSpeedUp = motorSpeedUpStr.toInt() < 100 ? (int)(32 * motorSpeedUpStr.toInt()) : 3200;
//     Auto.motorSpeedDown = motorSpeedDownStr.toInt() < 100 ? (int)(32 * motorSpeedDownStr.toInt()) : 3200;
//     Auto.brushSpeedUp = brushSpeedUpStr.toInt() < 100 ? (int)(2.55 * brushSpeedUpStr.toInt()) : 255;
//     Auto.brushSpeedDown = brushSpeedDownStr.toInt() < 100 ? (int)(2.55 * brushSpeedDownStr.toInt()) : 255;
//     Auto.nRepeat = nRepeatStr.toInt();

//     return Auto;
// }

// masterManual getMasterManual(String pesan)
// {
//     masterManual Manual;
//     Manual.arah = 0;
//     Manual.motorSpeed = 0;
//     Manual.brushSpeed = 0;

//     // Find the positions of the '-' and '*' characters
//     int dashPosition = pesan.indexOf('-');
//     int firstAsterisk = pesan.indexOf('*');
//     int secondAsterisk = pesan.indexOf('*', firstAsterisk + 1);

//     // Extract the substrings between the '-' and '*' characters
//     String directionStr = pesan.substring(dashPosition + 1, firstAsterisk);
//     String motorSpeedStr = pesan.substring(firstAsterisk + 1, secondAsterisk);
//     String brushSpeedStr = pesan.substring(secondAsterisk + 1);

//     // Convert the strings to integers
//     Manual.arah = directionStr.toInt();
//     Manual.motorSpeed = motorSpeedStr.toInt() < 100 ? (int)(32 * motorSpeedStr.toInt()) : 3200;
//     Manual.brushSpeed = brushSpeedStr.toInt() < 100 ? (int)(2.55 * brushSpeedStr.toInt()) : 255;

//     return Manual;
// }

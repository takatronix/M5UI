#include <M5UI.h>
#include "M5UnitENV.h"

SHT3X sht3x;
QMP6988 qmp;

void setup() {
    M5.begin();
    if (!qmp.begin(&Wire, QMP6988_SLAVE_ADDRESS_L, 0, 26, 400000U)) {
        Serial.println("Couldn't find QMP6988");
        while (1) delay(1);
    }

    if (!sht3x.begin(&Wire, SHT3X_I2C_ADDR, 0, 26, 400000U)) {
        Serial.println("Couldn't find SHT3X");
        while (1) delay(1);
    }
}

void loop() {
    if (sht3x.update()) {
        Serial.println("-----SHT3X-----");
        Serial.print("Temperature: ");
        Serial.print(sht3x.cTemp);
        Serial.println(" degrees C");
        Serial.print("Humidity: ");
        Serial.print(sht3x.humidity);
        Serial.println("% rH");
        Serial.println("-------------\r\n");
    }

    if (qmp.update()) {
        Serial.println("-----QMP6988-----");
        Serial.print(F("Temperature: "));
        Serial.print(qmp.cTemp);
        Serial.println(" *C");
        Serial.print(F("Pressure: "));
        Serial.print(qmp.pressure);
        Serial.println(" Pa");
        Serial.print(F("Approx altitude: "));
        Serial.print(qmp.altitude);
        Serial.println(" m");
        Serial.println("-------------\r\n");
    }
    delay(1000);
}

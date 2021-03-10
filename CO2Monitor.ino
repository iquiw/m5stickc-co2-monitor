#include <M5StickC.h>
#include "MHZ19.h"

#define RX_PIN 26  // Rx pin which the MHZ19 Tx pin is attached to
#define TX_PIN 0   // Tx pin which the MHZ19 Rx pin is attached to

MHZ19 myMHZ19;     // Constructor for library
unsigned long delaySec = 10000;

void setup() {
    // Initialize the M5StickC object
    M5.begin();
    M5.Lcd.setRotation(1);
    setCpuFrequencyMhz(80);
    M5.Axp.ScreenBreath(8);

    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    myMHZ19.begin(Serial1);
    myMHZ19.autoCalibration();
}

void loop() {
    if (Serial1) {
        render_co2();
    }
    render_battery();
    delay(delaySec);
}

void render_co2() {
    int co2 = myMHZ19.getCO2();
    M5.Lcd.setTextDatum(TR_DATUM);
    M5.Lcd.drawString("  " + (String) co2, 150, 20, 7);
    if (Serial) {
        Serial.print("CO2: ");
        Serial.println(co2);
    }
}

void render_battery() {
    M5.Lcd.setTextDatum(TL_DATUM);
    double vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
    uint8_t warning = M5.Axp.GetWarningLeve();
    uint16_t color = WHITE;
    if (warning) {
        color = RED;
    }
    M5.Lcd.fillRect(140, 0, 18, 9, BLACK);
    if (vbat >= 4) {
        M5.Lcd.fillRect(140, 0, 18, 9, color);
    } else if (vbat >= 3.7) {
        M5.Lcd.fillRect(140, 0, 12, 9, color);
    } else {
        M5.Lcd.fillRect(140, 0, 6, 9, color);
    }
    M5.Lcd.drawRect(140, 0, 18, 9, color);
    M5.Lcd.setCursor(105, 1);
    M5.Lcd.printf("%.3f", vbat);

    if (Serial) {
        Serial.print("vbat: ");
        Serial.print(vbat);
        Serial.print(", w: ");
        Serial.println(warning);
    }
}

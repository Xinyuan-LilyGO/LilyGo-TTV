

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "utilities.h"


U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);


void setup(void)
{
    Serial.begin(115200);

    /*Turn on power control*/
    pinMode(PWR_ON, OUTPUT);
    digitalWrite(PWR_ON, HIGH);

    /*Power on the display*/
    pinMode(OLED_PWR, OUTPUT);
    digitalWrite(OLED_PWR, HIGH);

    /*Set touch Pin as input*/
    pinMode(TOUCH_PIN, INPUT);

    u8g2.begin();

    u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function

}

void loop(void)
{
    u8g2.setFont(u8g2_font_unifont_t_chinese2);  // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);
    u8g2.print("Hello World!");
    u8g2.setCursor(0, 40);
    u8g2.print("你好世界");       // Chinese "Hello World"
    u8g2.sendBuffer();

    delay(1000);
}


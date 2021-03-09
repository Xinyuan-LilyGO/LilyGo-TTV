#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "font.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint32_t targetTime = 0;       // for next 1 second timeout

byte omm = 99;

static uint8_t conv2d(const char *p)
{
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

void setup(void)
{
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    targetTime = millis() + 1000;
    tft.setFreeFont(&LiquidCrystal56pt7b);
}

void loop()
{
    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        ss++;              // Advance second
        if (ss == 60) {
            ss = 0;
            omm = mm;
            mm++;            // Advance minute
            if (mm > 59) {
                mm = 0;
                hh++;          // Advance hour
                if (hh > 23) {
                    hh = 0;
                }
            }
        }

        // Update digital time
        byte xpos = 6;
        byte ypos = 0;

        if (omm != mm) {
            tft.drawString("00", xpos, ypos);
            tft.setTextColor(TFT_GREEN, TFT_BLACK); // Orange
            omm = mm;

            if (hh < 10) {
                xpos += tft.drawChar('0', xpos, ypos);
            }

            xpos += tft.drawNumber(hh, xpos, ypos);

            tft.drawLine(6, 119, 64, 119, TFT_GREEN);
            tft.drawLine(70, 119, 128, 119, TFT_GREEN);

            tft.drawLine(6, 115, 64, 115, TFT_GREEN);
            tft.drawLine(70, 115, 128, 115, TFT_GREEN);

            xpos = 6;
            ypos = 130;
            if (mm < 10)
                xpos += tft.drawChar('0', xpos, ypos);
            tft.drawNumber(mm, xpos, ypos);
        }

        if (ss % 2) {
            tft.drawLine(6, 119, 64, 119, TFT_BLACK);
            tft.drawLine(70, 119, 128, 119, TFT_BLACK);

            tft.drawLine(6, 115, 64, 115, TFT_BLACK);
            tft.drawLine(70, 115, 128, 115, TFT_BLACK);
        } else {
            tft.drawLine(6, 119, 64, 119, TFT_GREEN);
            tft.drawLine(70, 119, 128, 119, TFT_GREEN);

            tft.drawLine(6, 115, 64, 115, TFT_GREEN);
            tft.drawLine(70, 115, 128, 115, TFT_GREEN);
        }
    }
}




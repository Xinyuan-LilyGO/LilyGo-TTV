

//#define U8G2_USE_DYNAMIC_ALLOC

#include <stdarg.h>
#include <Arduino.h>
#include <SPI.h>
#include "oled.h"
#include "res/font.h"

#include <Wire.h>


#include <Wire.h>
#include <U8g2lib.h>
#include "utilities.h"


extern U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2;



// SSD1306 OLED

//---------------------------------------------------------------------------
PROGMEM const u8 OledBootProgram[] = {
    0xD5,   // Set Display Clock Divisor 0xF0
    0xF0,
    0x8D,   // Charge Pump Setting 0x14
    0x14,
    0xA1,   // Set Segment Re-map (A0) | (b0001)
    0xC8,   // Set COM Output Scan Direction
    0x81,   // Set Contrast 0xCF
    0xCF,
    0xD9,   // Set Precharge 0xF1
    0xF1,
    0xAF,   // Display On
    0x20,   // Set display mode
    0x00,   // Horizontal addressing mode
};

//---------------------------------------------------------------------------
ST_OLED Oled;


uint8_t *buf;
//---------------------------------------------------------------------------
void OledInit(void)
{

    /*Power on the display*/
    pinMode(OLED_PWR, OUTPUT);
    digitalWrite(OLED_PWR, HIGH);

    /*Set touch Pin as input*/
    pinMode(TOUCH_PIN, INPUT);

    u8g2.begin();



    Serial.print("OledInit");
    u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function

    // u8g2.setFont(u8g2_font_unifont_t_chinese2);  // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFont(u8g2_font_crox1h_tr   );//

    u8g2.setFontDirection(0);
    u8g2.clearBuffer();





}
//---------------------------------------------------------------------------
void OledDisplay(void)
{
    // u8g2.display();
    u8g2.sendBuffer();
    /*  u16 i;

        for(i=0; i<sizeof(Oled.buf); i++)
        {
            SPI.transfer(Oled.buf[i]);
        }*/
}
//---------------------------------------------------------------------------
void OledDrawStr(u8 fx, u8 fy, const char *fmt, ...)
{
    u8g2.setCursor(fx, fy);
    u8g2.print(fmt);
    //u8g2.drawStr(fx, fy, fmt);
    u8g2.sendBuffer();

    /*char s[40];

    va_list ap;
    va_start(ap, fmt);
    _SprintfDo(s, fmt, ap);
    va_end(ap);


    u8 i;

    for(i=0; s[i] != '\0'; i++)
    {
        OledDrawChr(fx++, fy, s[i]);
    }*/
}
//---------------------------------------------------------------------------
void OledDrawChr(u8 fx, u8 fy, char chr)
{
    u8g2.setCursor(fx, fy);
    u8g2.print(chr);
    /*
    if(fx >= OLED_SCREEN_FONT_CX || fy >= OLED_SCREEN_FONT_CY)
    {
       return;
    }

    u8 x;

    for(x=0; x<OLED_FONT_SIZE; x++)
    {
       Oled.buf[(fx * OLED_FONT_CX + x) + (fy * OLED_SCREEN_CX)] = __LPM(font + (chr * OLED_FONT_SIZE) + x);
    }*/
}
//---------------------------------------------------------------------------
void OledDrawDot(u8 x, u8 y)
{
    u8g2.drawPixel(x, y);
    /*Oled.buf[x + (y / 8) * OLED_SCREEN_CX] |= _BV(y % 8);*/
}
//---------------------------------------------------------------------------
void OledDrawCls(void)
{
    u8g2.clearBuffer();

    //u8g2.clearDisplay();// //这个老是闪烁
//  u16 i;
//  for(i=0; i<sizeof(Oled.buf); i++) Oled.buf[i] = 0x00;
    /*
        asm volatile(
            "movw  r30, %0                \n\t"
            "eor __tmp_reg__, __tmp_reg__ \n\t"
            "loop:                        \n\t"
            "st Z+, __zero_reg__          \n\t"
            "st Z+, __zero_reg__          \n\t"
            "st Z+, __zero_reg__          \n\t"
            "st Z+, __zero_reg__          \n\t"
            "inc __tmp_reg__              \n\t"
            "brne loop                    \n\t"

            : : "r" (Oled.buf) : "r30","r31"
        );*/
}
//---------------------------------------------------------------------------
void OledDrawBmp(s8 sx, s8 sy, u8 *p)
{
    //u8g2.setDrawColor(0);

    //u8g2.setDisplayRotation(U8G2_MIRROR);//镜像

    // u8g2.drawXBMP(sx, sy, 16, 12, p);

    u8g2.drawBitmap(sx, sy, 2, 12, p);
    //u8g2.setDisplayRotation(U8G2_R0);//默认，不转



    /* u8 *d  = (u8 *)p;
    u8  cx = __LPM(d++);
    u8  cy = __LPM(d++);

    u8 chr, mask;
    u8 x, y, b;

    for (y = 0; y < cy; y++) {
      for (x = 0; x < cx; x += 8) {
          if (sy + y < 0 || sy + y >= OLED_SCREEN_CY) {
              break;
          }

          chr  = __LPM(d++);
          mask = 0x80;

          for (b = 0; b < 8; b++) {
              if (sx + x + b >= 0 && sx + x + b < OLED_SCREEN_CX) {
                  if (chr & mask) {
                      Oled.buf[(sx + x + b) + ((sy + y) / 8) * OLED_SCREEN_CX] |= _BV((sy + y) & 0x7);
                  }
              }

              mask >>= 1;
          }
      }
    }*/
}


#include "game.h"
#include "oled.h"

#include "rnd.h"
#include <U8g2lib.h>
#include <IRremote.h>

#include "utilities.h"
#include <Arduino.h>

IRrecv irrecv(IR_RECV);
decode_results results;
extern U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2;

bool IR_right, IR_left, IR_down, IR_play, IR_up;


//---------------------------------------------------------------------------
void setup(void)
{
    Serial.begin(115200);
    Serial.print("setup");
    OledInit();
    RndInit();
    GameInit();
    irrecv.enableIRIn(); // Start the receiver
    Serial.print("setup doen");
}
//---------------------------------------------------------------------------
void loop(void)
{
    if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        if ( results.value == 0xFF906F )IR_right = 1 ;
        if ( results.value == 0xFFE01F ) IR_left = 1;
        if ( results.value == 0xFF02FD ) IR_up = 1 ;
        if ( results.value == 0xFF9867 ) IR_down = 1 ;
        if ( results.value == 0xFFA857 ) IR_play = 1;
        if ( results.value == 0xFF22DD ) ;
        irrecv.resume(); // Receive the next value
    }
    GameLoop();
}



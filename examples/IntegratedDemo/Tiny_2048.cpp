
#include "game_tiny.h"
#include "oled.h"

#include "rnd.h"
#include <U8g2lib.h>
#include <IRremote.h>

#include "utilities.h"
#include <Arduino.h>

extern U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2;

bool IR_right, IR_left, IR_down, IR_play, IR_up;

extern IRrecv irrecv;
extern decode_results results;

//---------------------------------------------------------------------------
void Tiny_2048_setup(void)
{
    /*    Serial.begin(115200);
        Serial.print("setup");*/
    OledInit();
    RndInit();
    GameInit();
    /*irrecv.enableIRIn(); // Start the receiver
    Serial.print("setup doen");*/
}
//---------------------------------------------------------------------------
void Tiny_2048_main(void)
{
    while (true) {
        if (irrecv.decode(&results)) {
            Serial.println(results.value, HEX);
            if ( results.value == 0xFF906F )IR_right = 1 ;
            if ( results.value == 0xFFE01F ) IR_left = 1;
            if ( results.value == 0xFF02FD ) IR_up = 1 ;
            if ( results.value == 0xFF9867 ) IR_down = 1 ;
            if ( results.value == 0xFFA857 ) IR_play = 1;
            if ( results.value == 0xFFE21D ) {
                delay(300);
                return;
            }
            irrecv.resume(); // Receive the next value
        }
        GameLoop();
    }


}


//---------------------------------------------------------------------------
void Tiny_2048_loop(void)
{

    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setFontDirection(0);
    u8g2.setFontRefHeightAll();
    for (;;) {
        uint8_t  val = u8g2.getMenuEvent();
        //st_Step(y, /* is_auto_fire */ 0, /* is_fire */ val == U8X8_MSG_GPIO_MENU_SELECT);
        u8g2.firstPage();
        do {
            Serial.println("Tiny_2048_loop");
            Tiny_2048_main();
        } while ( u8g2.nextPage() );

        /*
                if ( val == U8X8_MSG_GPIO_MENU_NEXT ) moveRight += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_PREV  )  moveLeft += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_DOWN )  downButton += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_SELECT) resetGame();
        if (val == U8X8_MSG_GPIO_MENU_HOME)*/
        return;
    }
}
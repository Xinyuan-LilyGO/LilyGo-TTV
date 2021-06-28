
#include <stdlib.h>
#include "Arduino.h"
#include "globals.h"
#include "EEPROM.h"

#include "save.h"
#include "title.h"
#include "mainmenu.h"
#include "game_Glove.h"
#include <IRremote.h>



#define EEPROM_SIZE 250
extern IRrecv irrecv;
extern decode_results results;
extern U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2;
/*
void setup()
{

}*/

void Glove_main()
{
    quitGame = false;
    /* quitGame = false;

     // Show title screen
     displayTitle();
     if (quitGame == true) return;
     updateInput();

     // If the game is freshly installed, jump right in
     if (!validSave) {

         saveCreate();*/

    displayGame();
    if (quitGame == true) return;
    /*
    }
    // Otherwise show the main menu
    else
    {
    if (displayMainMenu() == GAME_REBOOT) {
        saveSetup();
    }
    }*/
}


void Glove_loop(void)
{

    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.setFontDirection(0);
    u8g2.setFontRefHeightAll();
    for (;;) {
        uint8_t  val = u8g2.getMenuEvent();
        //st_Step(y, /* is_auto_fire */ 0, /* is_fire */ val == U8X8_MSG_GPIO_MENU_SELECT);
        u8g2.firstPage();
        do {
            Glove_main();
        } while ( u8g2.nextPage() );

        /*
                if ( val == U8X8_MSG_GPIO_MENU_NEXT ) moveRight += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_PREV  )  moveLeft += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_DOWN )  downButton += 1;
                 if ( val == U8X8_MSG_GPIO_MENU_SELECT) resetGame();
        if (val == U8X8_MSG_GPIO_MENU_HOME)*/
        return;
    }

};

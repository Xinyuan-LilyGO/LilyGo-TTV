
#include <stdlib.h>
#include "Arduino.h"
#include "globals.h"
#include "EEPROM.h"

#include "save.h"
#include "title.h"
#include "mainmenu.h"
#include "game.h"
#include <IRremote.h>

#define IR_RECV             13
IRrecv irrecv(IR_RECV);

decode_results results;

#define EEPROM_SIZE 250


void setup()
{
    // Start arduboy
    arduboy.begin();
    Serial.begin(115200);

    /*Turn on power control*/
    pinMode(PWR_ON, OUTPUT);
    digitalWrite(PWR_ON, HIGH);

    /*Power on the display*/
    pinMode(OLED_PWR, OUTPUT);
    digitalWrite(OLED_PWR, HIGH);

    /*Set touch Pin as input*/
    pinMode(TOUCH_PIN, INPUT);

    /*Touch chip power control*/
    pinMode(TOUCH_PWR, PULLUP);
    digitalWrite(TOUCH_PWR, HIGH);
    EEPROM.begin(1000);

    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("failed to initialise EEPROM"); delay(1000000);
    }

    irrecv.enableIRIn(); // Start the receiver

    // Set audio state
    if (!arduboy.audio.enabled()) {
        arduboy.audio.off();
    }

    // Set framerate
    arduboy.setFrameRate(50);

    // Perform boot save operations
    saveSetup();
}

void loop()
{

    // Show title screen
    displayTitle();

    updateInput();

    // If the game is freshly installed, jump right in
    if (!validSave) {

        saveCreate();
        displayGame();
    }
    // Otherwise show the main menu
    else {
        if (displayMainMenu() == GAME_REBOOT) {
            saveSetup();
        }
    }
}

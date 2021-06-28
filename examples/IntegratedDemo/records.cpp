#include "globals.h"
#include "prompt.h"
#include "save.h"

#define TONE_BACK 400

void displayRecords()
{
    int16_t address = GameSaveOffset;

    arduboy.clear();
    arduboy.setCursor(6 * 5, 0);
    arduboy.print1("- Records -\n\nName Score Time  #Rms");

    uint8_t hi;
    uint8_t lo;
    int16_t val;
    // Loop through scores
    int16_t scoreAddress;

    for (int8_t i = 0; i < 3; ++i) {
        scoreAddress = 20 + (i * 10) + address;
        arduboy.setCursor(0, 8 * (3 + i));
        arduboy.print1(spsn(scoreAddress + FILE_NAME, 4));


        hi = EEPROM.read(scoreAddress + FILE_SCORE);
        lo = EEPROM.read(scoreAddress + FILE_SCORE + 1);
        val = (hi << 8) | lo;
        arduboy.setCursor(6 * 5, 8 * (3 + i));

        char valstr[25];
        itoa(val, valstr, 10);
        arduboy.print1(valstr);
        if (val > 0) arduboy.print1("00");

        hi = EEPROM.read(scoreAddress + FILE_TIME);
        lo = EEPROM.read(scoreAddress + FILE_TIME + 1);
        val = (hi << 8) | lo;
        arduboy.setCursor(6 * 11, 8 * (3 + i));
        //arduboy.print(val);
        printTime(val);

        hi = EEPROM.read(scoreAddress + FILE_LEVEL);
        char histr[25];
        itoa(hi, histr, 10);
        arduboy.print1(" ");
        arduboy.print1(histr);
    }
    arduboy.setCursor(0, 8 * 7);

    if (GameMode == GAME_MODE_GLOVE) {
        arduboy.print1("ROOMS CLEARED: ");
        char getRoomstr[25];
        itoa(getRoomClearPercentage(), getRoomstr, 10);
        arduboy.print1(getRoomstr);
        arduboy.print1("%");
    } else {
        arduboy.print1("   YOU ARE GREAT!!");
    }

    arduboy.display();

    tautInput();
    while (true) {
        delay(10);
        updateInput();
        if (B_PRESSED || A_PRESSED) {
            arduboy.tunes.tone(TONE_BACK, 20);
            break;
        }
    }
}

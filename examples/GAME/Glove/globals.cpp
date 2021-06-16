#include "globals.h"
//#define IR_RECV             13
#include <IRremote.h>

Arduboy2 arduboy;

bool old_a = false;
bool old_b = false;
bool old_up = false;
bool old_down = false;
bool old_left = false;
bool old_right = false;

bool new_a = false;
bool new_b = false;
bool new_up = false;
bool new_down = false;
bool new_left = false;
bool new_right = false;

char progmemString[] = "";

uint8_t GameMode = GAME_MODE_GLOVE;
int16_t GameSaveOffset = GAME_GLOVE_OFFSET;

extern IRrecv irrecv;
extern decode_results results;

String strnow, strp;
// IRrecv irrecv(IR_RECV);
// decode_results results;
void tautInput()
{
    new_a = true;
    new_b = true;
    new_up = true;
    new_down = true;
    new_left = true;
    new_right = true;
}
int y = 0;
void updateInput()
{

    old_a = new_a;
    old_b = new_b;
    old_up = new_up;
    old_down = new_down;
    old_left = new_left;
    old_right = new_right;

    new_a = 0;
    new_b = 0;

    if (y == 0) {
        new_right = 0;
        new_left = 0;
        new_down = 0;
        new_up = 0;
    }

    Serial.println("irrecv");
    if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);

        if (results.value == 0xFFFFFFFF) {
            strnow = strp; //把当前值变为上一个接收到的
            y = 1;
        } else { //不是重复的则更新
            strnow = results.value;
            strp = results.value;
            y = 0;
        }
        /*    if ( results.value == 0xFF906F ) strnow = 0xFF906F; strp = 0xFF906F;// new_right = 1;//右
            if ( results.value == 0xFFE01F )  strnow = 0xFFE01F; strp = 0xFFE01F;// new_left = 1;//左
            if ( results.value == 0xFF9867 ) strnow = 0xFF9867; strp = 0xFF9867; // new_down = 1;//下降
            if ( results.value == 0xFF02FD ) strnow = 0xFF02FD; strp = 0xFF02FD; // new_up = 1;//上
            if ( results.value == 0xFFA857 ) strnow = 0xFFA857; strp = 0xFFA857;  //new_a = 1;//中间播放
            if (results.value == 0xFFFFFFFF) strnow = strp; //把当前值变为上一个接收到的*/
        Serial.println(strnow);

        if (strnow == "16748655" ) {

            new_left = 1;    //右
            strp = "16748655";
        }
        if (strnow == "16769055" )  {
            new_right = 1;  //左
            strp = "16769055";
        }
        if (strnow == "16750695" )  {
            new_down = 1;    //下降
            strp = "16750695";
        }
        if (strnow == "16712445" )  {
            new_up = 1;    //上
            strp = "16712445";
        }
        if (strnow == "16754775" )  {
            new_a = 1;    //中间播放
            strp = "16754775";
        }

        //if ( results.value == 0xFF22DD ) resetGame();
        irrecv.resume(); // Receive the next value

        /*
        new_a = arduboy.pressed(B_BUTTON);
        new_b = arduboy.pressed(A_BUTTON);
        new_up = arduboy.pressed(UP_BUTTON);
        new_down = arduboy.pressed(DOWN_BUTTON);
        new_left = arduboy.pressed(LEFT_BUTTON);
        new_right = arduboy.pressed(RIGHT_BUTTON);
        delay(1);*/
    }
}

char *rps(const char *const *str)
{
    strcpy_P(progmemString, (char *)pgm_read_dword(str));
    return progmemString;
}

char *dps(const char *str)
{
    strcpy_P(progmemString, (char *)pgm_read_dword(str));
    return progmemString;
}

char *eps(char *str)
{
    strcpy_P(progmemString, str);
    return progmemString;
}

char *spsn(int16_t addr, uint8_t len)
{
    int16_t offset;

    progmemString[0] = '\0';

    for (offset = 0; offset < len; offset++) {
        progmemString[offset] = EEPROM.read(addr + offset);
    }
    progmemString[offset] = '\0';

    return progmemString;
}


void printTime(uint16_t time)
{
    int16_t mins = time / 60;
    int16_t  secs = time % 60;


    char minsstr[25];
    char secsstr[25];
    itoa(mins, minsstr, 10);
    itoa(secs, secsstr, 10);

    if (mins < 10) arduboy.print1("0");
    arduboy.print1(minsstr);
    arduboy.print1(":");
    if (secs < 10) arduboy.print1("0");
    arduboy.print1(secsstr);

}

int16_t EEPROMReadint16_t(int16_t addr)
{
    uint8_t hi = EEPROM.read(addr + FILE_SCORE);
    uint8_t lo = EEPROM.read(addr + FILE_SCORE + 1);
    return (hi << 8) | lo;
}

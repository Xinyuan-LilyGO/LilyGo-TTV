#include "score.h"
#include "rnd.h"
#include "oled.h"

#include "stdlib.h"
#include "Arduino.h"

//---------------------------------------------------------------------------
ST_SCORE Score;


//---------------------------------------------------------------------------
void ScoreInit(void)
{
    _Memset(&Score, 0x00, sizeof(ST_SCORE));

    ScoreInitRnd();

}
//---------------------------------------------------------------------------
void ScoreInitRnd(void)
{
    RndInitSeed(0);

}
//---------------------------------------------------------------------------
void ScoreDraw(void)
{
    char Score_now[25];
    char Score_best[25];
    itoa(Score.now, Score_now, 10);

    OledDrawStr(100, 20, Score_now);
}
//---------------------------------------------------------------------------
void ScoreAddNow(u8 num)
{
    if (num >= 10) {
        Score.is10 = TRUE;
    }

    Score.now += num;
}
//---------------------------------------------------------------------------
bool ScoreIs10(void)
{
    return Score.is10;
}


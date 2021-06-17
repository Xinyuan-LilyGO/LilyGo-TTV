


#include "game.h"
#include "oled.h"

#include "panel.h"
#include "score.h"
#include "anime.h"
#include "sprite.h"
#include "common.h"

#include "stdlib.h"
#include <Arduino.h>
//---------------------------------------------------------------------------
ST_GAME Game;

extern bool IR_right, IR_left, IR_down, IR_play, IR_up;

u8 clearBuffer_flag;
//---------------------------------------------------------------------------
void GameInit(void)
{
    _Memset(&Game, 0x00, sizeof(ST_GAME));
}
//---------------------------------------------------------------------------
void GameLoop(void)
{
    //开始游戏，在初始化
    switch (Game.act) {

    case GAME_EXEC_RESET: GameExecReset(); break;
    case GAME_EXEC_ANIME: GameExecAnime(); break;
    case GAME_EXEC_PLAY:  GameExecPlay();  break;
    case GAME_EXEC_JUDGE: GameExecJudge(); break;
    case GAME_EXEC_OVER:  GameExecOver();  break;

    default:
        _SystemError("[Game act=%x]", Game.act);


        break;
    }

//  OledDrawStr(0, 4, "%03d %x", FrameGetCpuPercentMax(), FrameGetCnt());
}

//---------------------------------------------------------------------------
void GameExecReset(void)
{

    AnimeInit();
    PanelInit();
    ScoreInit();
    SpriteInit();

    PanelMakeCell();
    //PanelMakeCell();
    PanelDebug();

    GameSetAct(GAME_EXEC_ANIME);
}
//---------------------------------------------------------------------------

void GameExecAnime(void)
{

    PanelDraw();
    ScoreDraw();
    SpriteDraw();

    /*
        AnimeExec();//动画执行

        if (clearBuffer_flag == 0) {

            OledDrawCls();
        }
        clearBuffer_flag--;
        if (AnimeIsEnd() == FALSE) {//如果还没结束，return

            return;
        }
     */

    do {

        SpriteDraw();
        AnimeExec();//动画执行
    } while (AnimeIsEnd() == FALSE);




    //清屏
    if (clearBuffer_flag == 1) {
        clearBuffer_flag = 0;
        OledDrawCls();
    }

    if (ScoreIs10() == TRUE || PanelIsGameOver() == TRUE) {
        GameSetAct(GAME_EXEC_OVER);
    } else {
        GameSetAct(GAME_EXEC_PLAY);
    }
}
//---------------------------------------------------------------------------
void GameExecPlay(void)
{

    PanelDraw();
    ScoreDraw();
    SpriteDraw();

    if (IR_up) {
        clearBuffer_flag = 1;
        IR_up = 0;
        PanelMoveUp();
        GameSetAct(GAME_EXEC_JUDGE);
    } else if (IR_right) {
        clearBuffer_flag = 1;
        IR_right = 0;
        PanelMoveRight();
        GameSetAct(GAME_EXEC_JUDGE);
    } else if (IR_down) {
        clearBuffer_flag = 1;
        IR_down = 0;
        PanelMoveDown();
        GameSetAct(GAME_EXEC_JUDGE);
    } else if (IR_left) {
        clearBuffer_flag = 1;
        IR_left = 0;
        PanelMoveLeft();
        GameSetAct(GAME_EXEC_JUDGE);
    }
    if (IR_play) {
        clearBuffer_flag = 1;
        IR_play = 0;

        GameSetAct(GAME_EXEC_RESET);
    }
}
//---------------------------------------------------------------------------
void GameExecJudge(void)
{
    PanelDraw();
    ScoreDraw();
    SpriteDraw();


    if (AnimeIsDispOnly() == FALSE) {
        PanelMakeCell();
    }

    GameSetAct(GAME_EXEC_ANIME);
}
//---------------------------------------------------------------------------
void GameExecOver(void)
{
    PanelDraw();
    ScoreDraw();
    SpriteDraw();


    OledDrawStr(0, 50, "GAME");

    if (ScoreIs10() == TRUE) {
        OledDrawStr(0, 60, "CLEAR");
    } else {
        OledDrawStr(0, 60, "OVER");
    }

    if (IR_play) {

        GameSetAct(GAME_EXEC_RESET);
    }
}
//---------------------------------------------------------------------------
void GameSetAct(u8 act)
{
    Game.act = act;
}

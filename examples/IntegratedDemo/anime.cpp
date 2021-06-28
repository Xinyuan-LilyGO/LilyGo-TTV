#include "anime.h"
#include "sprite.h"

#include "oled.h"
#include "utilities.h"
#include "stdlib.h"
//---------------------------------------------------------------------------
ST_ANIME Anime;

extern bool IR_right, IR_left, IR_down, IR_play, IR_up;
//---------------------------------------------------------------------------
void AnimeInit(void)
{
    _Memset(&Anime, 0x00, sizeof(ST_ANIME));
}
//---------------------------------------------------------------------------
void AnimeReset(void)
{
    SpriteReset();

    Anime.regCnt = 0;
}
//---------------------------------------------------------------------------
void AnimeExec(void)
{
    u8 i ;

    for (i = 0; i < Anime.regCnt; i++) {
        if (Anime.d[i].isUse == FALSE) {
            continue;
        }

        switch (Anime.d[i].type) {
        case ANIME_TYPE_MAKE:
            AnimeExecMake(i);
            break;

        case ANIME_TYPE_MOVE:
            AnimeExecMove(i);
            break;

        case ANIME_TYPE_DISP:
            AnimeExecDisp(i);
            break;

        case ANIME_TYPE_ADD:
            AnimeExecAdd(i);
            break;
        }
    }
}
//---------------------------------------------------------------------------
void AnimeExecMake(u8 num)
{
    ST_ANIME_DATA *p = &Anime.d[num];

    if (p->wait < 2) {
        p->wait++;
        return;
    }
    p->wait = 0;

    if (++p->var >= 6) {
        p->isUse = FALSE;
    }

    SpriteSetPanelScale(num, p->fsx, p->fsy, p->chr, p->var);
}
//---------------------------------------------------------------------------
void AnimeExecMove(u8 num)
{
    ST_ANIME_DATA *p = &Anime.d[num];

    p->fsx += p->fmx;
    p->fsy += p->fmy;

    if ((p->dir == KEY_U && p->fsy <= p->fdy) || (p->dir == KEY_D && p->fsy >= p->fdy) ||
            (p->dir == KEY_L && p->fsx <= p->fdx) || (p->dir == KEY_R && p->fsx >= p->fdx)) {
        p->fsx = p->fdx;
        p->fsy = p->fdy;

        p->isUse = FALSE;
    }

    SpriteSetPanelNormal(num, p->fsx, p->fsy, p->chr);

}
//---------------------------------------------------------------------------
void AnimeExecDisp(u8 num)
{
    ST_ANIME_DATA *p = &Anime.d[num];

    SpriteSetPanelNormal(num, p->fsx, p->fsy, p->chr);

    p->isUse = FALSE;
}
//---------------------------------------------------------------------------
void AnimeExecAdd(u8 num)
{

    ST_ANIME_DATA *p = &Anime.d[num];

    if (p->var < 9) {
        p->var++;
        return;
    }

    if (p->var < 10) {
        SpriteDelPanel(p->fsx, p->fsy);
        SpriteSetPanelReverse(num, p->fsx, p->fsy, p->chr);
        p->var++;
        return;
    }

    if (p->var >= 10 && p->var < 20) {
        p->var++;
        return;
    }

    if (p->var == 20) {
        SpriteSetPanelNormal(num, p->fsx, p->fsy, p->chr);
        p->var++;
        return;
    }

    p->isUse = FALSE;
}
//---------------------------------------------------------------------------
void AnimeSetMake(s8 sx, s8 sy, u8 num)
{

    ST_ANIME_DATA *p = &Anime.d[Anime.regCnt];

    p->isUse = TRUE;
    p->type  = ANIME_TYPE_MAKE;
    p->fsx   = NUM2FIX(32 + (12 + 4) * sx);
    p->fsy   = NUM2FIX( 2 + (12 + 4) * sy);
    p->x     = sx;
    p->y     = sy;
    p->chr   = num - 1;
    p->var   = 0;
    p->wait  = 0;

    SpriteSetPanelScale(Anime.regCnt, p->fsx, p->fsy, p->chr, p->var);

    Anime.regCnt++;
}
//---------------------------------------------------------------------------
void AnimeSetMove(s8 sx, s8 sy, s8 dx, s8 dy, u8 num, u8 dir)
{
    ST_ANIME_DATA *p = &Anime.d[Anime.regCnt];

    p->isUse = TRUE;
    p->type  = ANIME_TYPE_MOVE;
    p->fsx   = NUM2FIX(32 + (12 + 4) * sx);
    p->fsy   = NUM2FIX( 2 + (12 + 4) * sy);
    p->fdx   = NUM2FIX(32 + (12 + 4) * dx);
    p->fdy   = NUM2FIX( 2 + (12 + 4) * dy);
    p->fmx   = NUM2FIX(5) * (dx - sx);
    p->fmy   = NUM2FIX(5) * (dy - sy);
    p->x     = dx;
    p->y     = dy;
    p->chr   = num - 1;
    p->dir   = dir;

    SpriteSetPanelNormal(Anime.regCnt, p->fsx, p->fsy, p->chr);

    Anime.regCnt++;
}
//---------------------------------------------------------------------------
void AnimeSetDisp(s8 sx, s8 sy, u8 num)
{

    ST_ANIME_DATA *p = &Anime.d[Anime.regCnt];

    p->isUse = TRUE;
    p->type  = ANIME_TYPE_DISP;
    p->fsx   = NUM2FIX(32 + (12 + 4) * sx);
    p->fsy   = NUM2FIX( 2 + (12 + 4) * sy);
    p->x     = sx;
    p->y     = sy;
    p->chr   = num - 1;

    SpriteSetPanelNormal(Anime.regCnt, p->fsx, p->fsy, p->chr);

    Anime.regCnt++;
}
//---------------------------------------------------------------------------
void AnimeSetAdd(s8 sx, s8 sy, u8 num)
{
    ST_ANIME_DATA *p = &Anime.d[Anime.regCnt];

    p->isUse = TRUE;
    p->type  = ANIME_TYPE_ADD;
    p->fsx   = NUM2FIX(32 + (12 + 4) * sx);
    p->fsy   = NUM2FIX( 2 + (12 + 4) * sy);
    p->x     = sx;
    p->y     = sy;
    p->chr   = num - 1;
    p->var   = 0;

    Anime.regCnt++;
}
//---------------------------------------------------------------------------
bool AnimeIsAdd(s8 x, s8 y)
{
    u8 i;

    for (i = 0; i < Anime.regCnt; i++) {
        if (Anime.d[i].type == ANIME_TYPE_ADD && Anime.d[i].x == x && Anime.d[i].y == y) {
            return TRUE;
        }
    }

    return FALSE;
}
//---------------------------------------------------------------------------
bool AnimeIsEnd(void)
{
    u8 i;


    for (i = 0; i < Anime.regCnt; i++) {
        if (Anime.d[i].isUse == TRUE) {
            return FALSE;
        }
    }

    return TRUE;
}
//---------------------------------------------------------------------------
bool AnimeIsDispOnly(void)
{
    u8 i;

    for (i = 0; i < Anime.regCnt; i++) {
        if (Anime.d[i].type != ANIME_TYPE_DISP) {
            return FALSE;
        }
    }

    return TRUE;
}

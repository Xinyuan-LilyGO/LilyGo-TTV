
#include "sprite.h"
#include "oled.h"
#include "res.h"
#include "stdlib.h"
//---------------------------------------------------------------------------
ST_SPRITE Sprite[SPRITE_MAX_CNT];


//---------------------------------------------------------------------------
void SpriteInit(void)
{
    _Memset(&Sprite, 0x00, sizeof(ST_SPRITE) * SPRITE_MAX_CNT);
}
//---------------------------------------------------------------------------
void SpriteReset(void)
{
    u8 i;

    for (i = 0; i < SPRITE_MAX_CNT; i++) {
        if (Sprite[i].isUse == TRUE) {
            Sprite[i].isUse = FALSE;
        }
    }
}
//---------------------------------------------------------------------------

void SpriteDraw(void)
{
    u8 i;

    /*    OledDrawBmp(30, 40, bitmap);

    */
    /* char  Sprite_sx[25];
     itoa( Sprite[10].sx, Sprite_sx, 10);
     OledDrawStr(0, 30, Sprite_sx);*/

    //OledDrawBmp(30, 50, Sprite[2].pDat); // Sprite[i].pDat 复位

    for (i = 0; i < SPRITE_MAX_CNT; i++) {
        if (Sprite[i].isUse == TRUE) {
            OledDrawBmp(Sprite[i].sx, Sprite[i].sy, Sprite[i].pDat);
        }
    }
}
//---------------------------------------------------------------------------
void SpriteSetPanel(u8 num, s16 fsx, s16 fsy, u8 *pDat)
{
    Sprite[num].sx    = FIX2NUM(fsx);
    Sprite[num].sy    = FIX2NUM(fsy);
    Sprite[num].pDat  = pDat;
    Sprite[num].isUse = TRUE;
}
//---------------------------------------------------------------------------

void SpriteSetPanelNormal(u8 num, s16 fsx, s16 fsy, u8 chr)
{

    SpriteSetPanel(num, fsx, fsy, (u8 *)ResPanelNormalList[chr]);
}
//---------------------------------------------------------------------------
void SpriteSetPanelReverse(u8 num, s16 fsx, s16 fsy, u8 chr)
{
    SpriteSetPanel(num, fsx, fsy, (u8 *)ResPanelReverseList[chr]);
}
//---------------------------------------------------------------------------
void SpriteSetPanelScale(u8 num, s16 fsx, s16 fsy, u8 chr, u8 var)
{
    ASSERT(chr < 2);

    if (chr == 0) {
        SpriteSetPanel(num, fsx, fsy, (u8 *)ResPanelScale1List[var]);
    } else {
        SpriteSetPanel(num, fsx, fsy, (u8 *)ResPanelScale2List[var]);
    }
}
//---------------------------------------------------------------------------
void SpriteDelPanel(s16 fsx, s16 fsy)
{
    u8 sx = FIX2NUM(fsx);
    u8 sy = FIX2NUM(fsy);
    u8 i;

    for (i = 0; i < SPRITE_MAX_CNT; i++) {
        if (Sprite[i].isUse == TRUE && Sprite[i].sx == sx && Sprite[i].sy == sy) {
            Sprite[i].isUse = FALSE;
        }
    }
}


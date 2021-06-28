#ifndef SPRITE_H
#define SPRITE_H
/*#ifdef __cplusplus
extern "C" {
#endif
*/

#include "common.h"
#include "anime.h"

//---------------------------------------------------------------------------
#define SPRITE_MAX_CNT              ANIME_MAX_DATA_CNT


//---------------------------------------------------------------------------

typedef struct {
    bool isUse;
    u8   sx;
    u8   sy;
    u8  *pDat;

} ST_SPRITE;


//---------------------------------------------------------------------------
void SpriteInit(void);
void SpriteReset(void);
void SpriteDraw(void);

void SpriteSetPanel(u8 num, s16 fsx, s16 fsy, u8 *pDat);
void SpriteSetPanelNormal(u8 num, s16 fsx, s16 fsy, u8 chr);
void SpriteSetPanelReverse(u8 num, s16 fsx, s16 fsy, u8 chr);
void SpriteSetPanelScale(u8 num, s16 fsx, s16 fsy, u8 chr, u8 var);
void SpriteDelPanel(s16 fsx, s16 fsy);


/*#ifdef __cplusplus
}
#endif*/
#endif

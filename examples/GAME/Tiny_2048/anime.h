#ifndef ANIME_H
#define ANIME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------
#define ANIME_MAX_DATA_CNT              24


enum {
    ANIME_TYPE_MAKE,
    ANIME_TYPE_MOVE,
    ANIME_TYPE_DISP,
    ANIME_TYPE_ADD,
};

//---------------------------------------------------------------------------

typedef struct {
    bool isUse;
    u8   type;
    u8   chr;
    u8   var;
    u8   wait;

    s8   x;                                 // �X�v���C�g�̏d�Ȃ�\�����O�p
    s8   y;
    s16  fsx;                               // �p�l�������ʒu
    s16  fsy;
    s16  fdx;                               // �p�l���̈ړ���
    s16  fdy;
    s16  fmx;                               // �p�l���̈ړ����x
    s16  fmy;
    u8   dir;                               // �ړ������i�L�[���͒l�j

} ST_ANIME_DATA;


typedef struct {
    u8 regCnt;
    ST_ANIME_DATA d[ANIME_MAX_DATA_CNT];

} ST_ANIME;


//---------------------------------------------------------------------------
void AnimeInit(void);
void AnimeReset(void);

void AnimeExec(void);
void AnimeExecMake(u8 num);
void AnimeExecMove(u8 num);
void AnimeExecDisp(u8 num);
void AnimeExecAdd(u8 num);

void AnimeSetMake(s8 sx, s8 sy, u8 num);
void AnimeSetMove(s8 sx, s8 sy, s8 dx, s8 dy, u8 num, u8 dir);
void AnimeSetDisp(s8 sx, s8 sy, u8 num);
void AnimeSetAdd(s8 sx, s8 sy, u8 num);

bool AnimeIsAdd(s8 x, s8 y);
bool AnimeIsEnd(void);
bool AnimeIsDispOnly(void);


#ifdef __cplusplus
}
#endif
#endif

#ifndef SCORE_H
#define SCORE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

typedef struct {
    u16  now;
    u16  best;
    bool is10;

} ST_SCORE;


//---------------------------------------------------------------------------
void ScoreInit(void);
void ScoreInitRnd(void);
void ScoreDraw(void);


void ScoreAddNow(u8 num);
bool ScoreIs10(void);



#ifdef __cplusplus
}
#endif
#endif

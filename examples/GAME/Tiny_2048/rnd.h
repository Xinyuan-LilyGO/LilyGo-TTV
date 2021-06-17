#ifndef RND_H
#define RND_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void RndInit(void);
void RndInitSeed(u32 num);

u32  Rnd32(void);
u32  Rnd(u32 num);


#ifdef __cplusplus
}
#endif
#endif

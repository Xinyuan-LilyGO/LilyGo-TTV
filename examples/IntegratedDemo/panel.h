#ifndef PANEL_H
#define PANEL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------
#define PANEL_MAX_CX            4
#define PANEL_MAX_CY            4
#define PANEL_MAX_REG_CNT       (PANEL_MAX_CX * PANEL_MAX_CY)

//---------------------------------------------------------------------------

typedef struct {
    u8 regCnt;
    u8 var[PANEL_MAX_CX][PANEL_MAX_CY];

} ST_PANEL;


//---------------------------------------------------------------------------
void PanelInit(void);
void PanelDraw(void);

void PanelMakeCell(void);
void PanelMoveCell(s8 sx, s8 sy, s8 x, s8 y, u8 dir);
void PanelMoveCell2(s8 sx, s8 sy, s8 dx, s8 dy, u8 dir);

void PanelMoveUp(void);
void PanelMoveRight(void);
void PanelMoveDown(void);
void PanelMoveLeft(void);

bool PanelIsGameOver(void);
void PanelDebug(void);

#ifdef __cplusplus
}
#endif
#endif

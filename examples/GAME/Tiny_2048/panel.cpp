#include "panel.h"
#include "oled.h"
#include "rnd.h"
#include "score.h"
#include "anime.h"
#include "sprite.h"
#include "utilities.h"
//---------------------------------------------------------------------------
ST_PANEL Panel;

extern bool IR_right, IR_left, IR_down, IR_play, IR_up, clearBuffer_flag;
//---------------------------------------------------------------------------
void PanelInit(void)
{
    _Memset(&Panel, 0x00, sizeof(ST_PANEL));

}
//---------------------------------------------------------------------------
void PanelDraw(void)
{
    OledDrawStr( 0, 10, "TINY");
    OledDrawStr( 0, 20, "2048");
    OledDrawStr(98, 10, "Score");

}
//---------------------------------------------------------------------------
void PanelMakeCell(void)
{

    u8 r = Rnd(PANEL_MAX_REG_CNT - Panel.regCnt);
    u8 x, y;

    for (y = 0; y < PANEL_MAX_CY; y++) {
        for (x = 0; x < PANEL_MAX_CX; x++) {
            if (Panel.var[x][y] == 0) {
                if (r == 0) {
                    goto End;
                } else {
                    r--;
                }
            }
        }
    }

End:
    //  ASSERT(x <= PANEL_MAX_CX && y <= PANEL_MAX_CY);
    // ASSERT(Panel.var[x][y] == 0);      //如果不注释，这里会卡住，

    Panel.var[x][y] = (Rnd(10) >= 9) ? 2 : 1;
    Panel.regCnt++;

    AnimeSetMake(x, y, Panel.var[x][y]);



}
//---------------------------------------------------------------------------
void PanelMoveCell(s8 sx, s8 sy, s8 x, s8 y, u8 dir)
{
    s8 dx = sx;
    s8 dy = sy;

    for (;;) {
        // �ړ���
        dx += x;
        dy += y;

        // ��
        if (dx < 0 || dy < 0 || dx >= PANEL_MAX_CX || dy >= PANEL_MAX_CY) {
            PanelMoveCell2(sx, sy, dx - x, dy - y, dir);
            return;
        }

        // ��
        if (Panel.var[dx][dy] == 0) {
            continue;
        }

        // �Z���܂��́A�����Z��
        if (Panel.var[sx][sy] != Panel.var[dx][dy] || AnimeIsAdd(dx, dy) == TRUE) {
            PanelMoveCell2(sx, sy, dx - x, dy - y, dir);
            return;
        } else {
            AnimeSetMove(sx, sy, dx, dy, Panel.var[dx][dy], dir);

            Panel.var[sx][sy] = 0;
            Panel.var[dx][dy]++;

            AnimeSetAdd(dx, dy, Panel.var[dx][dy]);
            ScoreAddNow(Panel.var[dx][dy]);

            Panel.regCnt--;
            return;
        }
    }
}
//---------------------------------------------------------------------------
void PanelMoveCell2(s8 sx, s8 sy, s8 dx, s8 dy, u8 dir)
{
    // �ړ����ƈړ��悪�������`�F�b�N�����܂�
    if (sx == dx && sy == dy) {
        AnimeSetDisp(sx, sy, Panel.var[sx][sy]);
    } else {
        Panel.var[dx][dy] = Panel.var[sx][sy];
        Panel.var[sx][sy] = 0;

        AnimeSetMove(sx, sy, dx, dy, Panel.var[dx][dy], dir);
    }
}
//---------------------------------------------------------------------------
void PanelMoveLeft(void)
{
    AnimeReset();

    s8 x, y;

    for (y = PANEL_MAX_CY - 1; y >= 0; y--) {
        for (x = 0; x < PANEL_MAX_CX; x++) {
            if (Panel.var[x][y] != 0) {
                PanelMoveCell(x, y, -1, 0, KEY_L);
            }
        }
    }

}
//---------------------------------------------------------------------------
void PanelMoveRight(void)
{
    AnimeReset();

    s8 x, y;

    for (y = 0; y < PANEL_MAX_CY; y++) {
        for (x = PANEL_MAX_CX - 1; x >= 0; x--) {
            if (Panel.var[x][y] != 0) {
                PanelMoveCell(x, y, 1, 0, KEY_R);
            }
        }
    }
}
//---------------------------------------------------------------------------
void PanelMoveUp(void)
{
    AnimeReset();

    s8 x, y;

    for (x = 0; x < PANEL_MAX_CX; x++) {
        for (y = 0; y < PANEL_MAX_CY; y++) {
            if (Panel.var[x][y] != 0) {
                PanelMoveCell(x, y, 0, -1, KEY_U);

            }
        }
    }

}
//---------------------------------------------------------------------------
void PanelMoveDown(void)
{
    AnimeReset();

    s8 x, y;

    for (x = PANEL_MAX_CX - 1; x >= 0; x--) {
        for (y = PANEL_MAX_CY - 1; y >= 0; y--) {
            if (Panel.var[x][y] != 0) {
                PanelMoveCell(x, y, 0, 1, KEY_D);
            }
        }
    }
}
//---------------------------------------------------------------------------
bool PanelIsGameOver(void)
{
    if (Panel.regCnt < PANEL_MAX_REG_CNT) {
        return FALSE;
    }

    s8 tx[4] = { 0,  1,  0, -1};
    s8 ty[4] = {-1,  0,  1,  0};
    s8 dx, dy;
    s8 x, y;
    u8 i;

    for (y = 0; y < PANEL_MAX_CY; y++) {
        for (x = 0; x < PANEL_MAX_CX; x++) {
            for (i = 0; i < 4; i++) {
                dx = x + tx[i];
                dy = y + ty[i];

                if (dx < 0 || dx >= PANEL_MAX_CX) {
                    continue;
                }

                if (dy < 0 || dy >= PANEL_MAX_CY) {
                    continue;
                }

                if (Panel.var[x][y] == Panel.var[dx][dy]) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}
//---------------------------------------------------------------------------
void PanelDebug(void)
{
// �����A�j���[�V������1, 2�����Ȃ����߁A3�ȏ��I�����Ȃ�����

    /*
        Panel.var[0][0] = 1;
        Panel.var[0][1] = 1;
        Panel.var[0][2] = 1;
        Panel.var[0][3] = 1;

        AnimeSetMake(0, 0, 1);
        AnimeSetMake(0, 1, 1);
        AnimeSetMake(0, 2, 1);
        AnimeSetMake(0, 3, 1);

        Panel.regCnt = 4;
    */

    Panel.var[0][0] = 1;
    Panel.var[1][0] = 1;
    Panel.var[2][0] = 2;

    AnimeSetMake(0, 0, 1);
    AnimeSetMake(1, 0, 1);
    AnimeSetMake(2, 0, 2);

    Panel.regCnt = 3;
}

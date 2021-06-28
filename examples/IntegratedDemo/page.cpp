#include <Arduino.h>
#include <U8g2lib.h>
#include "main.h"

extern U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2;

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32+ ICON_GAP


struct menu_entry_type {
    const uint8_t *font;
    uint16_t icon;
    const char *name;
};

struct menu_state {
    int16_t menu_start;       /* in pixel */
    int16_t frame_position;       /* in pixel */
    uint8_t position;         /* position, array index */
};

struct menu_entry_type menu_entry_list[] = {
    { u8g2_font_open_iconic_embedded_4x_t, 65, "Clock Setup"},
    { u8g2_font_open_iconic_embedded_4x_t, 66, "SpaceTrash"},
    { u8g2_font_open_iconic_embedded_4x_t, 67, "LittleRookChess"},
    { u8g2_font_open_iconic_embedded_4x_t, 69, "New Blocks"},
    { u8g2_font_open_iconic_embedded_4x_t, 70, "Glove"},
    { u8g2_font_open_iconic_embedded_4x_t, 71, "Tiny_2048"},
    { u8g2_font_open_iconic_embedded_4x_t, 68, "Video"},
    { u8g2_font_open_iconic_embedded_4x_t, 78, "Sleep"},
    { NULL, 0, NULL }
};


int8_t button_event = 0;        // set this to 0, once the event has been processed

void check_button_event(void)
{
    if ( button_event == 0 )
        button_event = u8g2.getMenuEvent();
}


void draw(struct menu_state *state)
{
    int16_t x;
    uint8_t i;
    x = state->menu_start;
    i = 0;
    while ( menu_entry_list[i].icon > 0 ) {
        if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() ) {
            u8g2.setFont(menu_entry_list[i].font);
            u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
        }
        i++;
        x += ICON_WIDTH + ICON_GAP;
        check_button_event();
    }
    u8g2.drawFrame(state->frame_position - 1, ICON_Y - ICON_HEIGHT - 1, ICON_WIDTH + 2, ICON_WIDTH + 2);
    u8g2.drawFrame(state->frame_position - 2, ICON_Y - ICON_HEIGHT - 2, ICON_WIDTH + 4, ICON_WIDTH + 4);
    u8g2.drawFrame(state->frame_position - 3, ICON_Y - ICON_HEIGHT - 3, ICON_WIDTH + 6, ICON_WIDTH + 6);
    check_button_event();
}


void to_right(struct menu_state *state)
{
    if ( menu_entry_list[state->position + 1].font != NULL ) {
        if ( (int16_t)state->frame_position + 2 * (int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() ) {
            state->position++;
            state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
        } else {
            state->position++;
            state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}

void to_left(struct menu_state *state)
{
    if ( state->position > 0 ) {
        if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP + (int16_t)ICON_WIDTH + (int16_t)ICON_GAP ) {
            state->position--;
            state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
        } else {
            state->position--;
            state->frame_position = ICON_BGAP;
            state->menu_start = state->frame_position - state->position * ((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}


uint8_t towards_int16(int16_t *current, int16_t dest)
{
    if ( *current < dest ) {
        (*current)++;
        return 1;
    } else if ( *current > dest ) {
        (*current)--;
        return 1;
    }
    return 0;
}

uint8_t towards(struct menu_state *current, struct menu_state *destination)
{
    uint8_t r = 0;
    uint8_t i;
    for ( i = 0; i < 6; i++ ) {
        r |= towards_int16( &(current->frame_position), destination->frame_position);
        r |= towards_int16( &(current->menu_start), destination->menu_start);
    }
    return r;
}



struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };



uint8_t menuloop(void)
{
    uint8_t val = 0;
    for (;;) {
        do {

            u8g2.firstPage();
            do {

                draw(&current_state);
                u8g2.setFont(u8g2_font_helvB10_tr);
                u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(menu_entry_list[destination_state.position].name)) / 2, u8g2.getDisplayHeight() - 5);
                u8g2.print(menu_entry_list[destination_state.position].name);
                val = u8g2.getMenuEvent();//这个最终是调用 u8x8_GetMenuEvent，在IntegrateDemo.ino里
                delay(10);
            } while ( u8g2.nextPage() );
            if ( val == U8X8_MSG_GPIO_MENU_NEXT )//遥控向右
                to_right(&destination_state);
            if ( val == U8X8_MSG_GPIO_MENU_PREV )//遥控向左
                to_left(&destination_state);
            if ( val == U8X8_MSG_GPIO_MENU_SELECT ) {//进入▶
                return destination_state.position;  //按了▶ ,(return)结束当前循环,回到loop函数判断
            }
            if ( val > 0 ) // 处理所有已知事件，清除事件 all known events are processed, clear event
                val = 0;
        } while ( towards(&current_state, &destination_state) );
    }
    return 0;
}

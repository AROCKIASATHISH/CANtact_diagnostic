#ifndef _TUI_H
#define _TUI_H

void TUI_init(void);
void TUI_Up(void);
void TUI_Down(void);
void TUI_move_forward(void);
void TUI_move_backward(void);
void TUI_color_set(void);
void TUI_color_notset(void);
void TUI_menu_process(uint8_t choice,char *menu_sub1[] , uint8_t status);
void TUI_init_position(uint8_t cnt);
void TUI_set_position(char pos,int line,char *menu_sub[],int coun);
void TUI_clear(uint8_t line);

#endif

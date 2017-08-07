//filename:LCD_Core.h

#ifndef _LCD_CORE_H
#define _LCD_CORE_H

#include "DataType.h"




extern void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
extern void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void show_setboardaddr(u32 data);
void show_setdirection(u8 data);
void judge_screen1(void);
void judge_screen2(void);
void flashwrite(u32 data);
void setshow(void);
void init_sc(void);
void text(void);
void close_exitPB11(void);
void datarefresh(u8 input);
void sc_lock(void);
void sc_print(void);
void sc_nomal(void);
void sc_numble(u16 numble);
void sc_weight(u16 weight);
void judge_initsc(void);
void open_exitPB11(void);
void data_allrefresh(void);
void adjust1_judge(void);
void adjust2_judge(void);
void adjust3_judge(void);
void adjust4_judge(void);
void sc_adjust1(void);
void sc_adjust2(void);
void sc_adjust3(void);
void sc_adjust4(void);

#endif
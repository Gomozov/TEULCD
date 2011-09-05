/*
  ULCD.h - Library for work with TE-ULCD display.
  Created by Valeriy Gomozov, August 27, 2011.
  Released into the public domain.
*/

#ifndef ULCD_h
#define ULCD_h

#include "WProgram.h"

class ULCD
{
  public:
    ULCD();
    void RESET();
    void digitalPortWrite(byte value);
    void LOAD_FONT(byte R, String FileName);
    void LOAD_PICTURE(byte R, String FileName); 
    void LOAD_TEXT(byte R, String Text);
    void LOAD_SCRIPT(byte R, byte Number);
    void SET_SIZE(int X, byte Y);
    void SET_POS(int X, byte Y);
    void SET_BACK_COLOR(byte R, byte G, byte B); 
    void SET_FONT_COLOR(byte R, byte G, byte B);
    void SET_FONT(byte R);
    void SET_MAIN(byte R);
    void SET_SCALE(byte min, byte max, byte pos);
    void SET_SCRIPT(byte R);
    void SET_TEXT(byte R);
    void SET_PICTURE(byte R);
    void SEND_REG(byte R1, byte R2);
    void WRITE_REG(byte R, byte value);
    void CR_WID(byte WidType);
    byte READ(byte R); 
    void REPAINT_TEXT(byte R1, byte R2, String Text);
    void REPAINT_BMP(byte R1, byte R2);
};

#endif
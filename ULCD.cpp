#include "ULCD.h"
#include "WProgram.h"
#include "SPI.h";

ULCD::ULCD()
{
  pinMode (9, OUTPUT);
  digitalWrite(9,HIGH);
}

void ULCD::digitalPortWrite(byte value) {
  digitalWrite(9,LOW);
  delay(5);
  SPI.transfer(value);
  delay(5);
  digitalWrite(9,HIGH); 
  delay(5);
}

void ULCD::SEND_REG(byte R1, byte R2) {
  digitalPortWrite(3); 
  digitalPortWrite(R1);
  digitalPortWrite(R2); 
}

void ULCD::WRITE_REG(byte R, byte value) {
  digitalPortWrite(2); 
  digitalPortWrite(R);
  digitalPortWrite(value); 
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(0);
}

void ULCD::CR_WID(byte WidType) {
  digitalPortWrite(32);
  digitalPortWrite(WidType); 
  delay(5);
}

void ULCD::SET_SIZE(int X, byte Y) {
  digitalPortWrite(2); 
  digitalPortWrite(72);
  if (X > 255) {
    digitalPortWrite(X-256);
    digitalPortWrite(1);
  } 
  else {
    digitalPortWrite(X);
    digitalPortWrite(0);
  }  
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(2); 
  digitalPortWrite(73);
  digitalPortWrite(Y);
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(40);
}

void ULCD::SET_POS(int X, byte Y) {
  digitalPortWrite(2);                
  digitalPortWrite(72);
  if (X > 255) {
    digitalPortWrite(X-256);
    digitalPortWrite(1);
  } 
  else {
    digitalPortWrite(X);
    digitalPortWrite(0);
  }  
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(2);                
  digitalPortWrite(73);
  digitalPortWrite(Y);
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(0);
  digitalPortWrite(38);               
}

void ULCD::SET_BACK_COLOR(byte R, byte G, byte B) {
  digitalPortWrite(2);                                         //Устанавливаем цвет фона
  digitalPortWrite(72);
  digitalPortWrite(B); 
  digitalPortWrite(G);
  digitalPortWrite(R);
  digitalPortWrite(0);
  digitalPortWrite(44);
}

void ULCD::SET_FONT_COLOR(byte R, byte G, byte B) {
  digitalPortWrite(2);                                        //Устанавливаем цвет текста
  digitalPortWrite(72);
  digitalPortWrite(B); 
  digitalPortWrite(G);
  digitalPortWrite(R);
  digitalPortWrite(0);
  digitalPortWrite(47);
}

void ULCD::LOAD_FONT(byte R, String FileName) {                  
  digitalPortWrite(8);                                     //Создаем контейнер   
  digitalPortWrite(14);                                       
  digitalPortWrite(0);                                     // From: 0 - Flash, 1 - SD
  for (int i = 0; i < FileName.length(); i++)
    digitalPortWrite(FileName[i]);            
  for (int i = 0; i < (12 - FileName.length()); i++)
    digitalPortWrite(0); 
  SEND_REG(72, 65);                             //Пересылаем шрифт в регистр OPERAND0
  digitalPortWrite(24);                                    //Создание объекта типа шрифт
  SEND_REG(R, 65);                                    //Сохраняем шрифт в регистр R
}

void ULCD::LOAD_PICTURE(byte R, String FileName) {                  
  digitalPortWrite(8);                                     //Создаем контейнер  
  digitalPortWrite(14);                                       
  digitalPortWrite(1);                                     // From: 0 - Flash, 1 - SD
  for (int i = 0; i < FileName.length(); i++)
    digitalPortWrite(FileName[i]);            
  for (int i = 0; i < (12 - FileName.length()); i++)
    digitalPortWrite(0);   
  SEND_REG(R, 65);    
}

void ULCD::SET_FONT(byte R) {
  SEND_REG(72, R);                                       //Загружаем шрифт в регистр OPERAND0
  digitalPortWrite(50);
}

void ULCD::REPAINT_TEXT(byte R1, byte R2, String Text) {
  SPI.setDataMode(SPI_MODE3);
  SEND_REG(65, R2);
  WRITE_REG(72, Text.length());
  digitalPortWrite(10);                                      //Устанавливаем длину контейнера
  WRITE_REG(66, 0);
  for (int i = 0; i < Text.length(); i++) {
    WRITE_REG(72, Text[i]);        
    digitalPortWrite(17);                                    //Помещаем байт в буффер
  }
  SEND_REG(R2, 65);     // Возможно это лишнее
  SEND_REG(65, R1);
  SEND_REG(72, R2);
  digitalPortWrite(52);
  digitalPortWrite(35);
  SPI.setDataMode(SPI_MODE0);
}

void ULCD::REPAINT_BMP(byte R1, byte R2) {
  SPI.setDataMode(SPI_MODE3);
  SEND_REG(65, R1);                  //Загружаем BitMap в регистр CURRENT
  SEND_REG(72, R2);                  //Загружаем в регистр OPERAND0 картинку из регистра R для BitMap
  digitalPortWrite(52);              //Загружаем картинку в BitMap
  digitalPortWrite(35);              //Перерисовываем BitMap
  SPI.setDataMode(SPI_MODE0);
}

void ULCD::SET_MAIN(byte R) {
  SEND_REG(65, R);                                       //Загружаем регистр R3 содержащий Frame в регистр CURRENT (R0 -> CURRENT)
  digitalPortWrite(34);                                       //Установить виджет главным
}

byte ULCD::READ(byte R) {
  digitalPortWrite(1);
  digitalPortWrite(R);                                    //answer |= (result << (8 * i));
  digitalWrite(9,LOW);
  delay(5);
  byte res = SPI.transfer(0);
  delay(5);
  digitalWrite(9,HIGH); 
  delay(5);
  digitalWrite(9,LOW);
  delay(5);
  SPI.transfer(0);
  delay(5);
  digitalWrite(9,HIGH); 
  delay(5);
  digitalWrite(9,LOW);
  delay(5);
  SPI.transfer(0);
  delay(5);
  digitalWrite(9,HIGH); 
  delay(5);
  digitalWrite(9,LOW);
  delay(5);
  SPI.transfer(0);
  delay(5);
  digitalWrite(9,HIGH); 
  delay(5);
  return res;  
}


void ULCD::LOAD_TEXT(byte R, String Text) {
  digitalPortWrite(8); 
  WRITE_REG(72, Text.length());
  digitalPortWrite(10);                                      //Устанавливаем длину контейнера
  WRITE_REG(66, 0);
  for (int i = 0; i < Text.length(); i++) {
    WRITE_REG(72, Text[i]);        
    digitalPortWrite(17);                                    //Помещаем байт в буффер
  }
  SEND_REG(R, 65);                                        
}

void ULCD::LOAD_SCRIPT(byte R, byte Number) {
  byte script[] = {2, 48, Number, 0, 0, 0};
  digitalPortWrite(8); 
  WRITE_REG(72, 6);
  digitalPortWrite(10);                                      //Устанавливаем длину контейнера
  WRITE_REG(66, 0);
  for (int i = 0; i < 6; i++) {
    WRITE_REG(72, script[i]);    
    digitalPortWrite(17);                                    //Помещаем байт в буффер
  }
  SEND_REG(R, 65);
}

void ULCD::SET_SCALE(byte min, byte max, byte pos) {
  WRITE_REG(72, min);
  digitalPortWrite(56);
  WRITE_REG(72, max);
  digitalPortWrite(58);
  WRITE_REG(72, 1);
  digitalPortWrite(60);
  WRITE_REG(72, pos);
  digitalPortWrite(54);
}

void ULCD::SET_SCRIPT(byte R) {
  SEND_REG(72, R);                                       //Загружаем шрифт в регистр OPERAND0
  digitalPortWrite(36);
  digitalPortWrite(7);
}

void ULCD::SET_TEXT(byte R) {
  SEND_REG(72, R);                                       //Загружаем шрифт в регистр OPERAND0
  digitalPortWrite(52);
}

void ULCD::SET_PICTURE(byte R) {
  SEND_REG(72, R);                                       //Загружаем шрифт в регистр OPERAND0
  digitalPortWrite(52);
}

void ULCD::RESET() {
  digitalPortWrite(254);
  delay(100);
}
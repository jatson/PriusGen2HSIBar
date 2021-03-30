#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "Config.h"

/* LCD definitions */
#if (LCD_1602A == FEAT_AVAILABLE)
#include <LiquidCrystal.h>
#include "SpecialCharacters.h"
const int rsLCD = A0, enLCD = A1, d4LCD = A2, d5LCD = A3, d6LCD = A4, d7LCD = A5;
LiquidCrystal sLCD(rsLCD, enLCD, d4LCD, d5LCD, d6LCD, d7LCD);
int contrastPin = 8;

#elif (OLED_128x64 == FEAT_AVAILABLE)
#include <Arduino.h>
#include <U8g2lib.h>

#if (LCD_CONNECTION_TYPE == CONNECTION_SPI)
#include <SPI.h>
#elif (LCD_CONNECTION_TYPE == CONNECTION_I2C)
#include <Wire.h>
#else
#error "LCD connection tpye is not configured, please check your config"
#endif /* (LCD_CONNECTION_TYPE == CONNECTION_SPI) */

#else
#error "LCD is not configured, please check your config"
#endif /* (LCD_1602A == FEAT_AVAILABLE) */

/* LCD DIM modes - TODO */
#define DM_MIN                 0
#define DM_INJECTOR_MPG        0 // AAAA  TTT  MMM E
#define DM_TEMP_RPM            1 // AAAA  CCC  RRRRE
#define DM_INJECTOR_RPM        2 // AAAA  TTT  RRRRE
#define DM_MAX                 2

typedef class displayHandler : public U8G2_SH1106_128X64_NONAME_1_HW_I2C
{
private:
    DisplayHandler m_LCD;

public:
    displayHandler(/* args */);
    ~displayHandler();

} DisplayHandler;

#endif
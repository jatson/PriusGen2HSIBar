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
#define -DM_MIN 0
#define DM_INJECTOR_MPG 0 // AAAA  TTT  MMM E
#define DM_TEMP_RPM 1     // AAAA  CCC  RRRRE
#define DM_INJECTOR_RPM 2 // AAAA  TTT  RRRRE
#define DM_MAX 2

/* Text alignment on LCD */
#define LCD_WIDTH m_lcd.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCD_WIDTH - (m_lcd.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCD_WIDTH - m_lcd.getUTF8Width(t))
#define ALIGN_LEFT 0

typedef class displayHandler : public U8G2_SH1106_128X64_NONAME_1_HW_I2C
{
private:
    DisplayHandler m_lcd;
    uint8_t m_hsiX;
    uint8_t m_hsiY;
    uint8_t m_hsiWidth;
    uint8_t m_hsiHeight;

    void drawPwrBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent);
    void drawRegenMainBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent);
    void drawAccelMainBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent);
    void drawChgBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent);

public:
    displayHandler(const u8g2_cb_t *rotation = U8G2_R0, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE);
    ~displayHandler();

    void drawHsi(int8_t chgPercent = 0, int8_t mainRegenPercent = 0, int8_t mainAccelPercent = 0, int8_t pwrPercent = 0);
    void welcomeMessage();
    void writeOnDisplay(const String &toWrite);
} DisplayHandler;

#endif /* DISPLAYHANDLER_H */
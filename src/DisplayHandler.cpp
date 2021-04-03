#include "DisplayHandler.h"

/**
 * @brief Construct a new Display Handler::display Handler object
 * 
 * @param rotation Set the display rotation
 * @param reset Set reset pin for the display
 * @param clock Set the display clock mode
 * @param data Set the data pin for the display
 */
DisplayHandler::displayHandler(const u8g2_cb_t *rotation = U8G2_R0, uint8_t reset = U8X8_PIN_NONE, uint8_t clock = U8X8_PIN_NONE, uint8_t data = U8X8_PIN_NONE) : U8G2_SH1106_128X64_NONAME_1_HW_I2C(rotation)
{
    m_hsiX = 3;
    m_hsiY = 5;
    m_hsiWidth = 120;
    m_hsiHeight = 20;

    m_lcd.clear();
    m_lcd.begin();

    m_lcd.setFont(u8g2_font_ncenB08_tr);
}

DisplayHandler::~displayHandler()
{
}

/**
 * @brief Draw the HSI bar to the display. 
 * By default it will draw an empty bar. Giving no arguments will clear the bar.
 * If you need you can add the filling values in percentage.
 * Positive values will start filling the bar from left to right
 * Negative values will start filling the bar from right to left
 * 
 * @param chgPercent Filling percentage of CHG bar
 * @param mainPercent Filling percentage of the main bar
 * @param pwrPercent Filling percentage of the PWR bar
 */
void DisplayHandler::drawHsi(int8_t chgPercent = 0, int8_t mainPercent = 0, int8_t pwrPercent = 0)
{
    uint8_t smallBarWidth = m_hsiWidth / 4;
    uint8_t smallBarHeight = m_hsiHeight / 2;

    uint8_t chgX = m_hsiX;
    uint8_t chgY = m_hsiY + smallBarHeight;

    uint8_t mainWidth = m_hsiWidth / 2;
    uint8_t mainHeight = m_hsiHeight;
    uint8_t mainX = m_hsiX + smallBarWidth - 1;
    uint8_t mainY = m_hsiY;

    uint8_t pwrX = mainX + mainWidth - 1;
    uint8_t pwrY = mainY;

    m_lcd.firstPage();
    do
    {
        drawChgBar(chgX, chgY, smallBarWidth, smallBarHeight, chgPercent);
        drawMainBar(mainX, mainY, mainWidth, mainHeight, mainPercent);
        drawPwrBar(pwrX, pwrY, smallBarWidth, smallBarHeight, pwrPercent);
    } while (m_lcd.nextPage());
}

void DisplayHandler::drawPwrBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    m_lcd.drawFrame(x, y, width, height); /* PWR bar */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        m_lcd.drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        m_lcd.drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::drawMainBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    m_lcd.drawFrame(x, y, width, height); /* main bar */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        m_lcd.drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        m_lcd.drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::drawChgBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    m_lcd.drawFrame(x, y, width, height); /* CHG */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        m_lcd.drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        m_lcd.drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::welcomeMessage()
{
    m_lcd.firstPage();
    do
    {
        m_lcd.setFont(u8g2_font_ncenB14_tr);
        m_lcd.drawStr(ALIGN_CENTER("Welcome"), 15, "Welcome");
        m_lcd.drawStr(ALIGN_CENTER("HSI Ver.01"), 40, "HSI Ver.01");
    } while (m_lcd.nextPage());

    delay(5000);
}

void DisplayHandler::writeOnDisplay(const String &toWrite)
{
    sLCD.clear();
    m_lcd.firstPage();
    do
    {
        m_lcd.drawStr(ALIGN_CENTER(toWrite), 15, toWrite);
        m_lcd.drawStr(ALIGN_CENTER(toWrite), 40, toWrite);
    } while (m_lcd.nextPage());
}
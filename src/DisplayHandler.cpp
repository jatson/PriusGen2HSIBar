#include "DisplayHandler.h"

/**
 * @brief Construct a new Display Handler::display Handler object
 * 
 */
DisplayHandler::DisplayHandler() : U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0)
{
    m_hsiX = 3;
    m_hsiY = 5;
    m_hsiWidth = 120;
    m_hsiHeight = 20;
}

DisplayHandler::~DisplayHandler()
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
 * @param mainRegenPercent Filling percentage of the regen bar
 * @param mainAccelPercent Filling percentage of the acceleration bar
 * @param pwrPercent Filling percentage of the PWR bar
 */
void DisplayHandler::drawHsi(int8_t chgPercent, int8_t mainRegenPercent, int8_t mainAccelPercent, int8_t pwrPercent)
{
    uint8_t smallBarWidth = m_hsiWidth / 4;
    uint8_t smallBarHeight = m_hsiHeight / 2;

    uint8_t chgX = m_hsiX;
    uint8_t chgY = m_hsiY + smallBarHeight;

    uint8_t mainHeight = m_hsiHeight;
    uint8_t mainY = m_hsiY;
    uint8_t mainWidth = m_hsiWidth / 4;
    
    uint8_t mainRegenX = m_hsiX + smallBarWidth - 1;
    uint8_t mainAccX = mainRegenX + mainWidth - 1;
    
    uint8_t pwrX = mainAccX + mainWidth - 1;
    uint8_t pwrY = mainY;

    firstPage();
    do
    {
        drawChgBar(chgX, chgY, smallBarWidth, smallBarHeight, chgPercent);
        drawRegenMainBar(mainRegenX, mainY, mainWidth, mainHeight, mainRegenPercent);
        drawAccelMainBar(mainAccX, mainY, mainWidth, mainHeight, mainAccelPercent);
        drawPwrBar(pwrX, pwrY, smallBarWidth, smallBarHeight, pwrPercent);
    } while (nextPage());
}

void DisplayHandler::drawPwrBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    drawFrame(x, y, width, height); /* PWR bar */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::drawRegenMainBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    drawFrame(x, y, width, height); /* main bar */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::drawAccelMainBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    drawFrame(x, y, width, height); /* main bar */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

void DisplayHandler::drawChgBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int8_t percent = 0)
{
    drawFrame(x, y, width, height); /* CHG */

    if (percent < 0)
    {
        uint8_t barX = (x + width) - (((float)width * abs(percent)) / 100.0);
        uint8_t barWidth = x + width - barX;
        drawBox(barX, y, barWidth, height);
    }
    else if (percent > 0)
    {
        uint8_t barWidth = (((float)width * percent) / 100.0);
        drawBox(x, y, barWidth, height);
    }
    else
    {
        /* nothing to do here */
    }
}

/**
 * @brief Displays a welcome message on the LCD
 * 
 */
void DisplayHandler::welcomeMessage()
{
    firstPage();
    do
    {
        setFont(u8g2_font_ncenB14_tr);
        drawStr(ALIGN_CENTER("Welcome"), 15, "Welcome");
        drawStr(ALIGN_CENTER("HSI Ver.01"), 40, "HSI Ver 0.1");
    } while (nextPage());

    delay(5000);
}

/**
 * @brief Writes a string centered to 15px from top
 * 
 * @param toWrite String to write on the display
 */
void DisplayHandler::writeOnDisplay(const String &toWrite)
{
    char buff[toWrite.length() + 1];
    toWrite.toCharArray(buff, toWrite.length());

    firstPage();
    do
    {
        drawStr(ALIGN_CENTER(buff), 15, buff);
    } while (nextPage());
}

/**
 * @brief Write a string centered to Y px from the top
 * 
 * @param toWrite String to write on the display
 * @param y Defines the Y coordinates for the string
 */
void DisplayHandler::writeOnDisplay(const String &toWrite, uint8_t y)
{
    char buff[toWrite.length() + 1];
    toWrite.toCharArray(buff, toWrite.length());

    firstPage();
    do
    {
        drawStr(ALIGN_CENTER(buff), y, buff);
    } while (nextPage());
}

/**
 * @brief Writes a string to the display to the given coordinates
 * 
 * @param toWrite String to write on the display
 * @param x Defines the x coordinates for the string
 * @param y Defines the Y coordinates for the string
 */
void DisplayHandler::writeOnDisplay(const String &toWrite, uint8_t x, uint8_t y)
{
    char buff[toWrite.length() + 1];
    toWrite.toCharArray(buff, toWrite.length());

    firstPage();
    do
    {
        drawStr(x, y, buff);
    } while (nextPage());
}
 /**
 * @brief Writes a char array to the display to the given coordinates
 * 
 * @param toWrite Char array to write on the display
  */
void DisplayHandler::writeOnDisplay(const char *toWrite)
{
    firstPage();
    do
    {
        drawStr(ALIGN_CENTER(toWrite), 15, toWrite);
    } while (nextPage());
}

/**
 * @brief Write a char array centered to Y px from the top
 * 
 * @param toWrite Char array to write on the display
 * @param y Defines the Y coordinates for the char array
 */
void DisplayHandler::writeOnDisplay(const char *toWrite, uint8_t y)
{
    firstPage();
    do
    {
        drawStr(ALIGN_CENTER(toWrite), y, toWrite);
    } while (nextPage());
}

/**
 * @brief Writes a char array to the display to the given coordinates
 * 
 * @param toWrite Char array to write on the display
 * @param x Defines the x coordinates for the char array
 * @param y Defines the Y coordinates for the char array
 */
void DisplayHandler::writeOnDisplay(const char *toWrite, uint8_t x, uint8_t y)
{
    firstPage();
    do
    {
        drawStr(x, y, toWrite);
    } while (nextPage());
}
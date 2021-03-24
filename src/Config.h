/* Basic defines here */
#define FEAT_AVAILABLE (1)
#define FEAT_NOT_AVAILABLE (0)

#define CONNECTION_I2C (1)
#define CONNECTION_SPI (2)

/* OLED 126x64 display */
#define OLED_128x64 FEAT_AVAILABLE
#if(OLED_128x64 == FEAT_AVAILABLE)
    #define LCD_CONNECTION_TYPE CONNECTION_I2C
#endif

/* LCD 1602 display */
#define LCD_1602A FEAT_NOT_AVAILABLE
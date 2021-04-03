/* Programming of special characters to build the bar 
 * Every unsigned char is a line of pixels
 * Block size is 5x8 --> 8 lines, 5 cols
 * 1 - pixel will be ON
 * 0 - pixel will be OFF
 * 
 * 128x64 displays doesn't need this kind of implementation,
 * There U8G2 will be used
 */

/*
 *   *****
 *   *
 *   *
 *   *
 *   *
 *   *
 *   *
 *   *****
 */
unsigned char E0[8] = {0b00011111, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011111};

/*   
 *   *****
 *   **
 *   **
 *   **
 *   **
 *   **
 *   **
 *   *****
 */
unsigned char E1[8] = {0b00011111, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011111};

/*   
 *   *****
 *   ***
 *   ***
 *   ***
 *   ***
 *   ***
 *   ***
 *   *****
 */
unsigned char E2[8] = {0b00011111, 0b00011100, 0b00011100, 0b00011100, 0b00011100, 0b00011100, 0b00011100, 0b00011111};

/*   
 *   *****
 *   ****
 *   ****
 *   ****
 *   ****
 *   ****
 *   ****
 *   *****
 */
unsigned char E3[8] = {0b00011111, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011110, 0b00011111};

/*   
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 */
unsigned char E4[8] = {0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111};

/*   
 *   *****
 *       *
 *       *
 *       *
 *       *
 *       *
 *       *
 *   *****
 */
unsigned char E5[8] = {0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00011111};

/*   
 *   *****
 *        
 *        
 *        
 *        
 *        
 *        
 *   *****
 */
unsigned char E6[8] = {0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111};

/*   
 *   *****
 *   *   *
 *   *   *
 *   *   *
 *   *   *
 *   *   *
 *   *   *
 *   *****
 */
unsigned char E7[8] = {0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111};

/*   
 *   *****
 *   **  *
 *   **  *
 *   **  *
 *   **  *
 *   **  *
 *   **  *
 *   *****
 */
unsigned char E8[8] = {0b00011111, 0b00011001, 0b00011001, 0b00011001, 0b00011001, 0b00011001, 0b00011001, 0b00011111};

/*   
 *   *****
 *   *** *
 *   *** *
 *   *** *
 *   *** *
 *   *** *
 *   *** *
 *   *****
 */
unsigned char E9[8] = {0b00011111, 0b00011101, 0b00011101, 0b00011101, 0b00011101, 0b00011101, 0b00011101, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *        
 *        
 *        
 *   *****
 */
unsigned char R0[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *       *
 *       *
 *       *
 *   *****
 */
unsigned char R1[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *      **
 *      **
 *      **
 *   *****
 */
unsigned char R2[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000011, 0b00000011, 0b00000011, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *     ***
 *     ***
 *     ***
 *   *****
 */
unsigned char R3[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000111, 0b00000111, 0b00000111, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *    ****
 *    ****
 *    ****
 *   *****
 */
unsigned char R4[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00001111, 0b00001111, 0b00001111, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 */
unsigned char R5[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *   *   *
 *   *   *
 *   *   *
 *   *****
 */
unsigned char R6[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *   *  **
 *   *  **
 *   *  **
 *   *****
 */
unsigned char R7[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00010011, 0b00010011, 0b00010011, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *   * ***
 *   * ***
 *   * ***
 *   *****
 */
unsigned char R8[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00010111, 0b00010111, 0b00010111, 0b00011111};

/*   
 *        
 *        
 *        
 *   *****
 *   *    
 *   *    
 *   *    
 *   *****
 */
unsigned char R9[8] = {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00010000, 0b00010000, 0b00010000, 0b00011111};

/*   
 *   *****
 *        
 *        
 *        
 *   *****
 *        
 *        
 *        
 */
unsigned char P0[8] = {0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   *    
 *   *    
 *   *    
 *   *****
 *        
 *        
 *        
 */
unsigned char P1[8] = {0b00011111, 0b00010000, 0b00010000, 0b00010000, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   **   
 *   **   
 *   **   
 *   *****
 *        
 *        
 *        
 */
unsigned char P2[8] = {0b00011111, 0b00011000, 0b00011000, 0b00011000, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   ***  
 *   ***  
 *   ***  
 *   *****
 *        
 *        
 *        
 */
unsigned char P3[8] = {0b00011111, 0b00011100, 0b00011100, 0b00011100, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   **** 
 *   **** 
 *   **** 
 *   *****
 *        
 *        
 *        
 */
unsigned char P4[8] = {0b00011111, 0b00011110, 0b00011110, 0b00011110, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   *****
 *   *****
 *   *****
 *   *****
 *        
 *        
 *        
 */
unsigned char P5[8] = {0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *       *
 *       *
 *       *
 *   *****
 *        
 *        
 *        
 */
unsigned char P6[8] = {0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   *   *
 *   *   *
 *   *   *
 *   *****
 *        
 *        
 *        
 */
unsigned char P7[8] = {0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   **  *
 *   **  *
 *   **  *
 *   *****
 *        
 *        
 *        
 */
unsigned char P8[8] = {0b00011111, 0b00011001, 0b00011001, 0b00011001, 0b00011111, 0b00000000, 0b00000000, 0b00000000};

/*   
 *   *****
 *   *** *
 *   *** *
 *   *** *
 *   *****
 *        
 *        
 *        
 */
unsigned char P9[8] = {0b00011111, 0b00011101, 0b00011101, 0b00011101, 0b00011111, 0b00000000, 0b00000000, 0b00000000};


// function to program special characters
// receives an integer 0-7 representing the ascii number and a string of bytes with drawing
void customize_character(int x, byte *desenho)
{
    int i = 0;
    for (i = 0; i < 8; i++)
    {
        sLCD.write((uint8_t)0x40 | (x << 3) | i); // Set the CGRAM address
        sLCD.write((uint8_t)desenho[i]);          // Set the character
    }
}

//---------------------------------------------------------------------------------------------------

//function receiving a value of R and R program, the special characters and prints the string of CHGzone

void drawbar_R(int r)
{
    if (debug == 1)
    {
        sLCD.setCursor(0, 0);
        sprintf(buffer, "%3d", (int)r);
        sLCD.print(buffer);
        return;
    }

    char devolve[3];

    // if R = 0 then the bar is empty and has to use only 2 characters (0 and 1) to release the remaining for other areas
    // if R! = 0 then uses the 3 characters, 0, 1 and 2
    if (r == 0 && RA != 0) // if the first time reaches 0 writes on the LCD, if repeated does nothing
    {
        // R9 R0 R0 E0 E6 E6 E6 E6 E6 E5 E6 E5 E6 E6 P0 P6
        customize_character(0, R9);
        customize_character(1, R0);

        devolve[0] = 0;
        devolve[1] = 1;
        devolve[2] = 1;

        // prints 3 characters
        sLCD.setCursor(0, 0);
        for (count = 0; count <= 2; count++)
        {
            sLCD.write((uint8_t)devolve[count]);
        }
    }

    if (r >= 1 && r <= 6)
    {
        if (RA == 0)
        {
            switch (r)
            {
            case 1:
                customize_character(2, R1);
                break;
            case 2:
                customize_character(2, R2);
                break;
            case 3:
                customize_character(2, R3);
                break;
            case 4:
                customize_character(2, R4);
                break;
            case 5:
                customize_character(2, R5);
                break;
            case 6:
                customize_character(2, R5);
                break;
            }

            devolve[2] = 2;
            sLCD.setCursor(2, 0);
            sLCD.write((uint8_t)devolve[2]);
        }

        if (RA >= 1 && RA <= 6)
        {
            switch (r)
            {
            case 1:
                customize_character(2, R1);
                break;
            case 2:
                customize_character(2, R2);
                break;
            case 3:
                customize_character(2, R3);
                break;
            case 4:
                customize_character(2, R4);
                break;
            case 5:
                customize_character(2, R5);
                break;
            case 6:
                customize_character(2, R5);
                break;
            }
        }

        if (RA >= 7 && RA <= 12)
        {
            switch (r)
            {
            case 1:
                customize_character(2, R1);
                customize_character(1, R0);
                break;
            case 2:
                customize_character(2, R2);
                customize_character(1, R0);
                break;
            case 3:
                customize_character(2, R3);
                customize_character(1, R0);
                break;
            case 4:
                customize_character(2, R4);
                customize_character(1, R0);
                break;
            case 5:
                customize_character(2, R5);
                customize_character(1, R0);
                break;
            case 6:
                customize_character(2, R5);
                customize_character(1, R0);
                break;
            }
        }

        if (RA >= 13 && RA <= 16)
        {
            switch (r)
            {
            case 1:
                customize_character(2, R1);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            case 2:
                customize_character(2, R2);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            case 3:
                customize_character(2, R3);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            case 4:
                customize_character(2, R4);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            case 5:
                customize_character(2, R5);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            case 6:
                customize_character(2, R5);
                customize_character(1, R0);
                customize_character(0, R9);
                break;
            }
        }
    }

    if (r >= 7 && r <= 12)
    {
        if (RA == 0)
        {
            switch (r)
            {
            case 7:
                customize_character(2, R5);
                customize_character(1, R1);
                break;
            case 8:
                customize_character(2, R5);
                customize_character(1, R2);
                break;
            case 9:
                customize_character(2, R5);
                customize_character(1, R3);
                break;
            case 10:
                customize_character(2, R5);
                customize_character(1, R4);
                break;
            case 11:
                customize_character(2, R5);
                customize_character(1, R5);
                break;
            case 12:
                customize_character(2, R5);
                customize_character(1, R5);
                break;
            }
            devolve[2] = 2;
            sLCD.setCursor(2, 0);
            sLCD.write((uint8_t)devolve[2]);
        }

        if (RA >= 1 && RA <= 6)
        {
            switch (r)
            {
            case 7:
                customize_character(2, R5);
                customize_character(1, R1);
                break;
            case 8:
                customize_character(2, R5);
                customize_character(1, R2);
                break;
            case 9:
                customize_character(2, R5);
                customize_character(1, R3);
                break;
            case 10:
                customize_character(2, R5);
                customize_character(1, R4);
                break;
            case 11:
                customize_character(2, R5);
                customize_character(1, R5);
                break;
            case 12:
                customize_character(2, R5);
                customize_character(1, R5);
                break;
            }
        }

        if (RA >= 7 && RA <= 12)
        {
            switch (r)
            {
            case 7:
                customize_character(1, R1);
                break;
            case 8:
                customize_character(1, R2);
                break;
            case 9:
                customize_character(1, R3);
                break;
            case 10:
                customize_character(1, R4);
                break;
            case 11:
                customize_character(1, R5);
                break;
            case 12:
                customize_character(1, R5);
                break;
            }
        }

        if (RA >= 13 && RA <= 16)
        {
            switch (r)
            {
            case 7:
                customize_character(1, R1);
                customize_character(0, R9);
                break;
            case 8:
                customize_character(1, R2);
                customize_character(0, R9);
                break;
            case 9:
                customize_character(1, R3);
                customize_character(0, R9);
                break;
            case 10:
                customize_character(1, R4);
                customize_character(0, R9);
                break;
            case 11:
                customize_character(1, R5);
                customize_character(0, R9);
                break;
            case 12:
                customize_character(1, R5);
                customize_character(0, R9);
                break;
            }
        }
    }

    if (r >= 13 && r <= 16)
    {
        if (RA == 0)
        {
            switch (r)
            {
            case 13:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R6);
                break;
            case 14:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R7);
                break;
            case 15:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R8);
                break;
            case 16:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R5);
                break;
            }
            devolve[2] = 2;
            sLCD.setCursor(2, 0);
            sLCD.write((uint8_t)devolve[2]);
        }

        if (RA >= 1 && RA <= 6)
        {
            switch (r)
            {
            case 13:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R6);
                break;
            case 14:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R7);
                break;
            case 15:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R8);
                break;
            case 16:
                customize_character(2, R5);
                customize_character(1, R5);
                customize_character(0, R5);
                break;
            }
        }

        if (RA >= 7 && RA <= 12)
        {
            switch (r)
            {
            case 13:
                customize_character(1, R5);
                customize_character(0, R6);
                break;
            case 14:
                customize_character(1, R5);
                customize_character(0, R7);
                break;
            case 15:
                customize_character(1, R5);
                customize_character(0, R8);
                break;
            case 16:
                customize_character(1, R5);
                customize_character(0, R5);
                break;
            }
        }

        if (RA >= 13 && RA <= 16)
        {
            switch (r)
            {
            case 13:
                customize_character(0, R6);
                break;
            case 14:
                customize_character(0, R7);
                break;
            case 15:
                customize_character(0, R8);
                break;
            case 16:
                customize_character(0, R5);
                break;
            }
        }
    }

    RA = r;
}

// -----------------------------------------------------------------------------------------------------------------
//Home
void drawbar_E(int e)
{
    if (debug == 1)
    {
        sLCD.setCursor(6, 0);
        sprintf(buffer, "%3d", (int)e);
        sLCD.print(buffer);
        return;
    }

    if (e == 0) // if 0 write all
    {
        if (EA != 0)
        {
            customize_character(3, E6);
            customize_character(4, E5);
            customize_character(5, E0);

            flagE4 = false;
            flagE5 = false;
            flagE6 = false;

            esc[0] = 5;
            esc[1] = 3;
            esc[2] = 3;
            esc[3] = 3;
            esc[4] = 3;
            esc[5] = 4;
            esc[6] = 3;
            esc[7] = 3;
            esc[8] = 3;
            esc[9] = 3;
            esc[10] = 4;

            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            if (EA >= 5)
                sLCD.write((uint8_t)esc[1]);
            if (EA >= 11)
                sLCD.write((uint8_t)esc[2]);
            if (EA >= 17)
                sLCD.write((uint8_t)esc[3]);
            if (EA >= 23)
                sLCD.write((uint8_t)esc[4]);
            if (EA >= 29)
                sLCD.write((uint8_t)esc[5]);
            if (EA >= 35)
                sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
    }
    else // e!=0
    {
        if (!flagE4)
        {
            customize_character(2, E4);
            flagE4 = true;
        };

        if (e <= 53 && !flagE6)
        {
            customize_character(3, E6);
            flagE6 = true;
        }

        if (e <= 59 && !flagE5)
        {
            customize_character(4, E5);
            flagE5 = true;
        }
    }

    //----------------------------------------------
    if (e >= 1 && e <= 5)
    {
        esc[0] = 5;
        esc[1] = 3;
        esc[2] = 3;
        esc[3] = 3;
        esc[4] = 3;
        esc[5] = 4;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        sLCD.setCursor(3, 0);
        sLCD.write((uint8_t)esc[0]);
        if (EA >= 5)
            sLCD.write((uint8_t)esc[1]);
        if (EA >= 11)
            sLCD.write((uint8_t)esc[2]);
        if (EA >= 17)
            sLCD.write((uint8_t)esc[3]);
        if (EA >= 23)
            sLCD.write((uint8_t)esc[4]);
        if (EA >= 29)
            sLCD.write((uint8_t)esc[5]);
        if (EA >= 35)
            sLCD.write((uint8_t)esc[6]);
        if (EA >= 41)
            sLCD.write((uint8_t)esc[7]);
        if (EA >= 47)
            sLCD.write((uint8_t)esc[8]);
        if (EA >= 53)
            sLCD.write((uint8_t)esc[9]);
        if (EA >= 59)
            sLCD.write((uint8_t)esc[10]);

        switch (e)
        {
        case 1:
            customize_character(5, E1);
            break;
        case 2:
            customize_character(5, E2);
            break;
        case 3:
            customize_character(5, E3);
            break;
        case 4:
            customize_character(5, E4);
            break;
        case 5:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 6 && e <= 11)
    {
        esc[0] = 2;
        esc[1] = 5;
        esc[2] = 3;
        esc[3] = 3;
        esc[4] = 3;
        esc[5] = 4;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 11)
        {
            sLCD.setCursor(4, 0);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            if (EA >= 17)
                sLCD.write((uint8_t)esc[3]);
            if (EA >= 23)
                sLCD.write((uint8_t)esc[4]);
            if (EA >= 29)
                sLCD.write((uint8_t)esc[5]);
            if (EA >= 35)
                sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);

            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
        }

        switch (e)
        {
        case 6:
            customize_character(5, E0);
            break;
        case 7:
            customize_character(5, E1);
            break;
        case 8:
            customize_character(5, E2);
            break;
        case 9:
            customize_character(5, E3);
            break;
        case 10:
            customize_character(5, E4);
            break;
        case 11:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 12 && e <= 17)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 5;
        esc[3] = 3;
        esc[4] = 3;
        esc[5] = 4;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 17)
        {
            sLCD.setCursor(5, 0);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            if (EA >= 23)
                sLCD.write((uint8_t)esc[4]);
            if (EA >= 29)
                sLCD.write((uint8_t)esc[5]);
            if (EA >= 35)
                sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);

            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
        }

        switch (e)
        {
        case 12:
            customize_character(5, E0);
            break;
        case 13:
            customize_character(5, E1);
            break;
        case 14:
            customize_character(5, E2);
            break;
        case 15:
            customize_character(5, E3);
            break;
        case 16:
            customize_character(5, E4);
            break;
        case 17:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 18 && e <= 23)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 5;
        esc[4] = 3;
        esc[5] = 4;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 23)
        {
            sLCD.setCursor(6, 0);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
            if (EA >= 29)
                sLCD.write((uint8_t)esc[5]);
            if (EA >= 35)
                sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
        }

        switch (e)
        {
        case 18:
            customize_character(5, E0);
            break;
        case 19:
            customize_character(5, E1);
            break;
        case 20:
            customize_character(5, E2);
            break;
        case 21:
            customize_character(5, E3);
            break;
        case 22:
            customize_character(5, E4);
            break;
        case 23:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 24 && e <= 29)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 5;
        esc[5] = 4;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 29)
        {
            sLCD.setCursor(7, 0);
            sLCD.write((uint8_t)esc[4]);
            sLCD.write((uint8_t)esc[5]);
            if (EA >= 35)
                sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
        }

        switch (e)
        {
        case 24:
            customize_character(5, E0);
            break;
        case 25:
            customize_character(5, E1);
            break;
        case 26:
            customize_character(5, E2);
            break;
        case 27:
            customize_character(5, E3);
            break;
        case 28:
            customize_character(5, E4);
            break;
        case 29:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 30 && e <= 35)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 5;
        esc[6] = 3;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 35)
        {
            sLCD.setCursor(8, 0);
            sLCD.write((uint8_t)esc[5]);
            sLCD.write((uint8_t)esc[6]);
            if (EA >= 41)
                sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
            sLCD.write((uint8_t)esc[5]);
        }

        switch (e)
        {
        case 30:
            customize_character(5, E7);
            break;
        case 31:
            customize_character(5, E8);
            break;
        case 32:
            customize_character(5, E9);
            break;
        case 33:
            customize_character(5, E4);
            break;
        case 34:
            customize_character(5, E4);
            break;
        case 35:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 36 && e <= 41)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 2;
        esc[6] = 5;
        esc[7] = 3;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 41)
        {
            sLCD.setCursor(9, 0);
            sLCD.write((uint8_t)esc[6]);
            sLCD.write((uint8_t)esc[7]);
            if (EA >= 47)
                sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
            sLCD.write((uint8_t)esc[5]);
            sLCD.write((uint8_t)esc[6]);
        }

        switch (e)
        {
        case 36:
            customize_character(5, E0);
            break;
        case 37:
            customize_character(5, E1);
            break;
        case 38:
            customize_character(5, E2);
            break;
        case 39:
            customize_character(5, E3);
            break;
        case 40:
            customize_character(5, E4);
            break;
        case 41:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 42 && e <= 47)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 2;
        esc[6] = 2;
        esc[7] = 5;
        esc[8] = 3;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 47)
        {
            sLCD.setCursor(10, 0);
            sLCD.write((uint8_t)esc[7]);
            sLCD.write((uint8_t)esc[8]);
            if (EA >= 53)
                sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);
            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
            sLCD.write((uint8_t)esc[5]);
            sLCD.write((uint8_t)esc[6]);
            sLCD.write((uint8_t)esc[7]);
        }

        switch (e)
        {
        case 42:
            customize_character(5, E0);
            break;
        case 43:
            customize_character(5, E1);
            break;
        case 44:
            customize_character(5, E2);
            break;
        case 45:
            customize_character(5, E3);
            break;
        case 46:
            customize_character(5, E4);
            break;
        case 47:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 48 && e <= 53)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 2;
        esc[6] = 2;
        esc[7] = 2;
        esc[8] = 5;
        esc[9] = 3;
        esc[10] = 4;

        if (EA >= 53)
        {
            sLCD.setCursor(11, 0);
            sLCD.write((uint8_t)esc[8]);
            sLCD.write((uint8_t)esc[9]);
            if (EA >= 59)
                sLCD.write((uint8_t)esc[10]);
        }
        else
        {
            sLCD.setCursor(3, 0);

            sLCD.write((uint8_t)esc[0]);
            sLCD.write((uint8_t)esc[1]);
            sLCD.write((uint8_t)esc[2]);
            sLCD.write((uint8_t)esc[3]);
            sLCD.write((uint8_t)esc[4]);
            sLCD.write((uint8_t)esc[5]);
            sLCD.write((uint8_t)esc[6]);
            sLCD.write((uint8_t)esc[7]);
            sLCD.write((uint8_t)esc[8]);
        }

        switch (e)
        {
        case 48:
            customize_character(5, E0);
            break;
        case 49:
            customize_character(5, E1);
            break;
        case 50:
            customize_character(5, E2);
            break;
        case 51:
            customize_character(5, E3);
            break;
        case 52:
            customize_character(5, E4);
            break;
        case 53:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 54 && e <= 59)
    {
        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 2;
        esc[6] = 2;
        esc[7] = 2;
        esc[8] = 2;
        esc[9] = 5;
        esc[10] = 4;

        sLCD.setCursor(3, 0);
        sLCD.write((uint8_t)esc[0]);
        sLCD.write((uint8_t)esc[1]);
        sLCD.write((uint8_t)esc[2]);
        sLCD.write((uint8_t)esc[3]);
        sLCD.write((uint8_t)esc[4]);
        sLCD.write((uint8_t)esc[5]);
        sLCD.write((uint8_t)esc[6]);
        sLCD.write((uint8_t)esc[7]);
        sLCD.write((uint8_t)esc[8]);
        sLCD.write((uint8_t)esc[9]);
        if (EA >= 59)
            sLCD.write((uint8_t)esc[10]);

        switch (e)
        {
        case 54:
            customize_character(5, E0);
            break;
        case 55:
            customize_character(5, E1);
            break;
        case 56:
            customize_character(5, E2);
            break;
        case 57:
            customize_character(5, E3);
            break;
        case 58:
            customize_character(5, E4);
            break;
        case 59:
            customize_character(5, E4);
            break;
        }
    }

    //-------------------------------------------------------------------
    if (e >= 60 && e <= 63)
    {
        switch (e)
        {
        case 60:
            customize_character(5, E7);
            break;
        case 61:
            customize_character(5, E8);
            break;
        case 62:
            customize_character(5, E9);
            break;
        case 63:
            customize_character(5, E4);
            break;
        }

        esc[0] = 2;
        esc[1] = 2;
        esc[2] = 2;
        esc[3] = 2;
        esc[4] = 2;
        esc[5] = 2;
        esc[6] = 2;
        esc[7] = 2;
        esc[8] = 2;
        esc[9] = 2;
        esc[10] = 5;

        sLCD.setCursor(3, 0);

        sLCD.write((uint8_t)esc[0]);
        sLCD.write((uint8_t)esc[1]);
        sLCD.write((uint8_t)esc[2]);
        sLCD.write((uint8_t)esc[3]);
        sLCD.write((uint8_t)esc[4]);
        sLCD.write((uint8_t)esc[5]);
        sLCD.write((uint8_t)esc[6]);
        sLCD.write((uint8_t)esc[7]);
        sLCD.write((uint8_t)esc[8]);
        sLCD.write((uint8_t)esc[9]);
        sLCD.write((uint8_t)esc[10]);
    }
    EA = e;
}

//------------------------------------------------------------------------------------------------
//PWRzone
void drawbar_P(int p)
{
    if (debug == 1)
    {
        sLCD.setCursor(13, 0);
        sprintf(buffer, "%3d", (int)p);
        sLCD.print(buffer);
        return;
    }

    char devolve[2];
    if (p == 0 && PA != 0) // if the first time it reaches 0 writes on the LCD, if repeated does nothing
    {
        // P0 P6
        customize_character(6, P0);
        customize_character(7, P6);

        devolve[0] = 6;
        devolve[1] = 7;

        sLCD.setCursor(14, 0);
        sLCD.write((uint8_t)devolve[0]);
        sLCD.write((uint8_t)devolve[1]);
    }

    if (p >= 1 && p <= 6)
    {
        if (PA >= 0 && PA <= 6)
        {
            switch (p)
            {
            case 1:
                customize_character(6, P1);
                break;
            case 2:
                customize_character(6, P2);
                break;
            case 3:
                customize_character(6, P3);
                break;
            case 4:
                customize_character(6, P4);
                break;
            case 5:
                customize_character(6, P5);
                break;
            case 6:
                customize_character(6, P5);
                break;
            }
        }
    }

    if (PA >= 7 && PA <= 10)
    {
        switch (p)
        {
        case 1:
            customize_character(6, P1);
            customize_character(7, P6);
            break;
        case 2:
            customize_character(6, P2);
            customize_character(7, P6);
            break;
        case 3:
            customize_character(6, P3);
            customize_character(7, P6);
            break;
        case 4:
            customize_character(6, P4);
            customize_character(7, P6);
            break;
        case 5:
            customize_character(6, P5);
            customize_character(7, P6);
            break;
        case 6:
            customize_character(6, P5);
            customize_character(7, P6);
            break;
        }
    }

    if (p >= 7 && p <= 10)
    {
        if (PA >= 0 && PA <= 6)
        {
            switch (p)
            {
            case 7:
                customize_character(6, P5);
                customize_character(7, P7);
                break;
            case 8:
                customize_character(6, P5);
                customize_character(7, P8);
                break;
            case 9:
                customize_character(6, P5);
                customize_character(7, P9);
                break;
            case 10:
                customize_character(6, P5);
                customize_character(7, P5);
                break;
            }
        }

        if (PA >= 7 && PA <= 10)
        {
            switch (p)
            {
            case 7:
                customize_character(7, P7);
                break;
            case 8:
                customize_character(7, P8);
                break;
            case 9:
                customize_character(7, P9);
                break;
            case 10:
                customize_character(7, P5);
                break;
            }
        }
    }
    PA = p;
}

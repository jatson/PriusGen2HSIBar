/* Project HSI bar for Prius 2G
  Requires the CAN-bus shield for Arduino.
  The data are displayed on the LCD Serial
  
  4-22-14 v1.091 updates by Chris Dragon:
  Added MPG and fuel injection timing displays.
  Added ability to toggle between showing amps and fuel injection or MPG and coolant temp using left and right d-pad on Sparkfun CAN-Bus.
  Added debug modes 1 through 4 accessible by pressing up on d-pad.
  Press center d-pad to clear display and reset debug and MPG display to default.
  English translation of source code.
  Modified to work in Arduino IDE 1.0.5.
  Fixed gearShift values to work with more cars (including my own).
  Limit redrawing amps more than once every 500ms to prevent fast flickering that makes the number unreadable.
  Negative engine RPMs are now displayed as positive followed by B for engine brake.  Hitting negative RPMs seems to only happen on an extended steep hill.
  
  4-24-14 v1.092
  Changed single letter 'R' after RPM to 'G' for Gas.  Added 'C' for Combined when both gas and electric are running.
  Replaced RPM on default displayMode with MPG, leaving single letter to indicate electric, ICE, or combined.
  Added displayMode DM_INJECTOR_RPM.
  Changing displayMode now clears screen so you can be sure the mode changed when the car is not sending many
  display updates.
  Fixed displayMode > 0 to update battery amps.

  2021. 03. 20 v0.1 by Barna Zoboki
  */

#include "CAN/mcp_can.h"
#include "Config.h"
#include "DisplayHandler.h"

/* LCD */
DisplayHandler sLCD;

/* CAN BUS definitions */
#define CAN_125 7 // CAN speed to 125 kbps
#define CAN_250 3 // 250 kbps speed CAN
#define CAN_500 1 // the speed CAN 500 kbps
MCP_CAN CAN(9);   // Set CS to pin 10

/* Joystick pin definitions */
#define UP A1
#define RIGHT A2
#define DOWN A3
#define CLICK A4
#define LEFT A5

#define LED2 7 // pins for the leds 2:03
#define LED3 6
#define FALTA 5

/* Gear position states */
#define GS_PARK 0
#define GS_REVERSE 1
#define GS_NEUTRAL 2
#define GS_DRIVE 3  // Labeled "D" or "A" on shifter
#define GS_EBRAKE 4 // Engine brake.  Labeled "B" on shifter

/* Static buffer variables */
char buffer[32]; // Buffer to write temporary data on the LCD
char esc[11];    // To write the bar and
int data[12];    // receive data functions

/* Static variables */
int throttlePedal;    // 0-100% throttle position
int throttlePedalRaw; // throttle position 0 -200
int brakePedal;       // brake position 0-100%
int rpm;              // 0-4500 rpm rpm
int batteryAmps;      // current / battery -125 to 125A
int coolantTemp;      // temperature of the cooling water 0-200 C
int kph;              // speed in kmh 0-176
int gearShift;        // Drive mode (position of gear shift)
int stateOfCharge;    // battery 0-100%
int EVMode;           // EVmode
int dashBrightness;   // brightness panel
int oldDashBrightness = 10;
int mpg = 0;
int injectorTiming = 0;

/* variables for Bar */
int R = 0;
int E = 0;
int P = 0;

/* points of initial variables bar */
int pointA = 20;
int pointB = 35;
int pointC = 65;

boolean flagE4 = false;
boolean flagE5 = false;
boolean flagE6 = false;

boolean message_ok0 = false;
boolean message_ok1 = false;
boolean message_ok2 = false;
boolean message_ok3 = false;
boolean message_ok4 = false;
boolean message_ok5 = false;
boolean message_ok6 = false;
boolean message_ok7 = false;
boolean message_ok8 = false;
boolean message_ok9 = false;
boolean message_ok10 = false;
boolean message_ok11 = false;

/* variables for the previous to the currentbar */
int RA = 0;
int EA = 0;
int PA = 0;

/* variables to make beadswhile reading the ECU */
int value;
long value2;
float floatValue;
int timeout = 0;

/* Timers */
unsigned long time1 = 0;
// unsigned long time2 = 0,
// unsigned long time3 = 0,
// unsigned long time4 = 0

int count = 0;  // General Variable
int count2 = 0; // Variable Overall
int flag = 0;

int debug = 0;
int displayMode = DM_MIN;
//unsigned long msLastLPS = millis();
unsigned long msLastAmps = millis();
unsigned long msLastInjector = millis();

/* Function declarations */
void customize_character(int x, byte *desenho);
char inic(unsigned char speed);
void ecu_3(int *data);

void setup()
{
    /* LED setup - Turned off for now */
    /*pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);*/

    /* Joystick setup - TODO */
    /*pinMode(UP,INPUT);
    pinMode(DOWN,INPUT);
    pinMode(LEFT,INPUT);
    pinMode(RIGHT,INPUT);
    pinMode(CLICK,INPUT);
    digitalWrite(UP, HIGH);       // connecting the internal pull-ups on inputs from the joystick
    digitalWrite(DOWN, HIGH);
    digitalWrite(LEFT, HIGH);
    digitalWrite(RIGHT, HIGH);
    digitalWrite(CLICK, HIGH);*/

    /* Initialize the MCP2515 CAN controller to the selected speed, 500 for the Prius */
    if (inic(CAN_500) == CAN_OK)
    {
        sLCD.writeOnDisplay("Ready");
        delay(2000);
        sLCD.writeOnDisplay("Turn on the car!");
        delay(2000);
    }
    /* CAN init failed */
    else
    {
        sLCD.writeOnDisplay("Startup error!");
        delay(10000);
    }

    sLCD.clear();
    sLCD.drawHsi();
}

void loop()
{
    // read data from the CAN BUS
    ecu_3(data);

    int dpadUp = digitalRead(UP);
    if (dpadUp == 0 || digitalRead(DOWN) == 0)
    {
        if (dpadUp == 0)
            debug++;
        else
            debug--;

        sLCD.clear();
        if (debug == 0) sLCD.drawHsi();

        sLCD.setCursor(0, 1);
        sprintf(buffer, "%d", (int)debug); // Show value of 'debug' var in lower left
        sLCD.print(buffer);
        delay(500);
    }

    int dpadLeft = digitalRead(LEFT);
    if (dpadLeft == 0 || digitalRead(RIGHT) == 0)
    {
        if (dpadLeft == 0)
            displayMode--;
        else
            displayMode++;

        if (displayMode < DM_MIN)
            displayMode = DM_MAX;
        if (displayMode > DM_MAX)
            displayMode = DM_MIN;

        sLCD.clear();
        if (debug == 0) sLCD.drawHsi();
    }

    if (digitalRead(CLICK) == 0)
    {
        // Clear LCD and reset all display/debug values to default
        displayMode = DM_MIN;
        debug = 0;
        sLCD.clear();
        sLCD.drawHsi();
    }

    // EVso
    if (data[0] != 9999)
        EVMode = data[0];

    // Display coolant temperature if available
    if (displayMode == DM_TEMP_RPM && data[1] != 9999)
    {
        coolantTemp = data[1];
        sLCD.setCursor(6, 1);
        sprintf(buffer, "%3dC", (int)coolantTemp);
        sLCD.print(buffer);
    }
    else if ((displayMode == DM_INJECTOR_MPG || displayMode == DM_INJECTOR_RPM) && millis() - msLastInjector > 500)
    {
        if (data[11] != 9999)
            injectorTiming = data[11];
        else if (rpm < 970)
        { /* injectorTiming && millis() - msLastInjector > 1000) {
             * When the ICE turns off, the car continues to report whatever injector timing value was last used (I've confirmed
             * the ECU is still responding to our timing queries but the string of data it returns does not change at all).  I'd
             * rather show 0 on the display but I'm not sure how to detect ICE off reliably since rpm doesn't go to 0
             * when the car is moving.  Elsewhere in the code it takes < 970 rpm to mean ICE is off which makes sense
             * because ICE is inefficient at under 1000 rpms so it's not supposed to run. Instead, assume 0 injector timing if we
             * see rpm < 970.
             *
             * From http://techno-fandom.org/~hobbit/cars/sweet/refl.html
             * One caveat about injector time is that it does *not* go to 0.0 when the engine ceases to inject, whether it's
             * spinning in warp-stealth or stopped. This is because after a shutdown command, the Prius engine ECM continues
             * to report several parameters, including last-seen injection time, that *were* in effect when the engine was
             * running and supplying power. This can show anywhere from 1 to 4 ms depending on how the shutdown happened,
             * but it is completely *false* since the engine isn't actually injecting any fuel in those states. Proof that such
             * reporting is false can be had with a simple LED monitor on an injector line. This is one of several such bugs in
             * the ECM firmware; there are quite a few important parameters that continue being reported from values "frozen"
             * at engine shutdown time. So injector width isn't a good indication of warp stealth either.
             *
             * Warmed-up base engine idle seems to require about 1.2 milliseconds of squirt. As driver demand increases the
             * throttle is opened fairly quickly and the injector time rapidly rises to about 5 ms and plateaus to rise more
             * slowly from there up, following the same general pattern as the operation line. But five milliseconds isn't
             * quite where we want to be yet; that corresponds to about the 10 kW operational "knee" we must get beyond.
             * Conveniently, the 15 kW region seems to center around 6 milliseconds, so trying to stay between 5.8 and 6.3 ms
             * as displayed accomplishes the same highway goal. The 2800 - 3000 RPM power burst corresponds to about 7 ms or
             * slightly less. Watching injector time also helps at lower speeds, as it helps indicate full loading there as
             * well. Just off the line up to about 25 MPH, low-RPM torque winds up using a slightly longer injection pulse like
             * 6.7 and then starts shrinking as the driveability map starts to shift upward. */
            injectorTiming = 0;
        }

        sLCD.setCursor(6, 1);
        // injectorTiming is in 1/8 ms units.  I was originally going to display this with three characters, so 7.125ms would
        // be displayed as 712i, but then I realized that 1/8 = 0.125 which means the value jumps from 7.0 to 7.125 to
        // 7.250 etc, which basically means the second character always changes, making the third character simply a
        // distraction.
        // Examples of injectorTiming values: 0x2b = 43/8 = 5.375ms, 0x13 = 19/8 = 2.375ms, 0x31 = 49/8 = 6.125ms
        // Since integer conversion always rounds down, I add 0.5 which causes the integer conversion to round to the nearest
        // value instead (it rounds up if the original number was between (x).5  and (x+1).49, otherwise it rounds down).
        sprintf(buffer, "%2dT ", (int)(((float)(injectorTiming * 10) / 8.0) + 0.5));
        sLCD.print(buffer);
        msLastInjector = millis();
    }

    /* Deal with changes in the dashboard dimmer state - TODO DIMMING */
    /*if ( data[2] !=9999)
    {
        dashBrightness = data[2];
        if (dashBrightness != oldDashBrightness && flag==0) // if there are changes and not in hibernation adjusts the brightness
        {
            if (dashBrightness == 24)
            {
                // Adjust the brightness min = 80LCD
                sLCD.write((uint8_t)DIM);
                sLCD.write((uint8_t)0x80);
                delay (200);
            }
            else
            {
                // Sets Brightness LCD max = 9D
                sLCD.write((uint8_t)DIM);
                sLCD.write((uint8_t)0x9D);
                delay(200);
            }
            oldDashBrightness = dashBrightness;  // update the currentvalue
            EA=63;           // forces draw bar on waking from hibernation
            PA=10;
            RA=16;
        }
    }*/

    if (millis() - time1 >= 10000) // check if it is not turned off within 10 seconds 10
    {
        count2 = 0;
        for (count = 0; count <= (FALTA - 1); count++)
        {
            if (data[count] == 9999)
                count2 = count2 + 1;
        }

        if (count2 == FALTA) // if it is off the LCD backlight off
        {
            oldDashBrightness = 10; // *** ************************************************** *********
            dashBrightness = 10;

            if (flag == 0) // if it is still connected and warns off
            {
                clear_lcd();
                sLCD.setCursor(0, 0);
                sLCD.print("   No signal    ");
                sLCD.setCursor(0, 1);
                sLCD.print(" Hibernating... ");

                /* Dimming LCD - TODO */
                //sLCD.write((uint8_t)DIM);
                //sLCD.write((uint8_t)0x80);  // switch the lighting

                delay(5000);
                clear_lcd();
                flag = 1; // update the flag hibernation
            }
        }
        else
        {
            flag = 0;
        }

        time1 = millis();
    }

    int mpg = -1;

    /* read speed */
    if (data[7] != 9999)
    {
        kph = data[7];
        if ((gearShift & 0x7) == GS_REVERSE)
        {
            kph = kph * -1; // if the car is in reverse, make the velocity negative
        }
        if (kph == 0)
        {
            mpg = 0;
        }
    }

    /* RPM */
    if (data[3] != 9999)
    {
        rpm = data[3];
        if (kph > 0 && rpm < 200)
        {
            mpg = 99;
        }

        char chrSuffix = 'E'; // Default to electric
        if (rpm < 0)
        {
            chrSuffix = 'B'; // Negative RPM shows up under certain circumstances when engine brake is on.
            rpm = -rpm;
        }
        else
        {
            if (rpm >= 970)
            {
                if (rpm > 2000 && batteryAmps > 9)
                {
                    // batteryAmps > 9 is arbitrary and should be based on a sensor somehow.  I've seen the
                    // car use anywhere from 1 amp to 8 amps with AC on full.  If you have the AC off, then
                    // if amps heads above 2 we should consider it to be combined electric and gas power.
                    chrSuffix = 'C'; // Combined gas and electric
                }
                else
                {
                    chrSuffix = 'G'; // Gas
                }
            }
        }

        if (displayMode == DM_TEMP_RPM || displayMode == DM_INJECTOR_RPM)
        {
            sLCD.setCursor(11, 1);
            if (EVMode == 64)
            {
                sLCD.print("   EV"); // whether in order writes EV EV
            }
            else
            {
                sprintf(buffer, "%4d", (int)rpm);
                sLCD.print(buffer);
                sLCD.print(chrSuffix);
            }
        }
        else if (displayMode == DM_INJECTOR_MPG)
        {
            sLCD.setCursor(1, 15);
            sLCD.print(chrSuffix);
        }
    }

    /* MPG */
    if (displayMode == DM_INJECTOR_MPG)
    {
        if (data[10] != 9999)
        {
            int lps = data[10];
            if (kph == 0)
            {
                mpg = 0; // if speed is 0 then set MPG to 0
            }
            else
            {
                if (rpm < 200 || lps == 0)
                {
                    mpg = 99; // set max MPG if speed>0 but no fuel is used
                }
                else
                {
                    // The calculation priidash uses seems to produce a value about 70% higher than the value shown on the MFD,
                    // such as 48 mpg when MFD shows 31 (both values held steady for at least 4 seconds).
                    // I tried sample values of 15kph, 1000 rpm and 800 lps and got:
                    // 378.541 * 15 * 3600 / 1000 / 800 = 25.5mpg
                    // The XLS file mentions another calculation for MPG:  MPH * 1100 / lps
                    // I tried the same sample data with 15 kph converted to 9.3mph:
                    // 9.3 * 1100 / 800 = 12.78
                    // That seems a lot closer to what the MFD would show but it may actually be too low.  Using KPH instead
                    // results in 20 which seems too high.  Argh.
                    // I went through the Priidash source again and realized that the speed they use in their formula is mph
                    // instead of kph, which gets us a value that looks correct:
                    // 378.541 * 9.3 * 3600 / 1000 / 800 = 15.8mpg
                    // Priidash also uses the speed of the rear wheels instead of the general speed of the car instrument
                    // I think because it's updated more often?  I'm not going to bother with that.
                    // I added * 0.621371 to the formula which converts kph to mph.
                    // This formula stays within 1mph of what the MFD says but always seems to be 1mph lower.  Even something
                    // like 41.2 on the MFD is often 40 in this equation.  I'm guessing the MFD uses floating point values
                    // for speed and rpm while we use integer values where presumably x.01 through x.99 are all rounded down
                    // to x.  So I'm testing if we're more accurate by adding 0.5 to speed and rpm.
                    mpg = 378.541 * ((float)kph + 0.5) * (float)0.621371 * (float)3600 / ((float)rpm + 0.5) / (float)lps;
                }
            }

            if (debug == 3)
            {
                sLCD.setCursor(12, 0);
                sprintf(buffer, "%3dK", (int)kph);
                sLCD.print(buffer);
            }
            //msLastLPS = millis();
        }

        if (mpg > -1)
        {
            if (mpg > 99)
            {
                mpg = 99;
            }

            sLCD.setCursor(11, 1);
            sprintf(buffer, "%2dm ", (int)mpg);
            sLCD.print(buffer);
        }
    }

    // current / battery
    if (data[4] != 9999) // read the battery
    {
        batteryAmps = data[4];
        if (millis() - msLastAmps >= 500)
        {
            sLCD.setCursor(0, 1);
            sprintf(buffer, "%+3dA ", (int)batteryAmps);
            sLCD.print(buffer);
            msLastAmps = millis();
        }
    }

    // accelerator
    if (data[5] != 9999)
        throttlePedalRaw = data[5];

    // brake
    if (data[6] != 9999)
        brakePedal = data[6];

    // Drive mode
    if (data[8] != 9999)
        gearShift = data[8];

    // SOC
    if (data[9] != 9999)
        stateOfCharge = data[9];

    // Construct the HSI bar
    // Instead of watching so much for the state of accel and brake, try to base the bar off system info.
    // When acel/brake are 0 and speed is 0, save a baseAmps var (usually +1 with all off, +4 or +8 at various levels of AC,
    // or can I query the system for that directly?).  This will not work at speed, I don't think.
    // Left of bar fills from -100 to base amps, electric from base to... base+30?
    // How to measure ICE side?

    if (debug == 2)
    {
        // Show raw values instead of bar
        sLCD.setCursor(0, 0);
        sprintf(buffer, "%3d%3d%3d%3d%3d", (int)gearShift, (int)throttlePedalRaw, (int)brakePedal, (int)kph, (int)stateOfCharge);
        sLCD.print(buffer);
    }

    if ((((gearShift & 0x7) >= GS_DRIVE) || ((gearShift & 0x7) == GS_REVERSE)))
    {
        // (gearShift & 0x7) values: Park = GS_PARK  Reverse = GS_REVERSE  Neutral = GS_NEUTRAL  A or Drive = GS_DRIVE  B or Engine brake = GS_EBRAKE
        // No no feet on the pedals, brake and acceleration 0 0
        if ((throttlePedalRaw == 0) && (brakePedal == 0))
        {
            // changes D and B
            if ((gearShift & 0x7) != GS_REVERSE)
            {
                if (kph < 9) // low speeds, the car accelerates slightly
                {
                    E = 1;
                    P = 0;
                    R = 0;
                }

                if (kph >= 9 && kph < 12) // speed between 9 and 11 km / h: not accelerate or regenerates
                {
                    E = 0;
                    P = 0;
                    R = 0;
                }

                if ((kph >= 12) && ((gearShift & 0x7) == GS_DRIVE)) // D change and greater than 12km / h speed: regenerates a bit
                {
                    E = 0;
                    P = 0;
                    R = 1;
                }

                if ((kph >= 12) && ((gearShift & 0x7) == GS_EBRAKE)) // change B and greater than 12km / h speed: regenerates some more
                {
                    E = 0;
                    P = 0;
                    R = 2;
                }
            }

            // R change at any speed accelerates 2 dashes
            if ((gearShift & 0x7) == GS_REVERSE)
            {
                E = 2;
                P = 0;
                R = 0;
            }
        }

        // Foot brake only - independent of change
        if ((throttlePedalRaw == 0) && (brakePedal > 0))
        {

            if (kph < 5) // reverse or up to 5 km / h not regenerate, either D or B
            {
                P = 0;

                if (brakePedal >= 14) // with the strong foot on the brake does not accelerate or regenerates
                {
                    E = 0;
                    R = 0;
                }

                if (brakePedal < 14) // with the foot slightly on the brake bar accelerates 1
                {
                    E = 1;
                    R = 0;
                }
            }

            if (kph >= 5 && kph <= 16) // speed between 5 and 16 km / h, little difference between D and B
            {
                E = 0;
                P = 0;
                if (brakePedal > 16)
                    R = (kph - 5) * 16 / 11; // if greater than 16 BRAKE fills as the speed from 5
                if (brakePedal <= 16)
                    R = (kph - 5) * brakePedal / 11; // if BRAKE lower fills in force as brake and speed
            }

            if (kph >= 16) // exceeding 16km / hslower
            {
                E = 0;
                P = 0;
                if (brakePedal > 16)
                    R = 16; // if greater than 16 BRAKE fills the entire bar
                if (brakePedal <= 16)
                    R = brakePedal; // if BRAKE  fills as the force on the brake from scratch
            }
        }

        // Foot throttle only
        // change R: starts at 2 and ends at 63 with 50% throttle
        // point A, E = 33 is where the powermodeends
        // point B, E = 63 to 2200rpm
        // point / C, full pwr mode

        if ((throttlePedalRaw > 0) && (brakePedal == 0))
        {
            R = 0;

            // calculate the point A
            if (kph < 50)
            {
                if (stateOfCharge < 45)
                    pointA = 5;
                if (stateOfCharge >= 45 && stateOfCharge <= 56)
                    pointA = 5 + ((stateOfCharge - 45) * 15 / 11);
                if (stateOfCharge >= 57 && stateOfCharge < 67)
                    pointA = 20;
                if (stateOfCharge >= 67 && stateOfCharge < 77)
                    pointA = 20 + (stateOfCharge - 67);
                if (stateOfCharge >= 77)
                    pointA = 30;
            }

            if (kph >= 50 && kph < 63)
            {
                if (stateOfCharge < 45)
                    pointA = 5;
                if (stateOfCharge >= 45 && stateOfCharge <= 56)
                    pointA = 5 + ((stateOfCharge - 45) * (136 - (kph - 50) * 75 / 10) / 100);
                if (stateOfCharge >= 57 && stateOfCharge < 67)
                    pointA = 5 + ((11) * (136 - (kph - 50) * 75 / 10) / 100);
                if (stateOfCharge >= 67 && stateOfCharge < 77)
                    pointA = (19 + (stateOfCharge - 66)) * (26 - (kph - 50)) / 26;
                if (stateOfCharge >= 77)
                    pointA = 15 + ((11) * (136 - (kph - 50) * 105 / 10) / 100);
            }

            if (kph >= 63)
            {
                if (stateOfCharge < 45)
                    pointA = 5;
                if (stateOfCharge >= 45 && stateOfCharge <= 56)
                    pointA = 5 + ((stateOfCharge - 45) * 5 / 11);
                if (stateOfCharge >= 57 && stateOfCharge < 67)
                    pointA = 10;
                if (stateOfCharge >= 67 && stateOfCharge < 77)
                    pointA = 10 + ((stateOfCharge - 67) / 2);
                if (stateOfCharge >= 77)
                    pointA = 15;
            }

            // calculation of point B
            pointB = 50 - ((211 * kph) / 1000);

            // C point of origin is 65 (PWR full)

            // fill in R mode or EVmode
            if ((gearShift & 0x7) == GS_REVERSE || EVMode == 64)
            {
                pointA = 25;
                pointB = 40;
            }

            // fill the bar

            throttlePedal = throttlePedalRaw / 2;

            if (throttlePedal <= pointA)
            {
                P = 0;
                E = (throttlePedalRaw * 33 / pointA / 2);
            }

            if (throttlePedal > pointA && throttlePedal < pointB)
            {
                P = 0;
                E = ((throttlePedalRaw - (pointA * 2)) * 15) / (pointB - pointA) + 33;
            }

            if (throttlePedal >= pointB && throttlePedal < pointC)
            {
                E = 63;
                P = ((throttlePedalRaw - (pointB * 2)) * 5) / (pointC - pointB);
            }

            if (throttlePedal >= pointC)
            {
                E = 63;
                P = 10;
            }

            // adds 1 to lower speeds than 9
            if (kph < 9 && E < 5)
                E = E + 1;
        }

        // Feet on the accelerator and the brake at the same time
        if ((throttlePedalRaw > 0) && (brakePedal > 0))
        {
            E = 0;
            R = 0;
            P = 10;
        }
    }
    else // other cases showing emptybar
    {
        E = 0;
        P = 0;
        R = 0;
    }
    // if there are changes from the previous situation draws the various parts of the bar

    if (debug < 2)
    {
        // debug 0 is normal HSI bar, 1 makes barra functions print numeric values.  Higher debug #s replace the
        // HSI bar with other info.
        if (R != RA)
            drawbar_R(R);
        if (E != EA)
            drawbar_E(E);
        if (P != PA)
            drawbar_P(P);
    }
}

// ------------------------------------------functions----------------------------------------------

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

/* Reading data from CAN */
void ecu_3(int *data)
{
    timeout = 0;
    for (count = 0; count <= 11; count++)
    {
        data[count] = 9999;
    }

    message_ok0 = false;
    message_ok1 = false;
    message_ok2 = false;
    message_ok3 = false;
    message_ok4 = false;
    message_ok5 = false;
    message_ok6 = false;
    message_ok7 = false;
    message_ok8 = false;
    message_ok9 = false;
    message_ok10 = false;
    message_ok11 = false;

    //MCP_CAN sendMessage;

    // Request fuel injector timing from the ECU.
    // The notes below are mostly quotes from http://techno-fandom.org/~hobbit/cars/can-split.html
    // Passive-CAN flavors like 07 E7 or 07 E8 depending on what I want bit 3 to be, or possibly
    // 07 D0 for seeing the 7DF generic ID traffic
    // 07 DF 02 01 00 as sort of an "are you there" query while trying to establish a connection to the car.
    // It isn't really a "connection" per se like stateful TCP, it's just sort of a hello and an answer.
    // Breakdown: CAN ID 7DF, the "any" address ; OBD-II length of 02 ; mode 01 ; PID 00 meaning
    // "give me the mask for the first 32 supported PIDs". We don't see the CAN length, since that's always
    // assumed to be 8. [That's held in four bits of the CAN header which is, confusingly enough, also called a DLC!]
    // The car answers with the ID of whatever ECU replies, but generally 7E8 for engine/powertrain and 7EA for the
    // hybrid ECU. One or more ECUs may answer this query at once, too! Remember that all this 7DF/7Ex stuff is
    // completely arbitrary, defined by the SAE, but does have the quality that CAN IDs that high are going to take a
    // LOW priority on the bus and not interfere with the high-rate non-OBD-II running chatter.
    // I think this code corresponds to the above:
    // sendMessage.id = 0x7DF; // CAN address ID
    // sendMessage.header.rtr = 0; // ?
    //sendMessage.header.length = 8; // Always 8
    //sendMessage.data[0] = 0x02; // OBDII length
    //sendMessage.data[1] = 0x01; // Mode 1
    //sendMessage.data[2] = 0x05; // PID
    //sendMessage.data[3] = 0x00;
    //sendMessage.data[4] = 0x00;
    //sendMessage.data[5] = 0x00;
    //sendMessage.data[6] = 0x00;
    //sendMessage.data[7] = 0x00;

    // Enhanced-mode queries to the Prius definitely use 07 Ex 02 21 yy where x is specific to an ECU and yy
    // is whatever PID is being requested. Always a one-byte PID
    // How to get fuel injector open-time in milliseconds from the Prius, which I'll post more about the benefits of
    // in a separate thread so people can read it without wading through all the protocol discussion. Basically, we
    // want to send 07 E0 02 21 F3.  Notes continue where we process the reply.
    /*unsigned long messageID = 0x07E0; // CAN address ID
    byte messageExt = 0;
    byte messageLength = 8;
    byte messageData[8];
    messageData[0] = 0x02; // OBDII length
    messageData[1] = 0x21; // Mode 1
    messageData[2] = 0xF3; // PID
    messageData[3] = 0x00;
    messageData[4] = 0x00;
    messageData[5] = 0x00;
    messageData[6] = 0x00;
    messageData[7] = 0x00;

    int response = CAN.sendMsgBuf(messageID, messageExt, messageLength, messageData);

    //mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);

    // Don't bother requesting ignition timing when the ICE isn't running (rpm 970 or less).
    if(rpm >= 970) 
    {
        if (response) 
        {
            //   SET(LED2_HIGH);
            //return 1;
        }
        else 
        {
            //   PRINT("Fehler: konnte die Nachricht nicht auslesen\n\n");
        }
    }*/

    //mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);

    while (timeout < 8000)
    {
        timeout++;
        if (CAN.checkReceive())
        {
            // See http://www.vassfamily.net/ToyotaPrius/CAN/PriusCodes.xls‎ which describes the message.id values
            // (ID column of xls file) that we receive on the CAN bus without requesting them in particular (thus
            // why this code doesn't call mcp2515_send_message to request a particular PID).
            // See also https://github.com/ilduganov/priidash/blob/master/source/CANmsg.cpp for how priidash uses
            // various values.
            byte recLenght;
            byte recData[8];
            if (CAN.readMsgBuf(&recLenght, recData))
            {
                // Fuel injector timing
                // We'll get back 07 E8 06 61 F3 xx yy zz qq -- see why? Six bytes of return data
                // from the powertrain ECU, mode/pid 61 F3 to confirm what the answer is for, and then four bytes to dig the
                // figure out of.
                // In terms of tCAN, message.data[0] = 06, data[1] = 61, data[2] = F3, data[3/4] = xxyy, data[5]=zz, data[6]=qq
                // The same PID seems to be used for "injector milliliters" as well as time, and it took a bit of
                // guesswork to determine that I want "zz" at RXD 3808. Next was how to scale it, and by comparing against what
                // the scantool says it seems to be given in eighths of a millisecond. I can only theorize from the grouping of
                // parameters in the scantool and eyeballing BUS for a while that the milliliters is a 16-bit quantity from
                // "xx yy", and "qq" is something about fuel trim which seems to sit near 7F or 80 which would be consistent
                // with some other fuel-trim figures we've discussed here already.
                // If I understand the above correctly, it's only zz we care about which is injector timing in 1/8 of a ms.
                // It also seems logical that xxyy would increase with ms (the longer fuel is injected, the more milliliters
                // are injected) but I'm just guessing.
                // 1280rpm: xxyy=17b5 zz=2b = 43/8 = 5.375ms
                // 1312rpm:      0908    13 = 19/8 = 2.375ms  I think this is when I pressed the pedal so it's odd the value went down
                // 1312rpm:      1a29    31 = 49/8 = 6.125ms
                if ((CAN.getCanId() == 0x07E8) && (recData[0] == 0x06) && (recData[1] == 0x61) && (recData[2] == 0xF3) && (!message_ok11))
                {
                    if (debug == 4)
                    {
                        sLCD.setCursor(0, 0);
                        sprintf(buffer, "%02x%02x%02x%02x%02x%02x%02x ", (int)recData[0], (int)recData[1],
                                (int)recData[2], (int)recData[3], (int)recData[4], (int)recData[5],
                                (int)recData[6]);
                        sLCD.print(buffer);
                    }
                    data[11] = recData[5];
                    message_ok11 = true;
                }

                // at position 0 EVmodeswitch
                // temperature 1
                // 2-position dimmer
                // 3 RPM in position
                // in position 4 battery current
                // 5 in the bottom positionaccelerator
                // brake in position 6
                // 7-positionspeed
                // change in position 8
                // at position 9 SOC
                if ((CAN.getCanId() == 0x52C && !message_ok1)) // Check the ID of temperature
                {
                    data[1] = (recData[1]) / 2; // convert to degrees C
                    message_ok1 = true;
                }

                if ((CAN.getCanId() == 0x529 && !message_ok0)) // Check the ID of EV mode
                {
                    data[0] = recData[4];
                    message_ok0 = true;
                }

                if ((CAN.getCanId() == 0x57F && !message_ok2)) // Check ID brightness
                {
                    data[2] = recData[2];
                    message_ok2 = true;
                }

                if ((CAN.getCanId() == 0x3C8 && !message_ok3)) // Check the ID of RPM
                {
                    value2 = ((recData[2] * 256) + recData[3]) / 8; // convert to RPM
                    data[3] = value2;
                    //rpm = value2;
                    message_ok3 = true;
                }

                if ((CAN.getCanId() == 0x3B && !message_ok4)) // Check the ID of the current from the battery
                {
                    value = ((recData[0]) * 256) + (recData[1]); // put the two bytes
                    if ((value & 0x800) != 0)
                    {
                        value = value - 0x1000;
                    }                     // convert 12-bit 16-bit
                    data[4] = value / 10; // convert to amps
                    message_ok4 = true;
                }

                if ((CAN.getCanId() == 0x244 && !message_ok5)) // Check the ID of the accelerator pedal position
                {
                    data[5] = recData[6]; // 0 to 200
                    message_ok5 = true;
                }

                if ((CAN.getCanId() == 0x30 && !message_ok6)) // Check the ID of the brake pedal position
                {
                    floatValue = ((recData[4] * 100) / 127); // turn in%
                    data[6] = floatValue;
                    message_ok6 = true;
                }

                if ((CAN.getCanId() == 0xB4 && !message_ok7)) // Check the ID of the speed
                {
                    value2 = (recData[5] << 8) | (recData[6]);
                    value2 = ((value2 * 10) / 1024);
                    data[7] = value2;
                    //kph = value2;
                    message_ok7 = true;
                }

                // Check the ID of Drive Mode (gear shift position in data[5], cruise control on/off data[4], powered or standby data[6])
                // According to the XLS file we could also use id 0x540.
                if ((CAN.getCanId() == 0x120 && !message_ok8))
                {
                    data[8] = (recData[5]);
                    message_ok8 = true;
                }

                if ((CAN.getCanId() == 0x3CB && !message_ok9)) // Check the ID of the State Of Charge
                {
                    value2 = (recData[2] << 8) | (recData[3]);
                    value2 = (value2 / 2);
                    data[9] = value2;
                    message_ok9 = true;
                }

                if ((CAN.getCanId() == 0x520 && !message_ok10)) // Check the ID of the MPG (we'll return liters per time unit)
                {
                    // Priidash calls this lps as if to mean liters per second, but it gets values of 250+ on mild acceleration
                    // and 900+ on a mild hill so I don't know what the units actually are.  pridash source uses data[0] and data[1]
                    // for this value, but I verified data[0] is always a4 as indicated in the XLS file and the actual value is
                    // at data[1] and data[2].  I assume priidash discards what we see as data[0] except that sometimes contains
                    // a useful value so I don't get it.  I tried a calc using a400 = 41984 as lps to see if it would produce
                    // a reasonable value at 1000 rpm and 15kph but you get 0.48mpg.  Using a reasonable 800 value for lps gets
                    // you 25mpg using priidash's formula.
                    int lps = recData[1] * 256 + recData[2];

                    if (debug == 3)
                    {
                        sLCD.setCursor(0, 0);
                        sprintf(buffer, "%3dL %02x%02x%02x ", (int)lps, (int)recData[0], (int)recData[1], (int)recData[2]);
                        sLCD.print(buffer);
                    }

                    data[10] = lps;
                    message_ok10 = true;
                }
            }
        }
        if (message_ok1 && message_ok0 && message_ok2 && message_ok3 && message_ok4 && message_ok5 && message_ok6 && message_ok7 && message_ok8 && message_ok9)
            return;
    }
    return;
}

// function to start the 2515 chip
char inic(unsigned char speed)
{
    return CAN.begin(speed);
}

/*==========================Internet of Threats=================================
 * Program:		Garage Hack Simulation
 * Authors: 	Ryan Peterson
 * Date: 		9/22/19
 * Description:  attack simulator for garage door opener
 *               includes methods such as brute force, debruijn, random,
 *               linear feedback shift register, and binary search
=============================================================================*/
#ifndef _SUPPRESS_PLIB_WARNING  //turn off MPLAB PLIB warnings
#define _SUPPRESS_PLIB_WARNING
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <xc.h>         //include Microchip XC compiler header
#include <plib.h>       //include Microchip peripheral libraries for timers, adc, etc...
#include "config.h"     //include Digilent Basys MX3 configuration file
#include "utils.h"
#include "lcd.h"        //include Digilent LCD (liquid crsytal display) library
#include "led.h"        //include Digilent LED (light emmiting diode) library
#include "swt.h"        //include Digilent SWT (switch) library
#include "ssd.h"        //include Digilent SSD (seven segment display) library
#include "spiflash.h"   //include Digilent SPIFlash library
#include "btn.h"        //include Digilent BTN (button) library
#include "lfsr.h"
#include "string.h"

/* -------------------- Configuration Bits --------------------------------- */
#pragma config JTAGEN = OFF             //turn off JTAG
#pragma config FWDTEN = OFF             //turn off watchdog timer
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
/* ------------- Forward Declarations -------------------------------------- */
void delay_ms(int ms);
void update_SSD(int value, int mode);
/* -------------- Global Definitions/Variables ----------------------------- */
#define SYS_FREQ        (80000000L)
#define INT_SEC         1
#define CORE_TICK_RATE  (SYS_FREQ/2/INT_SEC)
#define PB_FREQ         40000000

enum clock {       //possible states of clock
    countUp, reset, stop
};
int clock= reset;
enum attack {       //possible states of project
    bruteForce, deBruijn, LFSR, random, binary
};
int attack= bruteForce; //default to brute force attack mode
int code; //correct code for the garage
int bits=8; //number of bits for code, originally set to 8
int counter=0; //bits sent to open 
int timeCounter=0;
char topMsg[80], botMsg[80]; //LCD messages
char *mode; // Set or attack, used in LCD
int hackCode = 0b00000000; //attempt at hacking the garage
int lfsrVal; //value returned from lfsr calls
int indexCounter=0; //index for where we are in deBruijn sequence
int correct = 0; //used in for loop when we get the correct code
int mask; //a mask used to get n bits of a number
int upper, lower; //used in binary search
int nextBit; //holds to value of the next bit in the debruijn sequence
char* charset = "01"; //debruijn charset is 0 and 1
int i; //used in for loops to set every value in a arrays to 0
int a6[12];   // 'a' arrays are uses in debruijn sequence
int a7[14];
int a8[16];
int a9[18];
int a10[20];
int a11[22];
int a12[24];
int a13[26];
int a14[28];
int a15[30];
int a16[32];
int dbFlag = 0;
char sequence[65551]; //the debruijn sequence, fixed sized array, 65551 is size of 16bit sequence
char* s = sequence; // a pointer to the sequence

int main(int argc, char** argv) {
    LCD_Init();      //initiate items being used
    delay_ms(100);
    LED_Init();
    delay_ms(100);
    BTN_Init();
    delay_ms(100);
    SSD_Init();
    delay_ms(100);
    SWT_Init();
    delay_ms(100);
    SPIFLASH_Init();
    delay_ms(100);
    mode="SET";
    for(i=0; i<12; i++) {
        a6[i]= 0;
    }
    for(i=0; i<14; i++) {
        a7[i]= 0;
    }
    for(i=0; i<16; i++) {
        a8[i]= 0;
    }
    for(i=0; i<18; i++) {
        a9[i]= 0;
    }
    for(i=0; i<20; i++) {
        a10[i]= 0;
    }
    for(i=0; i<22; i++) {
        a11[i]= 0;
    }
    for(i=0; i<24; i++) {
        a12[i]= 0;
    }
    for(i=0; i<26; i++) {
        a13[i]= 0;
    }
    for(i=0; i<28; i++) {
        a14[i]= 0;
    }
    for(i=0; i<30; i++) {
        a15[i]= 0;
    }
    for(i=0; i<32; i++) {
        a16[i]= 0;
    }
    sequence[0] = '\0';
    
    OpenCoreTimer(CORE_TICK_RATE);
    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_5 | CT_INT_SUB_PRIOR_0));
    INTEnableSystemMultiVectoredInt();
    while(1) {
        sprintf(topMsg, "%s   Bits:%d    ",mode, bits); //display mode
        LCD_WriteStringAtPos(topMsg, 0, 0);
        sprintf(botMsg, "%d  |  %d    ", code, counter); //display correct code and bits
        LCD_WriteStringAtPos(botMsg, 1, 0);
        update_SSD(timeCounter, 0);  //SSD will show the amount of tries it takes to open garage
        if(BTN_GetValue('C')) { //change mode if center button is pressed
            delay_ms(1000);
            if(mode=="SET") {
                mode="ATTACK";
                counter = 0;
                if(attack == deBruijn) {
                    LCD_WriteStringAtPos("Generating Sequence", 0, 0);
                    if(!dbFlag) {
                        if(bits == 6) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a6, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 7) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a7, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 8) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a8, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 9) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a9, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 10) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a10, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 11) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a11, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 12) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a12, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 13) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a13, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 14) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a14, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 15) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a15, sequence);
                            dbFlag = 1;
                        }
                        else if(bits == 16) {
                            s = (char *) de_bruijn(charset, bits, ((int)(pow(2,bits))+bits-1), a16, sequence);
                            dbFlag = 1;
                        }
                        counter = bits;
                    }
                }
            }
            else
                mode="SET";
        }
        if(mode=="SET") {
            if (BTN_GetValue('U')) {
                delay_ms(1000);
                dbFlag = 0;
                bits++;
                if(bits == 17) {
                    bits = 6;
                }
                code = 0;
            }
            if (BTN_GetValue('D')) {
                delay_ms(1000);
                dbFlag = 0;
                bits--;
                if(bits == 5) {
                    bits = 16;
                }
                code = 0;
            }
            if (BTN_GetValue('R')) {
                delay_ms(1000);
                code++;
                if(code == pow(2,bits) ) {
                    code = 0;
                }
            }
            if (BTN_GetValue('L')) {
                delay_ms(1000);
                code--;
                if(code == -1) {
                    code = (pow(2,bits)) -1;
                }
            }
            mask = pow(2, bits)-1;
            hackCode = 0b00000000;
            if(SWT_GetValue(0)) {
                attack = LFSR;
            } else if(SWT_GetValue(1)) {
                attack = deBruijn;
                indexCounter = bits;
            } else if(SWT_GetValue(2)) {
                attack = random;
            } else if(SWT_GetValue(3)) {
                attack = binary;
                upper = mask;
                lower = 0;
            } else {
                attack = bruteForce;
            }
            
            
        }
        if(mode=="Opened") {        //when we open garage
            update_SSD(timeCounter, 0);        //show counter on SSD
            for(correct=0; correct<10; correct++) {        //flash LEDs with correct code
                LED_SetGroupValue(hackCode);
                delay_ms(500);
                LED_SetGroupValue(0b00000000);
                delay_ms(500);
            }
            correct = 0;
            counter =0;
            hackCode = 0;
            code = rand() % (int)(pow(2,bits));
            indexCounter = 0; 
            clock = reset;
            mode = "SET";           //move back into SET mode
        }
        while(mode == "ATTACK"){   //we can start sending bits (guesses)
            clock = countUp;
            sprintf(topMsg, "%s   Bits:%d    ",mode, bits); //display mode
            LCD_WriteStringAtPos(topMsg, 0, 0);
            sprintf(botMsg, "%d  |  %d    ", code, counter); //display correct code and bits
            LCD_WriteStringAtPos(botMsg, 1, 0);
            LED_SetGroupValue(hackCode);        //Set LEDs to code we guess
            update_SSD(timeCounter, 0);                //update SSD with number of tries
            if(hackCode == code) {
                clock = stop;
                mode = "Opened";
            } 
            else { //if we have not gotten the right code, perform the attack
                if(attack == deBruijn) { //attack designed with deBruijn sequence
                    //READ from pBufOut to get next digit in sequence
                    nextBit = sequence[indexCounter] - '0';
                    hackCode = ((hackCode<<1) | (nextBit & 0b1 )) & mask;
                    counter++;
                    indexCounter++;
                }
                else if(attack == LFSR) { //doesn't cover all possible codes yet
                    if(bits == 8) {
                        lfsrVal= lfsr8Bit(code);
                    } else if (bits == 9) {
                        lfsrVal= lfsr9Bit(code);
                    } else if (bits == 10) {
                        lfsrVal= lfsr10Bit(code);
                    } else if (bits == 11) {
                        lfsrVal= lfsr11Bit(code);
                    } else if (bits == 12) {
                        lfsrVal= lfsr12Bit(code);
                    }else if (bits == 13) {
                        lfsrVal= lfsr13Bit(code);
                    } else if (bits == 14) {
                        lfsrVal= lfsr14Bit(code);
                    } else if (bits == 15) {
                        lfsrVal= lfsr15Bit(code);
                    } else if (bits == 16) {
                        lfsrVal= lfsr16Bit(code);
                    } else {
                        attack = bruteForce;
                    }
                    counter = lfsrVal;
                    hackCode = code;
                    mode = "Opened";
                }
                else if(attack == random) { //pseudo random attack
                    hackCode = rand() % (int)(pow(2,bits));
                    counter += bits;
                }
                else if(attack == binary) {
                    hackCode = (int)((upper+lower)/2);
                    if(code-hackCode<0) {
                        upper = hackCode;
                    }
                    if(code-hackCode>0){
                        lower = hackCode;
                    }
                    counter+=bits;
                }
                else { //brute force attack, try every possible code
                    hackCode++;
                    counter+= bits; //add n new bits each time
                }
            }
            delay_ms(100); //software delay to slow down time in between attempts
        }
    }
    return (EXIT_SUCCESS);
}
void __ISR(_CORE_TIMER_VECTOR, IPL5SOFT) CoreTimerHandler(void) {
    mCTClearIntFlag(); // clear the interrupt flag
    switch (clock) {
        case countUp:
            timeCounter++;
            break;
        case reset:
            timeCounter=0;
            break;
        case stop:
            
            break;
    }
    UpdateCoreTimer(CORE_TICK_RATE);
}

    /* delay_ms
     * A software delay to be used in program 
     * 
     * @param ms   integer value delayed by 
     * 
     */
    void delay_ms(int ms) {
        int i, counter;
        for (counter = 0; counter < ms; counter++) {
            for (i = 0; i < 1426; i++) {
            } //software delay ~1 millisec 
        }
    }
    
    /* update_SSD
     * Takes in an integer to display on SSD
     * 
     * @param value   integer value to be displayed on SSD
     * @param mode    interger that determines what to display on board (1 for decimal, 0 for hex)
     */
    void update_SSD(int value, int mode) {
        char SSD1 =0b0000000;
        char SSD2 =0b0000000;
        char SSD3 =0b0000000;
        char SSD4 =0b0000000;
        
        if(mode) {
            // Code for Decimal SSD
            int thou, hunds, tens, ones;
            thou=floor((value/1000)%10);
            if(thou>0) {
                SSD4=thou;
            }
            else
                SSD4=18;

            hunds=floor((value/100)%10);
            if(thou==0 && hunds==0) {
                //SSD4=18;
                SSD3=18;
            }
            else
                SSD3=hunds;

            tens=floor((value/10)%10);
            if(thou==0 && hunds==0 && tens==0) {
                //SSD4=18;
                //SSD3=18;
                SSD2=18;
            }
            else
                SSD2=tens;

            ones = value%10;

            SSD1 = ones;
        } 
        else {
            //Code for hex SSD
            int digit;
            char SSD;
            int loop = 3;
            for(loop = 3; loop>=0; loop--) {
                digit = floor((value/(int)(pow(16,loop)))%16);
                SSD = digit;
                if(loop == 0) { SSD1 = SSD; }
                else if(loop == 1) { SSD2 = SSD;}
                else if(loop == 2) { SSD3 = SSD;}
                else { SSD4 = SSD;}
            }
            if( SSD4 == 0 ) { 
                SSD4 = 18;
                if( SSD3 == 0 ) {
                    SSD3 = 18;
                    if( SSD2==0 ) {
                        SSD2 = 18;
                    }
                }
            }
        }
        SSD_WriteDigits(SSD1, SSD2, SSD3, SSD4, 0, 0, 0, 0);
    }


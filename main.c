#include <msp430.h> 
#include <stdint.h>
#include "Capacitive_Touch/cap_touch.h"

#define LED1    BIT7        //yellow led 1, port2
#define LED2    BIT6        //yellow led 2, port2
#define LED3    BIT3        //blue led, port1
#define LED4    BIT5        //red led, port1
#define LED5    BIT4        //green led, port1
#define LED_ACTIVE_TIME     10000

#define delay_time                   10000
#define NUMBER_OF_MODES              4
#define MCLK_FREQ_MHZ 8                     // MCLK = 8MHz

//long int timer_count;
long int IC_val1, IC_val2;
int mode = 0, periode;
int touch_array[4];

void configClock_16MHz(void);
void Software_Trim();                       // Software Trim to get the best DCOFTRIM value
void config_clock_8MHz(void);




/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	    // stop watchdog timer
	configClock_16MHz();
//    cap_touch_enable(2, 1);         // enable cap touch at P2.1

	P1SEL0 &= ~(LED3 + LED4 + LED5);
	P1SEL1 &= ~(LED3 + LED4 + LED5);
	P2SEL0 &= ~(LED1 + LED2);
	P2SEL1 &= ~(LED1 + LED2);

	P1DIR |= (LED3 + LED4 + LED5);
	P2DIR |= (LED1 + LED2);
//    P2REN |= CapT1 + CapT2;             //enable pull resistor

//	P1SEL0 &= ~BIT0;
    P1SEL1 |= BIT0;                                 // Set as SMCLK pin, second function
    P1DIR |= BIT0;

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	P1OUT |= LED3 + LED4 + LED5;
    P2OUT &= ~(LED1 + LED2);
    __delay_cycles(16000000);
    P2OUT |= (LED1 + LED2);

    TB0CTL |= TBSSEL__SMCLK | MC__CONTINUOUS;     //start timerB0, SMCLK, continuous mode

    while(1)
    {
        reccord_touch_value(touch_array);
        if(C1_discharge_time() >= C1_ACTIVE_VALUE)
        {
            P2OUT |= (LED1 + LED2);
            P1OUT &= ~(LED3 + LED4 + LED5);
            __delay_cycles(5000000);
        }
        if(C2_discharge_time() >= C2_ACTIVE_VALUE)
        {
            P1OUT |= (LED3 + LED4 + LED5);
            P2OUT &= ~(LED1 + LED2);
            __delay_cycles(5000000);
        }
        if(C3_discharge_time() >= C3_ACTIVE_VALUE)
        {
            mode++;
            __delay_cycles(5000000);
        }
        if(C4_discharge_time() > C4_ACTIVE_VALUE)
        {
            mode--;
            __delay_cycles(5000000);
        }
        if(mode > NUMBER_OF_MODES)  mode=0;
        if(mode < 0)    mode = NUMBER_OF_MODES;

        switch(mode)
        {
            case 0:
                P1OUT |= (LED3 + LED4 + LED5);
//                P2OUT |= (LED1 + LED2);
//                __delay_cycles(delay_time);
                P2OUT &= !(LED1 + LED2);
//                __delay_cycles(delay_time);
                break;
            case 1:
//                P2OUT |= LED1;
//                P2OUT &= ~LED2;
//                __delay_cycles(delay_time);
                P2OUT |= LED2;
                P2OUT &= ~LED1;
//                __delay_cycles(delay_time);
                break;
            case 2:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                P1OUT &= ~LED3;
                break;
            case 3:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                P1OUT &= ~LED4;
                break;
            case 4:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                P1OUT &= ~LED5;
                break;
//            case 5:
//                P1OUT |= (LED4 | LED5);
//                P1OUT &= ~LED3;
//                __delay_cycles(delay_time*2);
//                P1OUT |= (LED3 | LED5);
//                P1OUT &= ~LED4;
//                __delay_cycles(delay_time*2);
//                P1OUT |= (LED3 | LED4);
//                P1OUT &= ~LED5;
//                __delay_cycles(delay_time*2);
//                P1OUT |= (LED3 | LED4 |LED5);
//                break;
            default:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                break;
        }
    }

//	return 0;
}

void initTimer(void)
{
//    TB0CCR1 = count_value/2;
//    TB0CCTL1 |= CCIE;
    TB0CCR0 = LED_ACTIVE_TIME;                  //Count up to the value stored in TB0CCR0
    TB0CCTL0 |= CCIE;                           //CCR0 interrupt enable
    TB0CTL |= TBSSEL__ACLK | MC__UP | TBCLR;    // select the ACLK, UP mode, clear TBR to start counting
}

void configClock_16MHz(void)
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);                           // disable FLL
    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;        // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                       // default DCOCLKDIV as MCLK and SMCLK source
}

void config_clock_8MHz(void)
{
    __bis_SR_register(SCG0);                // disable FLL
    CSCTL3 |= SELREF__REFOCLK;              // Set REFO as FLL reference source
    CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
    CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                // enable FLL
    Software_Trim();                        // Software Trim to get the best DCOFTRIM value
}

void Software_Trim()
{
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
}

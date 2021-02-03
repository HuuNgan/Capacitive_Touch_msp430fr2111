#include <msp430.h> 
#include <stdint.h>
#include "Capacitive_Touch/cap_touch.h"

#define LED1    BIT7        //yellow led 1, port2
#define LED2    BIT6        //yellow led 2, port2
#define LED3    BIT3        //blue led, port1
#define LED4    BIT5        //red led, port1
#define LED5    BIT4        //green led, port1

#define delay_time                   10000
#define NUMBER_OF_MODES              7

//long int timer_count;
long int IC_val1, IC_val2;
int mode = 0, periode;

void configClock(void);

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	    // stop watchdog timer
	configClock();
    cap_touch_enable(2, 1);         // enable cap touch at P2.1

	P1SEL0 &= ~(LED3 + LED4 + LED5);
	P1SEL1 &= ~(LED3 + LED4 + LED5);
	P2SEL0 &= ~(LED1 + LED2);
	P2SEL1 &= ~(LED1 + LED2);

	P1DIR |= (LED3 + LED4 + LED5 + CapT3 + CapT4);
	P2DIR |= (LED1 + LED2 + CapT1 + CapT2);
//    P2REN |= CapT1 + CapT2;             //enable pull resistor

	P1OUT |= (CapT3 + CapT4);
	P2OUT |= (CapT1 + CapT2);

//	P1SEL0 &= ~BIT0;
//    P1SEL1 |= BIT0;                                 // Set as SMCLK pin, second function
//    P1DIR |= BIT0;

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	P1OUT |= LED3 + LED4 + LED5;
    P2OUT &= ~(LED1 + LED2);
    __delay_cycles(16000000);
    P2OUT |= (LED1 + LED2);

    TB0CTL |= TBSSEL__SMCLK | MC__CONTINUOUS;     //start timerB0, SMCLK, continuous mode
    P2IES |=  CapT1 + CapT2;             // Hi/Low edge
    P2IE  &= ~ CapT2;
    P2IE  |=  CapT1;             // interrupt enabled
    P2DIR &= ~(CapT1 + CapT2);          // set CapT1 and CapT2 as input
//    __bis_SR_register(GIE);

    while(1)
    {
        if(C3_discharge_time() >= C3_ACTIVE_VALUE)
        {
            mode++;
            if(mode > NUMBER_OF_MODES)  mode = 0;
            __delay_cycles(3000000);
        }
        switch(mode)
        {
            case 0:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                __delay_cycles(delay_time);
                P2OUT &= !(LED1 + LED2);
                __delay_cycles(delay_time);
                break;
            case 1:
                P2OUT |= LED1;
                P2OUT &= ~LED2;
                __delay_cycles(delay_time);
                P2OUT |= LED2;
                P2OUT &= ~LED1;
                __delay_cycles(delay_time);
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
            case 5:
                P1OUT |= (LED4 | LED5);
                P1OUT &= ~LED3;
                __delay_cycles(delay_time*2);
                P1OUT |= (LED3 | LED5);
                P1OUT &= ~LED4;
                __delay_cycles(delay_time*2);
                P1OUT |= (LED3 | LED4);
                P1OUT &= ~LED5;
                __delay_cycles(delay_time*2);
                P1OUT |= (LED3 | LED4 |LED5);
                break;
            default:
                P1OUT |= (LED3 + LED4 + LED5);
                P2OUT |= (LED1 + LED2);
                break;
        }
    }

//	return 0;
}

void configClock(void)
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

//// Port 2 interrupt service routine
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
//#pragma vector=PORT2_VECTOR
//__interrupt void Port_2(void)
//#elif defined(__GNUC__)
//void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
//#else
//#error Compiler not supported!
//#endif
//{
//    if((CAPT1_PORT_IN & CapT1)==0)
//    {
//        IC_val2 = TB0R;
//        if(IC_val2 > IC_val1)    periode = IC_val2 - IC_val1;
//        else periode = 65536 + IC_val2 - IC_val1;
//        IC_val1 = IC_val2;
//
//        if(periode >= C1_ACTIVE_VALUE)
//        {
//            mode++;
//            if(mode >= NUMBER_OF_MODES) mode = 0;
//            __delay_cycles(3000000);
//        }
//
//        //switch to checking CapT2
//        cap_touch_disable(2, 1);
//        cap_touch_enable(2, 0);          //enable CapT2
//        P2IE  = 0;
//        P2IE  |=  CapT2;                // interrupt enabled
//
//        P2IFG &= ~CapT1;                        // Clear CapT1 IFG
//    }
//
//    else if((CAPT2_PORT_IN & CapT2)==0)
//    {
//        IC_val2 = TB0R;
//        if(IC_val2 > IC_val1)    periode = IC_val2 - IC_val1;
//        else periode = 65536 + IC_val2 - IC_val1;
//        IC_val1 = IC_val2;
//
//        if(periode >= C1_ACTIVE_VALUE)
//        {
//            mode++;
//            if(mode >= NUMBER_OF_MODES) mode = 0;
//            __delay_cycles(3000000);
//        }
//
//        //switch to checking CapT1
//        cap_touch_disable(2, 0);
//        cap_touch_enable(2, 1);          //enable CapT1
//        P2IE  = 0;
//        P2IE  |=  CapT1;                // interrupt enabled
//
//        P2IFG &= ~CapT2;                        // Clear CapT1 IFG
//    }
//}

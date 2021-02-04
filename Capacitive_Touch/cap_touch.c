/*
 * cap_touch.c
 *
 *  Created on: 3 thg 2, 2021
 *      Author: huungan
 */

#include <msp430.h>
#include <stdint.h>
#include "cap_touch.h"

long int IC_val1, IC_val2;
int discharge_time;

void reccord_touch_value(int *touch_array)
{
    touch_array[0] = C1_discharge_time();
    touch_array[1] = C2_discharge_time();
    touch_array[2] = C3_discharge_time();
    touch_array[3] = C4_discharge_time();
}

int C1_discharge_time()
{
    CAPT12_PORT_DIR |= (CapT1 + CapT2);
    CAPT34_PORT_DIR |= (CapT3 + CapT4);
    CAPT12_PORT_OUT &= ~CapT2;
    CAPT34_PORT_OUT &= ~(CapT3 + CapT4);
    CAPT1_PORT_OUT |= CapT1;
    __delay_cycles(CHARGE_TIME);
    CAPT1_PORT_DIR &= ~CapT1;
    CAPT1_PORT_REN |= CapT1;
    CAPT1_PORT_OUT &= ~CapT1;           //Pull-down
    IC_val1 = TB0R;
    while(CAPT1_PORT_IN & CapT1);
    IC_val2 = TB0R;
    if(IC_val2 > IC_val1)    discharge_time = IC_val2 - IC_val1;
    else discharge_time = 65536 + IC_val2 - IC_val1;

    return discharge_time;
}

int C2_discharge_time()
{
    CAPT12_PORT_DIR |= (CapT1 + CapT2);
    CAPT34_PORT_DIR |= (CapT3 + CapT4);
    CAPT12_PORT_OUT &= ~CapT1;
    CAPT34_PORT_OUT &= ~(CapT3 + CapT4);
    CAPT2_PORT_OUT |= CapT2;
    __delay_cycles(CHARGE_TIME);
    CAPT2_PORT_DIR &= ~CapT2;
    CAPT2_PORT_REN |= CapT2;
    CAPT2_PORT_OUT &= ~CapT2;           //Pull-down
    IC_val1 = TB0R;
    while(CAPT2_PORT_IN & CapT2);
    IC_val2 = TB0R;
    if(IC_val2 > IC_val1)    discharge_time = IC_val2 - IC_val1;
    else discharge_time = 65536 + IC_val2 - IC_val1;

    return discharge_time;
}

int C3_discharge_time()
{
    CAPT12_PORT_DIR |= (CapT1 + CapT2);
    CAPT34_PORT_DIR |= (CapT3 + CapT4);
    CAPT12_PORT_OUT &= ~(CapT1 + CapT2);
    CAPT34_PORT_OUT &= ~CapT4;
    CAPT3_PORT_OUT |= CapT3;
    __delay_cycles(CHARGE_TIME);
    CAPT3_PORT_DIR &= ~CapT3;
    CAPT3_PORT_REN |= CapT3;
    CAPT3_PORT_OUT &= ~CapT3;           //Pull-down
    IC_val1 = TB0R;
    while(CAPT3_PORT_IN & CapT3);
    IC_val2 = TB0R;
    if(IC_val2 > IC_val1)    discharge_time = IC_val2 - IC_val1;
    else discharge_time = 65536 + IC_val2 - IC_val1;

    return discharge_time;
}

int C4_discharge_time()
{
    CAPT12_PORT_DIR |= (CapT1 + CapT2);
    CAPT34_PORT_DIR |= (CapT3 + CapT4);
    CAPT12_PORT_OUT &= ~(CapT1 + CapT2);
    CAPT34_PORT_OUT &= ~CapT3;
    CAPT4_PORT_OUT |= CapT4;
    __delay_cycles(CHARGE_TIME);
    CAPT4_PORT_DIR &= ~CapT4;
    CAPT4_PORT_REN |= CapT4;
    CAPT4_PORT_OUT &= ~CapT4;           //Pull-down
    IC_val1 = TB0R;
    while(CAPT4_PORT_IN & CapT4);
    IC_val2 = TB0R;
    if(IC_val2 > IC_val1)    discharge_time = IC_val2 - IC_val1;
    else discharge_time = 65536 + IC_val2 - IC_val1;

    return discharge_time;
}

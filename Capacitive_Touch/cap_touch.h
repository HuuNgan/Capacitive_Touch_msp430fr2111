/*
 * cap_touch.h
 *
 *  Created on: 3 thg 2, 2021
 *      Author: huungan
 */

#ifndef CAPACITIVE_TOUCH_CAP_TOUCH_H_
#define CAPACITIVE_TOUCH_CAP_TOUCH_H_

// Input pins definition
#define CapT1   BIT1        //port2
#define CapT2   BIT0        //port2
#define CapT3   BIT7        //port1
#define CapT4   BIT6        //port1

// Values definition
#define C1_ACTIVE_VALUE         35
#define C2_ACTIVE_VALUE         28
#define C3_ACTIVE_VALUE         35
#define C4_ACTIVE_VALUE         30
#define CHARGE_TIME             800000

// Definition for capacitive touch I/O ports

#define CAPT12_PORT_OUT      P2OUT
#define CAPT12_PORT_DIR      P2DIR
#define CAPT12_PORT_SEL0     P2SEL0
#define CAPT12_PORT_SEL1     P2SEL1
#define CAPT12_PORT_IES      P2IES
#define CAPT12_PORT_IE       P2IE
#define CAPT12_PORT_REN      P2REN
#define CAPT12_PORT_IN       P2IN

#define CAPT34_PORT_OUT      P1OUT
#define CAPT34_PORT_DIR      P1DIR
#define CAPT34_PORT_SEL0     P1SEL0
#define CAPT34_PORT_SEL1     P1SEL1
#define CAPT34_PORT_IES      P1IES
#define CAPT34_PORT_IE       P1IE
#define CAPT34_PORT_REN      P1REN
#define CAPT34_PORT_IN       P1IN

#define CAPT1_PORT_OUT      P2OUT
#define CAPT1_PORT_DIR      P2DIR
#define CAPT1_PORT_SEL0     P2SEL0
#define CAPT1_PORT_SEL1     P2SEL1
#define CAPT1_PORT_IES      P2IES
#define CAPT1_PORT_IE       P2IE
#define CAPT1_PORT_REN      P2REN
#define CAPT1_PORT_IN       P2IN

#define CAPT2_PORT_OUT      P2OUT
#define CAPT2_PORT_DIR      P2DIR
#define CAPT2_PORT_SEL0     P2SEL0
#define CAPT2_PORT_SEL1     P2SEL1
#define CAPT2_PORT_IES      P2IES
#define CAPT2_PORT_IE       P2IE
#define CAPT2_PORT_REN      P2REN
#define CAPT2_PORT_IN       P2IN

#define CAPT3_PORT_OUT      P1OUT
#define CAPT3_PORT_DIR      P1DIR
#define CAPT3_PORT_SEL0     P1SEL0
#define CAPT3_PORT_SEL1     P1SEL1
#define CAPT3_PORT_IES      P1IES
#define CAPT3_PORT_IE       P1IE
#define CAPT3_PORT_REN      P1REN
#define CAPT3_PORT_IN       P1IN

#define CAPT4_PORT_OUT      P1OUT
#define CAPT4_PORT_DIR      P1DIR
#define CAPT4_PORT_SEL0     P1SEL0
#define CAPT4_PORT_SEL1     P1SEL1
#define CAPT4_PORT_IES      P1IES
#define CAPT4_PORT_IE       P1IE
#define CAPT4_PORT_REN      P1REN
#define CAPT4_PORT_IN       P1IN

// Cap touch configure definitions
#define cap_touch_enable(port, pin)     CAPTIOCTL|=(CAPTIOEN|CAPTIOPOSEL_##port|CAPTIOPISEL_##pin)
#define cap_touch_disable(port, pin)    CAPTIOCTL&=~(CAPTIOEN|CAPTIOPOSEL_##port|CAPTIOPISEL_##pin)
#define cap_touch_disable_all          CAPTIOCTL&=~0xFF

//Prototypes
int C1_discharge_time();
int C2_discharge_time();
int C3_discharge_time();
int C4_discharge_time();
void reccord_touch_value(int *touch_array);

#endif /* CAPACITIVE_TOUCH_CAP_TOUCH_H_ */

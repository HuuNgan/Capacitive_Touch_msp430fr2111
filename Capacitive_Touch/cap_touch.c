/*
 * cap_touch.c
 *
 *  Created on: 3 thg 2, 2021
 *      Author: huungan
 */

#include <msp430.h>
#include <stdint.h>
#include "cap_touch.h"

//void cap_touch_init(uint8_t port, uint8_t pin)
//{
//    CAPTIOCTL &= ~0xFF      // clear register
//    if(port == 1)
//    {
//        switch(pin)
//        {
//            case 0:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_0);
//                break;
//            case 1:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_1);
//                break;
//            case 2:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_2);
//                break;
//            case 3:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_3);
//                break;
//            case 4:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_4);
//                break;
//            case 5:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_5);
//                break;
//            case 6:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_6);
//                break;
//            case 7:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_1 | CAPTIOPISEL_7);
//                break;
//        }
//    }
//    else if(port == 2)
//    {
//        switch(pin)
//        {
//            case 0:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_0);
//                break;
//            case 1:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_1);
//                break;
//            case 2:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_2);
//                break;
//            case 3:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_3);
//                break;
//            case 4:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_4);
//                break;
//            case 5:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_5);
//                break;
//            case 6:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_6);
//                break;
//            case 7:
//                CAPTIOCTL |= (CAPTIOEN | CAPTIOPOSEL_2 | CAPTIOPISEL_7);
//                break;
//        }
//    }
//
//    else ;
//}


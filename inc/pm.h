/*
* pm.h - define macro of power manager.
*
* Author: Shan Jiejing
* Date: 2016-12-25
*/

#ifndef _PM_H_
#define _PM_H_

#include  <msp430x14x.h>

#define SUSPEND() _BIS_SR(LPM3_bits + GIE)
#define RESUME() _BIC_SR_IRQ(LPM3_bits)

#endif //_PM_H_
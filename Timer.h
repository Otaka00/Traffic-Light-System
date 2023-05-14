#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


void TIMER0_init(void);
void TIMER1_init(void);
void TIMER0_delay(uint32 delay);
void TIMER1_delay(uint32 delay);
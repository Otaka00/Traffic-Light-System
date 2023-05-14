#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

enum state {
  NSGREEN, // NorthSouth green LED on
  NSYELLOW, // NorthSouth yellow LED on
  EWGREEN, // EastWest green LED on
  EWYELLOW, // EastWest yellow LED on
  switchToNS, // both lights red for 1 second, to switch to NorthSouth
  switchToEW // both lights red for 1 second, to switch to EastWest
};

void PORTF_init(void);
void PORTB_init(void);
void PORTC_init(void);

void TrafficLEDs(bool NSR, bool NSY, bool NSG, bool EWR, bool EWY, bool EWG, bool PNSR, bool PNSG, bool PEWR, bool PEWG);
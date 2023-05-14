#include "DIO.h"

volatile enum state TrafficState = NSGREEN;

void main() {
  PORTF_init();
  PORTB_init();
  PORTC_init();
  TIMER0_init();
  TIMER1_init();
  __asm("CPSIE I");
  while(1) {
    switch(TrafficState) {
      case NSGREEN:
        TrafficLEDs(0, 0, 1, 1, 0, 0, 1, 0, 0, 1);
        TIMER0_delay(5000);
        TrafficState = NSYELLOW;
        break;

      case NSYELLOW:
        TrafficLEDs(0, 1, 0, 1, 0, 0, 1, 0, 0, 1);
       TIMER0_delay(2000);
        TrafficState = switchToEW;
        break;

      case EWGREEN:
        TrafficLEDs(1, 0, 0, 0, 0, 1, 0, 1, 1, 0);
       TIMER0_delay(5000);
        TrafficState = EWYELLOW;
        break;

      case EWYELLOW:
        TrafficLEDs(1, 0, 0, 0, 1, 0, 0, 1, 1, 0);
       TIMER0_delay(2000);
        TrafficState = switchToNS;
        break;

      case switchToNS:
        TrafficLEDs(1, 0, 0, 1, 0, 0, 1, 0, 1, 0);
        TIMER0_delay(1000);
        TrafficState = NSGREEN;
        break;

      case switchToEW:
        TrafficLEDs(1, 0, 0, 1, 0, 0, 1, 0, 1, 0);
       // TIMER0_delay(1000);
        TrafficState = EWGREEN;
        break;
    }
  }
}

void RestoreState(void) {
  switch(TrafficState) {
    case NSGREEN:
      TrafficLEDs(0, 0, 1, 1, 0, 0, 1, 0, 0, 1);
      break;

    case NSYELLOW:
      TrafficLEDs(0, 1, 0, 1, 0, 0, 1, 0, 0, 1);
      break;

    case EWGREEN:
      TrafficLEDs(1, 0, 0, 0, 0, 1, 0, 1, 1, 0);
      break;

    case EWYELLOW: 
      TrafficLEDs(1, 0, 0, 0, 1, 0, 0, 1, 1, 0);
      break;

    case switchToNS:
      TrafficLEDs(1, 0, 0, 1, 0, 0, 1, 0, 1, 0);
      break;

    case switchToEW:
      TrafficLEDs(1, 0, 0, 1, 0, 0, 1, 0, 1, 0);
      break;
  }
}
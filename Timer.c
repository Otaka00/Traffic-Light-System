#include "DIO.h"
#include "Timer.h"
void TIMER0_init(void) { // for traffic
  SYSCTL_RCGCTIMER_R |= 0x01; // enable clock
  TIMER0_CTL_R = 0x0; // disable timer to avoid unexpected behaviour
  TIMER0_CFG_R = 0x00; // 32-bit timer 
  TIMER0_TAMR_R = 0x01; // one shot
  TIMER0_TAPR_R = 0;
  TIMER0_ICR_R = 0x1; // clear timeout flag
  TIMER0_IMR_R = 0x01; // interrupts enable
  NVIC_PRI4_R = (NVIC_PRI4_R & 0x00FFFFFF) | 0x80000000; // set priority 4 to TIMER0
 NVIC_EN0_R = (1<<19); // enable IQR 19 in NVIC
}

void TIMER1_init(void) { // for pedestrians
  SYSCTL_RCGCTIMER_R |= (1<<1);
  TIMER1_CTL_R = 0x0;
  TIMER1_CFG_R = 0x00;
  TIMER1_TAMR_R = 0x01;
  TIMER1_ICR_R = 0x1;
  TIMER1_IMR_R = 0x01;
  NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF0FFF) | 0x04000; // set priority 2 to TIMER1
  NVIC_EN0_R = (1<<21); // enable IQR 21 in NVIC
}

void TIMER0_delay(uint32 delay) {
    TIMER0_TAILR_R = (unsigned int)(delay * 16000) - 1;  
    TIMER0_CTL_R |= 0x3; // enable TIMER0 and stalling
    // make sure that the interrupt is from TIMER0, not from the PORTF switch
    while(TIMER0_TAILR_R != 0) {
      __asm("    wfi\n"); // wait for interrupt
      if(TIMER0_TAILR_R != 0)
        RestoreState();
    }
}

void TIMER1_delay(uint32 delay) {
    TIMER1_TAILR_R = (unsigned int)(delay * 16000) - 1;
    TIMER1_CTL_R |= 0x3;
    __asm("    wfi\n");
}

void Timer0Handler(void) { 
  TIMER0_CTL_R = 0x00; // disable timer
  TIMER0_TAILR_R = 0x0;
  TIMER0_ICR_R = 0x01; // clear raw interrupt status timeout flag
}

void Timer1Handler(void) {
  TIMER1_CTL_R = 0x00;
  TIMER1_ICR_R = 0x01;
}

void PortFHandler(void) {
  TIMER0_CTL_R = 0x0;
  // pedestrians handling
  if (GPIO_PORTF_MIS_R & 0x10) { // if switch1 interrupt = EeastWest
    TrafficLEDs(0,0,1,1,0,0,1,0,0,1);
    TIMER1_delay(2000);
    GPIO_PORTF_ICR_R |= 0x10; // clear interrupt flag
  }
  else if (GPIO_PORTF_MIS_R & 0x01) { // if switch2 interrupt = NorthSouth
    TrafficLEDs(1,0,0,0,0,1,0,1,1,0);
    TIMER1_delay(2000);
    GPIO_PORTF_ICR_R |= 0x01;
  }
  TIMER0_CTL_R |= 0x3;
}

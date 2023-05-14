#include "DIO.h"

void PORTF_init(void) {
  SYSCTL_RCGCGPIO_R |= 0x00000020;
  while((SYSCTL_PRGPIO_R & 0x00000020) == 0);
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0x1F;
  GPIO_PORTF_DIR_R = 0x0E;
  GPIO_PORTF_PUR_R = 0x11;
  GPIO_PORTF_DEN_R = 0x1F;
  GPIO_PORTF_IS_R &= ~(1<<4) | ~(1<<0); // switches interrupts are edge sensitive
  GPIO_PORTF_IBE_R &= ~(1<<4) | ~(1<<0); // trigger is controlled by iev
  GPIO_PORTF_IEV_R &= ~(1<<4) | ~(1<<0); // switches interrupts are -ve edge triggered
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF0FFFFF) | 0x00600000; // set interrupt priority to 2
  GPIO_PORTF_ICR_R |= (1<<4) | (1<<0);
  GPIO_PORTF_IM_R = (1<<4) | (1<<0); // enable interrupts for both switches
  NVIC_EN0_R = 0x40000000; // enable interrupts
}

void PORTB_init(void) { // for traffic
  SYSCTL_RCGCGPIO_R |= 0x00000002;
  while((SYSCTL_PRGPIO_R & 0x00000002) == 0);
  GPIO_PORTB_CR_R = 0x9F;
  GPIO_PORTB_DIR_R = 0x9F;
  GPIO_PORTB_PUR_R = 0x00;
  GPIO_PORTB_DEN_R = 0x9F;
}

void PORTC_init(void) { // for pedestrians
  SYSCTL_RCGCGPIO_R |= 0x00000004;
  while((SYSCTL_PRGPIO_R & 0x00000004) == 0);
  GPIO_PORTC_CR_R = 0xF0;
  GPIO_PORTC_DIR_R = 0xF0;
  GPIO_PORTC_PUR_R = 0x00;
  GPIO_PORTC_DEN_R = 0xF0;
}

// NSR = NorthSouth Red, NSY = NorthSouth Yellow, NSG = NorthSouth Green
// EWR = EastWest Red, EWY = EastWest Yellow, EWG = EastWest Green
// PNSR = Pedestrians NorthSouth Red, PNSG = Pedestrians NorthSouth Green
// PEWR = Pedestrians EastWest Red, PEWG = Pedestrians EastWest Green
void TrafficLEDs(bool NSR, bool NSY, bool NSG, bool EWR, bool EWY, bool EWG, bool PNSR, bool PNSG, bool PEWR, bool PEWG) {
  __asm("CPSID I");
  GPIO_PORTB_DATA_R = (NSR<<0) | (NSY<<1) | (NSG<<2) | (EWR<<3) | (EWY<<4) | (EWG<<7);
  GPIO_PORTC_DATA_R = (PNSR<<4) | (PNSG<<5) | (PEWR<<6) | (PEWG<<7);
  __asm("CPSIE I");
}
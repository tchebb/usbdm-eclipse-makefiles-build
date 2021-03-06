#include <stdint.h>
#include "derivative.h"
#include "utilities.h"

#define GREEN_LED_MASK    (1<<0)
#define GREEN_LED2_MASK   (1<<1)
#define RED_LED_MASK      (1<<2)
#define YELLOW_LED_MASK   (1<<3)

void greenLedOn(void) {
   GPIO->CLRTC = ~GREEN_LED_MASK;
}
void greenLedOff(void) {
   GPIO->SETTC = GREEN_LED_MASK;
}
void greenLedToggle(void) {
   GPIO->PORTTC ^= GREEN_LED_MASK;
}
void greenLedEnable(void) {
   GPIO->DDRTC |= GREEN_LED_MASK;
}
void greenLedDisable(void) {
   GPIO->DDRTC &= ~GREEN_LED_MASK;
}

void greenLed2On(void) {
   GPIO->CLRTC = ~GREEN_LED2_MASK;
}
void greenLed2Off(void) {
   GPIO->SETTC = GREEN_LED2_MASK;
}
void greenLed2Toggle(void) {
   GPIO->PORTTC ^= GREEN_LED2_MASK;
}
void greenLed2Enable(void) {
   GPIO->DDRTC |= GREEN_LED2_MASK;
}
void greenLed2Disable(void) {
   GPIO->DDRTC &= ~GREEN_LED2_MASK;
}

void redLedOn(void) {
   GPIO->CLRTC = ~RED_LED_MASK;
}
void redLedOff(void) {
   GPIO->SETTC = RED_LED_MASK;
}
void redLedToggle(void) {
   GPIO->PORTTC ^= RED_LED_MASK;
}
void redLedEnable(void) {
   GPIO->DDRTC |= RED_LED_MASK;
}
void redLedDisable(void) {
   GPIO->DDRTC &= ~RED_LED_MASK;
}

void yellowLedOn(void) {
   GPIO->CLRTC = ~YELLOW_LED_MASK;
}
void yellowLedOff(void) {
   GPIO->SETTC = YELLOW_LED_MASK;
}
void yellowLedToggle(void) {
   GPIO->PORTTC ^= YELLOW_LED_MASK;
}
void yellowLedEnable(void) {
   GPIO->DDRTC |= YELLOW_LED_MASK;
}
void yellowLedDisable(void) {
   GPIO->DDRTC &= ~YELLOW_LED_MASK;
}

#define PAR_GPIO (0)

void led_initialise(void) {
   // Configure pins as GPIO function
   GPIO->QPARTC  = GPIO_QPAR_QPAR3(PAR_GPIO)|GPIO_QPAR_QPAR2(PAR_GPIO)|GPIO_QPAR_QPAR1(PAR_GPIO)|GPIO_QPAR_QPAR0(PAR_GPIO);
   GPIO->DDRTC  |= GREEN_LED_MASK|GREEN_LED2_MASK|RED_LED_MASK|YELLOW_LED_MASK;
}

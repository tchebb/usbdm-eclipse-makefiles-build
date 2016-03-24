/**
 * @file      gpio.cpp (from gpio-MK22F12-FRDM.cpp)
 *
 * @brief     Pin declarations for MK22F12-FRDM, generated from MK22FA12_64p.csv
 *            Devices   [MK22FX512M12, MK22FX512AM12, MK22FN1M0M12, MK22FN1M0AM12]
 *            Reference [K22P64M120SF5RM, K22P64M120SF5V2RM]
 */

#include "gpio.h"

namespace USBDM {

#if (DO_MAP_PINS_ON_RESET>0)
struct PinInit {
   uint32_t pcrValue;
   uint32_t volatile *pcr;
};

static constexpr PinInit pinInit[] = {
#if (PTA0_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA0_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[0]},
#endif
#if (PTA1_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA1_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[1]},
#endif
#if (PTA2_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA2_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[2]},
#endif
#if (PTA3_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA3_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[3]},
#endif
#if (PTA4_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA4_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[4]},
#endif
#if (PTA5_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA5_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[5]},
#endif
#if (PTA12_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA12_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[12]},
#endif
#if (PTA13_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA13_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[13]},
#endif
#if (PTA18_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA18_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[18]},
#endif
#if (PTA19_SIG_SEL>=0)
   { PORT_PCR_MUX(PTA19_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTA->PCR[19]},
#endif
#if (PTB0_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB0_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[0]},
#endif
#if (PTB1_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB1_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[1]},
#endif
#if (PTB2_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB2_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[2]},
#endif
#if (PTB3_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB3_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[3]},
#endif
#if (PTB16_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB16_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[16]},
#endif
#if (PTB17_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB17_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[17]},
#endif
#if (PTB18_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB18_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[18]},
#endif
#if (PTB19_SIG_SEL>=0)
   { PORT_PCR_MUX(PTB19_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTB->PCR[19]},
#endif
#if (PTC0_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC0_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[0]},
#endif
#if (PTC1_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC1_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[1]},
#endif
#if (PTC2_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC2_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[2]},
#endif
#if (PTC3_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC3_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[3]},
#endif
#if (PTC4_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC4_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[4]},
#endif
#if (PTC5_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC5_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[5]},
#endif
#if (PTC6_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC6_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[6]},
#endif
#if (PTC7_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC7_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[7]},
#endif
#if (PTC8_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC8_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[8]},
#endif
#if (PTC9_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC9_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[9]},
#endif
#if (PTC10_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC10_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[10]},
#endif
#if (PTC11_SIG_SEL>=0)
   { PORT_PCR_MUX(PTC11_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTC->PCR[11]},
#endif
#if (PTD0_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD0_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[0]},
#endif
#if (PTD1_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD1_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[1]},
#endif
#if (PTD2_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD2_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[2]},
#endif
#if (PTD3_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD3_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[3]},
#endif
#if (PTD4_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD4_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[4]},
#endif
#if (PTD5_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD5_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[5]},
#endif
#if (PTD6_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD6_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[6]},
#endif
#if (PTD7_SIG_SEL>=0)
   { PORT_PCR_MUX(PTD7_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTD->PCR[7]},
#endif
#if (PTE0_SIG_SEL>=0)
   { PORT_PCR_MUX(PTE0_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTE->PCR[0]},
#endif
#if (PTE1_SIG_SEL>=0)
   { PORT_PCR_MUX(PTE1_SIG_SEL)|USBDM::DEFAULT_PCR, &PORTE->PCR[1]},
#endif
};

/**
 * Used to configure pin-mapping before 1st use of peripherals
 */
void usbdm_PinMapping() {
#if (PTA0_SIG_SEL>=0) || (PTA1_SIG_SEL>=0) || (PTA2_SIG_SEL>=0) || (PTA3_SIG_SEL>=0) || \
    (PTA4_SIG_SEL>=0) || (PTA5_SIG_SEL>=0) || (PTA12_SIG_SEL>=0) || (PTA13_SIG_SEL>=0) || \
    (PTA18_SIG_SEL>=0) || (PTA19_SIG_SEL>=0)

   SIM->FIXED_PORT_CLOCK_REG |= PORTA_CLOCK_MASK;
#endif
#if (PTB0_SIG_SEL>=0) || (PTB1_SIG_SEL>=0) || (PTB2_SIG_SEL>=0) || \
    (PTB3_SIG_SEL>=0) || (PTB16_SIG_SEL>=0) || (PTB17_SIG_SEL>=0) || (PTB18_SIG_SEL>=0) || \
    (PTB19_SIG_SEL>=0)

   SIM->FIXED_PORT_CLOCK_REG |= PORTB_CLOCK_MASK;
#endif
#if (PTC0_SIG_SEL>=0) || (PTC1_SIG_SEL>=0) || (PTC2_SIG_SEL>=0) || (PTC3_SIG_SEL>=0) || \
    (PTC4_SIG_SEL>=0) || (PTC5_SIG_SEL>=0) || (PTC6_SIG_SEL>=0) || (PTC7_SIG_SEL>=0) || \
    (PTC8_SIG_SEL>=0) || (PTC9_SIG_SEL>=0) || (PTC10_SIG_SEL>=0) || (PTC11_SIG_SEL>=0)

   SIM->FIXED_PORT_CLOCK_REG |= PORTC_CLOCK_MASK;
#endif
#if (PTD0_SIG_SEL>=0) || \
    (PTD1_SIG_SEL>=0) || (PTD2_SIG_SEL>=0) || (PTD3_SIG_SEL>=0) || (PTD4_SIG_SEL>=0) || \
    (PTD5_SIG_SEL>=0) || (PTD6_SIG_SEL>=0) || (PTD7_SIG_SEL>=0)

   SIM->FIXED_PORT_CLOCK_REG |= PORTD_CLOCK_MASK;
#endif
#if (PTE0_SIG_SEL>=0) || (PTE1_SIG_SEL>=0)
   SIM->FIXED_PORT_CLOCK_REG |= PORTE_CLOCK_MASK;
#endif

   for (const PinInit *p=pinInit; p<(pinInit+(sizeof(pinInit)/sizeof(pinInit[0]))); p++) {
      *(p->pcr) = p->pcrValue;
   }
}
#endif

} // End namespace USBDM

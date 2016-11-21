/*
 * @file mcg.cpp (derived from mcg_mkl.cpp)
 *
 * Based on KL25
 *    1 FLL (FRDIV=/1-/128, /32-/1024, /1280, /1536)
 *    1 PLL (VCO PRDIV=/1-/24, VDIV=pll_vdiv_min+x) (pll_vdiv_min=x16,x24)
 *    RTC enable forces low range
 *    SIM_CLKDIV1_OUTDIV4_MASK divides core clock (post divider)
 *
 *  Created on: 04/03/2012
 *      Author: podonoghue
 */
#include "string.h"
#include "derivative.h" /* include peripheral declarations */
#include "system.h"
#include "utilities.h"
#include "stdbool.h"
#include "hardware.h"
#include "rtc.h"
#include "mcg.h"
#include "osc.h"
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
extern "C" uint32_t SystemCoreClock;
extern "C" uint32_t SystemBusClock;

namespace USBDM {

$(/MCG/clockInfo)
volatile uint32_t SystemMcgffClock;
volatile uint32_t SystemMcgFllClock;
volatile uint32_t SystemMcgPllClock;
volatile uint32_t SystemMcgOutClock;
volatile uint32_t SystemCoreClock;
volatile uint32_t SystemBusClock;
volatile uint32_t SystemLpoClock;

/** Callback for programmatically set handler */
MCGCallbackFunction Mcg::callback = {0};

/** Current clock mode (FEI out of reset) */
McgInfo::ClockMode Mcg::currentClockMode = McgInfo::ClockMode::ClockMode_FEI;

#ifdef SMC_PMPROT_AHSRUN_MASK
/**
 * Switch to/from high speed run mode
 * Changes the CPU clock frequency/1, and bus clock frequency /2
 * If the clock is set up for 120 MHz this will be the highest performance possible.
 *
 * This routine assumes that the clock preferences have been set up for the usual RUN mode and only
 * the Core clock divider needs to be changed.
 */
void Mcg::hsRunMode(bool enable) {
   SMC->PMPROT = SMC_PMPROT_AHSRUN_MASK;

   if (enable) {
      SMC->PMCTRL = SMC_PMCTRL_RUNM(3);
      while ((SMC->PMSTAT & 0x80) == 0) {
         // Wait for mode change
         __asm__("nop");
      }
      // Set the SIM _CLKDIV dividers (CPU /1, Bus /2)
      SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0))|(SIM_CLKDIV1_OUTDIV2(1))|(SimInfo::clkdiv1 & (SIM_CLKDIV1_OUTDIV3_MASK|SIM_CLKDIV1_OUTDIV4_MASK));
   }
   else {
      // Set the SIM _CLKDIV dividers (CPU normal)
      SIM->CLKDIV1 = SimInfo::clkdiv1;
      SMC->PMCTRL = SMC_PMCTRL_RUNM(0);
   }
   SystemCoreClockUpdate();
}
#endif

constexpr uint8_t clockTransitionTable[8][8] = {
         /*  from                 to =>   ClockMode_FEI,           ClockMode_FEE,           ClockMode_FBI,           ClockMode_BLPI,          ClockMode_FBE,           ClockMode_BLPE,          ClockMode_PBE,           ClockMode_PEE */
         /* ClockMode_FEI,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_FEE,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_FBI,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_BLPI, McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_BLPI, */ { McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI, },
         /* ClockMode_FBE,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_BLPE, McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE, },
         /* ClockMode_BLPE, */ { McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_BLPE, McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE, },
         /* ClockMode_PBE,  */ { McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_BLPE, McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PEE, },
         /* ClockMode_PEE,  */ { McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PEE, },
   };

#ifndef SIM_CLKDIV1_OUTDIV3
#define SIM_CLKDIV1_OUTDIV3(x) 0
#endif

#ifndef SIM_CLKDIV1_OUTDIV2
#define SIM_CLKDIV1_OUTDIV2(x) 0
#endif

/**
 * Transition from current clock mode to mode given
 *
 * @param to Clock mode to transition to
 *
 * @return E_NO_ERROR on success
 */
int Mcg::clockTransition(const McgInfo::ClockInfo &clockInfo) {
   McgInfo::ClockMode to = clockInfo.clockMode;

   //TODO move!
#ifdef USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK
   if (clockInfo.c7&&MCG_C7_OSCSEL_MASK) {
      // Note IRC48M Internal Oscillator automatically enable if MCG_C7_OSCSEL = 2
      SIM->SCGC4 |= SIM_SCGC4_USBOTG_MASK;
      USB0->CLK_RECOVER_IRC_EN = USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK|USB_CLK_RECOVER_IRC_EN_REG_EN_MASK;
   }
#endif

   // Set PLL PRDIV0 etc
   mcg->C5  = clockInfo.c5;

   // Set PLL VDIV0 etc
   mcg->C6  = clockInfo.c6;
   
#ifdef MCG_C7_OSCSEL
   // Select OSCCLK Source
   mcg->C7 = clockInfo.c7; // OSCSEL = 0,1,2 -> XTAL/XTAL32/IRC48M
#endif

   // Set Fast Internal Clock divider
   mcg->SC = clockInfo.sc;

   // Set conservative clock dividers
   setSysDividers(SIM_CLKDIV1_OUTDIV4(5)|SIM_CLKDIV1_OUTDIV3(5)|SIM_CLKDIV1_OUTDIV2(5)|SIM_CLKDIV1_OUTDIV1(5));

   if (to != McgInfo::ClockMode_None) {
      int transitionCount = 0;
      do {
         // Used to indicate that clock stabilization wait is needed
         bool externalClockInUse = false;

         McgInfo::ClockMode next;
         if (currentClockMode == McgInfo::ClockMode_None) {
            next = McgInfo::ClockMode_FEI;
         }
         else {
            next = (McgInfo::ClockMode)clockTransitionTable[currentClockMode][to];
         }
         switch (next) {

         case McgInfo::ClockMode_None:
         case McgInfo::ClockMode_FEI: // From FEE, FBI, FBE or reset(FEI)

            mcg->C2 = clockInfo.c2;

            mcg->C1 =
                  MCG_C1_CLKS(0)           | // CLKS     = 0     -> Output of FLL is selected
                  MCG_C1_IREFS(1)          | // IREFS    = 1     -> Slow IRC for FLL source
                  clockInfo.c1;               // FRDIV, IRCLKEN, IREFSTEN

            // Wait for S_IREFST to indicate FLL Reference has switched to IRC
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_IREFST_MASK) != (MCG_S_IREFST(1)));

            // Wait for S_CLKST to indicating that OUTCLK has switched to FLL
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(0));

            // Set FLL Parameters
            mcg->C4 = (mcg->C4&(MCG_C4_FCTRIM_MASK|MCG_C4_SCFTRIM_MASK))|clockInfo.c4;
            break;

         case McgInfo::ClockMode_FEE: // from FEI, FBI or FBE
            mcg->C1 =
                  MCG_C1_CLKS(0)           | // CLKS     = 0     -> Output of FLL is selected
                  MCG_C1_IREFS(0)          | // IREFS    = 0     -> External reference clock is FLL source
                  clockInfo.c1;           // FRDIV, IRCLKEN, IREFSTEN

            // Wait for S_IREFST to indicate FLL Reference has switched to ERC
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_IREFST_MASK) != (MCG_S_IREFST(0)));

            // Wait for S_CLKST to indicating that OUTCLK has switched to FLL
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(0));
            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_FBI: // from BLPI, FEI, FEE, FBE
            mcg->C1 =
                  MCG_C1_CLKS(1)           | // CLKS     = 1     -> Internal reference clock is selected
                  MCG_C1_IREFS(1)          | // IREFS    = 1     -> Slow IRC for FLL source
                  clockInfo.c1;           // FRDIV, IRCLKEN, IREFSTEN

            // Clear LP
            mcg->C2 = clockInfo.c2;

            // Wait for S_CLKST to indicating that OUTCLK has switched to IRC
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(1));
            break;

         case McgInfo::ClockMode_FBE: // from FEI, FEE, FBI, PBE, BLPE
            // Clear LP
            mcg->C2 = clockInfo.c2;

            mcg->C1 =
                  MCG_C1_CLKS(2)           | // CLKS     = 2     -> External reference clock is selected
                  MCG_C1_IREFS(0)          | // IREFS    = 1     -> Slow IRC for FLL source
                  clockInfo.c1;           // FRDIV, IRCLKEN, IREFSTEN

            // Select FLL as MCG clock source
            mcg->C6  = clockInfo.c6;

            // Wait for S_IREFST to indicate FLL Reference has switched to ERC
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_IREFST_MASK) != (MCG_S_IREFST(0)));

            // Wait for S_CLKST to indicating that OUTCLK has switched to IRC
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2));

            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_PBE: // from FBE, BLPE, PEE
            // Clear LP
            mcg->C2 = clockInfo.c2;

            mcg->C1 =
                  MCG_C1_CLKS(2)           | // CLKS     = 2     -> External reference clock is selected
                  MCG_C1_IREFS(0)          | // IREFS    = 1     -> Slow IRC for FLL source
                  clockInfo.c1;           // FRDIV, IRCLKEN, IREFSTEN

            // Select PLL as MCG clock source
            mcg->C6  = clockInfo.c6|MCG_C6_PLLS_MASK;
            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_PEE: // from PBE
            mcg->C1 =
                  MCG_C1_CLKS(0)           | // CLKS     = 0     -> Output of PLLCS is selected
                  MCG_C1_IREFS(0)          | // IREFS    = 1     -> Slow IRC for FLL source
                  clockInfo.c1;           // FRDIV, IRCLKEN, IREFSTEN
            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_BLPE: // from FBE, PBE (registers differ depending on transition)
            externalClockInUse = true;
            //no break

         case McgInfo::ClockMode_BLPI: // from FBI
            // Set LP
            mcg->C2 = clockInfo.c2|MCG_C2_LP_MASK;
            break;
         }
         // Wait for oscillator stable (if used)
         if (externalClockInUse && (clockInfo.c2&MCG_C2_EREFS0_MASK)) {
            do {
               __asm__("nop");
            } while ((mcg->S & MCG_S_OSCINIT0_MASK) == 0);
         }
         currentClockMode = next;
         if (transitionCount++>5) {
            return -1;
         }
      } while (currentClockMode != to);
   }
   setSysDividers(SimInfo::clkdiv1);

   SystemCoreClockUpdate();
   return 0;
}

/**
 * Update SystemCoreClock variable
 *
 * Updates the SystemCoreClock variable with current core Clock retrieved from CPU registers.
 */
void Mcg::SystemCoreClockUpdate(void) {

   uint32_t mcg_erc_clock = McgInfo::getErcClock();

   if ((mcg->C1&MCG_C1_IREFS_MASK) == 0) {
      // External reference clock is selected
      SystemMcgffClock = mcg_erc_clock/(1<<((mcg->C1&MCG_C1_FRDIV_MASK)>>MCG_C1_FRDIV_SHIFT));

      if (((mcg->C2&MCG_C2_RANGE0_MASK) != 0) && 
	     ((RtcInfo::rtc->CR&RTC_CR_OSCE_MASK) == 0)) {
         // High divisors - extra division
         if ((mcg->C1&MCG_C1_FRDIV_MASK) == MCG_C1_FRDIV(6)) {
            SystemMcgffClock /= 20;
         }
         else if ((mcg->C1&MCG_C1_FRDIV_MASK) == MCG_C1_FRDIV(7)) {
            SystemMcgffClock /= 12;
         }
         else {
            SystemMcgffClock /= 32;
         }
      }
   }
   else {
      // External reference clock is selected
      SystemMcgffClock = McgInfo::system_slow_irc_clock;
   }

   uint32_t systemFllClock = SystemMcgffClock * ((mcg->C4&MCG_C4_DMX32_MASK)?732:640) * (((mcg->C4&MCG_C4_DRST_DRS_MASK)>>MCG_C4_DRST_DRS_SHIFT)+1);

   uint32_t systemPllClock = 0;
   systemPllClock  = (mcg_erc_clock/10)*(((mcg->C6&MCG_C6_VDIV0_MASK)>>MCG_C6_VDIV0_SHIFT)+McgInfo::pll_vdiv_min);
   systemPllClock /= ((mcg->C5&MCG_C5_PRDIV0_MASK)>>MCG_C5_PRDIV0_SHIFT)+1;
   systemPllClock *= (10/McgInfo::pll_post_divider);

   SystemMcgPllClock = 0;
   SystemMcgFllClock = 0;
   switch (mcg->S&MCG_S_CLKST_MASK) {
      case MCG_S_CLKST(0) : // FLL
         SystemMcgOutClock = systemFllClock;
         SystemMcgFllClock = systemFllClock;
         break;
      case MCG_S_CLKST(1) : // Internal Reference Clock
         SystemMcgOutClock = McgInfo::getInternalIrcClock();
         break;
      case MCG_S_CLKST(2) : // External Reference Clock
         SystemMcgOutClock = McgInfo::getErcClock();
         break;
      case MCG_S_CLKST(3) : // PLL
         SystemMcgOutClock = systemPllClock;
         SystemMcgPllClock = systemPllClock;
         break;
   }
   if (mcg->C5&MCG_C5_PLLCLKEN0_MASK) {
      SystemMcgPllClock = systemPllClock;
   }
   SystemCoreClock   = SystemMcgOutClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT)+1);
   SystemBusClock    = SystemCoreClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV4_MASK)>>SIM_CLKDIV1_OUTDIV4_SHIFT)+1);

   SystemLpoClock    = 1000;

   ::SystemBusClock  = SystemBusClock;
   ::SystemCoreClock = SystemCoreClock;
}

/**
 * Sets up the clock out of RESET
 */
void Mcg::initialise(void) {

   currentClockMode = McgInfo::ClockMode::ClockMode_None;

   // Transition to desired clock mode
   clockTransition(McgInfo::clockInfo[0]);

   SimInfo::initRegs();

   SystemCoreClockUpdate();
}

} // end namespace USBDM

/**
 * Sets up the clock out of RESET
 */
extern "C"
void clock_initialise(void) {

#ifdef USBDM_OSC0_IS_DEFINED
   USBDM::Osc0::initialise();
#endif

#ifdef USBDM_RTC_IS_DEFINED
   USBDM::Rtc::initialise();
#endif

#ifdef USBDM_MCG_IS_DEFINED
   USBDM::Mcg::initialise();
#endif

}


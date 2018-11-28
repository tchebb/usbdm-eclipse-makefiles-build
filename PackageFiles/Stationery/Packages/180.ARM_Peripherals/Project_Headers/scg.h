/**
 * @file      scg.h (180.ARM_Peripherals/Project_Headers/mgc.h)
 *
 * @brief    Abstraction layer for MCG interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */

#ifndef INCLUDE_USBDM_SCG_H_
#define INCLUDE_USBDM_SCG_H_
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "derivative.h"
#include "system.h"
#include "hardware.h"

namespace USBDM {

/**
 * @addtogroup SCG_Group SCG, Multipurpose Clock Generator
 * @brief Abstraction for Multipurpose Clock Generator
 * @{
 */

/** Core/System clock (from SCGOUTCLK/CLKDIV) */
extern volatile uint32_t SystemCoreClock;
/** Bus clock (from SCGOUTCLK/CLKDIV) */
extern volatile uint32_t SystemBusClock;
/** LPO - Low power oscillator 1kHz clock available in LP modes */
extern volatile uint32_t SystemLpoClock;

/**
 * Clock configuration names
 */
enum ClockConfig {
$(/SCG/ClockConfig:// XXXX !!!!!!!ClockConfig - Not found!!!!!!!)
   ClockConfig_default = 0,
};

/**
 * Type definition for SCG interrupt call back
 */
typedef void (*SCGCallbackFunction)(void);

/**
 * @brief Class representing the SCG
 *
 * <b>Example</b>
 * @code
 *    Scg::initialise();
 * @endcode
 */
class Scg {

private:
   /** Callback function for ISR */
   static SCGCallbackFunction callback;

   /** Hardware instance */
   static __attribute__((always_inline)) volatile SCG_Type &scg() { return ScgInfo::scg(); }

public:
   /**
    * Table of clock settings
    */
   static const ScgInfo::ClockInfo clockInfo[];

   /**
    * Transition from current clock mode to mode given
    *
    * @param[in]  to Clock mode to transition to
    *
    * @return E_NO_ERROR on success
    */
   static ErrorCode clockTransition(const ScgInfo::ClockInfo &to);

   /**
    * Transition from current clock mode to mode given
    *
    * @param clockConfig Clock configuration index
    */
   static void clockTransition(ClockConfig clockConfig) {
      Scg::clockTransition(Scg::clockInfo[clockConfig]);
   }
   
   /**
    * Update SystemCoreClock variable
    *
    * Updates the SystemCoreClock variable with current core Clock retrieved from CPU registers.
    */
   static void SystemCoreClockUpdate(void);

   /**
    * Enable interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    */
   static void enableNvicInterrupts() {
      enableNvicInterrupt(ScgInfo::irqNums[0]);
   }

   /**
    * Enable and set priority of interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    *
    * @param[in]  nvicPriority  Interrupt priority
    */
   static void enableNvicInterrupts(uint32_t nvicPriority) {
      enableNvicInterrupt(ScgInfo::irqNums[0], nvicPriority);
   }

   /**
    * Disable interrupts in NVIC
    */
   static void disableNvicInterrupts() {
      NVIC_DisableIRQ(ScgInfo::irqNums[0]);
   }
   /**
    * SCG interrupt handler -  Calls SCG callback
    */
   static void irqHandler() {
      if (callback != 0) {
         callback();
      }
   }

   /**
    * Set callback for ISR
    *
    * @param[in]  callback The function to call from stub ISR
    */
   static void setCallback(SCGCallbackFunction callback) {
      Scg::callback = callback;
   }

   /**
    * Get current clock mode
    *
    * @return clock mode
    */
   static ScgInfo::ClockMode getClockMode();

   /**
    * Get name for clock mode
    *
    * @return Pointer to static string
    */
   static const char *getClockModeName(ScgInfo::ClockMode);

   /**
    * Get name for current clock mode
    *
    * @return Pointer to static string
    */
   static const char *getClockModeName() {
      return getClockModeName(getClockMode());
   }

   /**
    *  Configure the SCG for given mode
    *
    *  @param[in]  settingNumber CLock setting number
    */
   static void configure(ClockConfig settingNumber=ClockConfig_default) {
      clockTransition(clockInfo[settingNumber]);
   }

   /**
    *   Finalise the SCG
    */
   static void finalise() {
      clockTransition(clockInfo[ClockConfig_default]);
   }

   /**
    * Initialise SCG to default settings.
    */
   static void defaultConfigure();

   /**
    * Set up the OSC out of reset.
    */
   static void initialise() {
      defaultConfigure();
   }

};

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_SCG_H_ */

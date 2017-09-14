/**
 * @file     lptmr.h (180.ARM_Peripherals/Project_Headers/lptmr.h)
 * @brief    Low power timer interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef INCLUDE_USBDM_LPTMR_H_
#define INCLUDE_USBDM_LPTMR_H_
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
#include "cmath"

namespace USBDM {

/**
 * @addtogroup LPTMR_Group LPTMR, Low Power Timer
 * @brief Abstraction for Low Power Timer
 * @{
 */

/**
 * Type definition for LPTMR interrupt call back
 */
typedef void (*LPTMRCallbackFunction)(void);

/**
 * Select the LPTMR clock clock source which determines count speed or glitch filtering
 */
enum LptmrClockSel {
   LptmrClockSel_mcgirclk = LPTMR_PSR_PCS(0), //!< MCG Internal Reference Clock (MCGIRCLK)
   LptmrClockSel_lpoclk   = LPTMR_PSR_PCS(1), //!< Low power oscillator (LPO - 1kHz)
   LptmrClockSel_erclk32  = LPTMR_PSR_PCS(2), //!< 32kHz Clock Source (ERCLK32)
   LptmrClockSel_oscerclk = LPTMR_PSR_PCS(3), //!< Oscillator External Reference Clock (OSCERCLK)
};

/**
 * Select the LPTMR clock pre-scale which affect counter speed/glitch filtering
 */
enum LptmrPrescale {
   LptmrPrescale_Bypass = LPTMR_PSR_PBYP(1),       //!< Divide is bypassed (no divider/glitch filtering)
   LptmrPrescale_2      = LPTMR_PSR_PRESCALE(0),   //!< Divide by 2
   LptmrPrescale_4      = LPTMR_PSR_PRESCALE(1),   //!< Divide by 4
   LptmrPrescale_8      = LPTMR_PSR_PRESCALE(2),   //!< Divide by 8
   LptmrPrescale_16     = LPTMR_PSR_PRESCALE(3),   //!< Divide by 16
   LptmrPrescale_32     = LPTMR_PSR_PRESCALE(4),   //!< Divide by 32
   LptmrPrescale_64     = LPTMR_PSR_PRESCALE(5),   //!< Divide by 64
   LptmrPrescale_128    = LPTMR_PSR_PRESCALE(6),   //!< Divide by 128
   LptmrPrescale_256    = LPTMR_PSR_PRESCALE(7),   //!< Divide by 256
   LptmrPrescale_512    = LPTMR_PSR_PRESCALE(8),   //!< Divide by 512
   LptmrPrescale_1024   = LPTMR_PSR_PRESCALE(9),   //!< Divide by 1024
   LptmrPrescale_2048   = LPTMR_PSR_PRESCALE(10),  //!< Divide by 2048
   LptmrPrescale_4096   = LPTMR_PSR_PRESCALE(11),  //!< Divide by 4096
   LptmrPrescale_8192   = LPTMR_PSR_PRESCALE(12),  //!< Divide by 8192
   LptmrPrescale_16384  = LPTMR_PSR_PRESCALE(13),  //!< Divide by 16384
   LptmrPrescale_32768  = LPTMR_PSR_PRESCALE(14),  //!< Divide by 32768
   LptmrPrescale_65536  = LPTMR_PSR_PRESCALE(15),  //!< Divide by 65536
};

/**
 * Select the LPTMR clock input pin
 */
enum LptmrPinSel {
   LptmrPinSel_cmp0 = LPTMR_CSR_TPS(0), //!< CMP pin
   LptmrPinSel_alt1 = LPTMR_CSR_TPS(1), //!< LPTMR_ALT1 pin
   LptmrPinSel_alt2 = LPTMR_CSR_TPS(2), //!< LPTMR_ALT1 pin
};

/**
 * Select the LPTMR Pulse Mode input pin polarity
 */
enum LptmrPulseEdge {
   LptmrPulse_RisingEdge  = LPTMR_CSR_TPP(0), //!< LPTMR Time counting mode
   LptmrPulse_FallingEdge = LPTMR_CSR_TPP(1), //!< LPTMR Pulse counting mode
};

/**
 * Select when the LPTMR counter resets to zero
 */
enum LptmrResetOn {
   LptmrResetOn_Compare  = LPTMR_CSR_TFC(0), //!< LPTMR Counter is reset whenever TCF is set.
   LptmrResetOn_Overflow = LPTMR_CSR_TFC(1), //!< LPTMR Counter is reset on overflow.
};

/**
 * @brief Template class representing a Low Power Timer
 */
template<class Info>
class LptmrBase_T {

protected:
   /** Minimum resolution required when setting interval */
   static constexpr int MINIMUM_RESOLUTION = 100;

   /** Callback function for ISR */
   static LPTMRCallbackFunction callback;

   /** Pointer to hardware */
   static constexpr volatile LPTMR_Type *lptmr     = Info::lptmr;

   /* Pointer to clock register */
   static constexpr volatile uint32_t   *clockReg  = Info::clockReg;

public:
   /**
    * Configures all mapped pins associated with this peripheral
    */
   static void __attribute__((always_inline)) configureAllPins() {
      // Configure pins
      Info::initPCRs();
   }

   /**
    * Enables the LPTMR clock and configures the pins
    */
   static void __attribute__((always_inline)) enable() {
      configureAllPins();

      // Enable clock
      *clockReg |= Info::clockMask;
      __DMB();
   }
   
   /**
    * Set LPTMR to pulse counting mode with selection of input pin, edge selection and reset mode.\n
    * The timer is enabled.
    *
    * @param[in] lptmrPinSel     Input pin for Pulse Counting mode
    * @param[in] lptmrPulseEdge  Edge for pulse counting (default = rising-edge)
    * @param[in] lptmrResetOn    Selects when the LPTMR counter resets to zero (default = on overflow)
    */
   static void __attribute__((always_inline)) setPulseCountingMode(
         LptmrPinSel       lptmrPinSel,
         LptmrPulseEdge    lptmrPulseEdge=LptmrPulse_RisingEdge,
         LptmrResetOn      lptmrResetOn=LptmrResetOn_Overflow) {

      enable();
      // Change settings with timer disabled
      lptmr->CSR = LPTMR_CSR_TMS_MASK|lptmrPinSel|lptmrPulseEdge|lptmrResetOn;
      // Enable timer
      lptmr->CSR = LPTMR_CSR_TEN_MASK|LPTMR_CSR_TMS_MASK|lptmrPinSel|lptmrPulseEdge|lptmrResetOn;
   }

   /**
    * Set LPTMR to time counting mode.\n
    * The timer is enabled.
    *
    * @param[in] lptmrResetOn Selects when the LPTMR counter resets to zero (default = on compare event)
    */
   static void __attribute__((always_inline)) setTimeCountingMode(LptmrResetOn lptmrResetOn=LptmrResetOn_Compare) {
      enable();
      // Change settings with timer disabled
      lptmr->CSR = lptmrResetOn;
      // Enable timer
      lptmr->CSR = LPTMR_CSR_TEN_MASK|lptmrResetOn;
   }

   /**
    * Set LPTMR clock source and prescaler
    *
    * @param[in] lptmrClockSel   Clock source selection
    * @param[in] lptmrPrescale   Clock divider
    */
   static void __attribute__((always_inline)) setClock(LptmrClockSel lptmrClockSel, LptmrPrescale lptmrPrescale=LptmrPrescale_Bypass) {
      lptmr->PSR = lptmrClockSel|lptmrPrescale;
   }

   /**
    * Enable/disable interrupts in LPTMR
    *
    * @param[in]  enable true to enable, false to disable
    */
   static void __attribute__((always_inline)) enableInterrupts(bool enable=true) {
      if (enable) {
         lptmr->CSR |= LPTMR_CSR_TIE_MASK;
      }
      else {
         lptmr->CSR &= ~LPTMR_CSR_TIE_MASK;
      }
   }

   /**
    * Enable/disable interrupts in NVIC
    *
    * @param[in]  enable    True => enable, False => disable
    * @param[in]  priority  Interrupt priority
    */
   static void enableNvicInterrupts(bool enable=true, uint32_t priority=NvicPriority_Normal) {

      if (enable) {
         // Enable interrupts
         NVIC_EnableIRQ(Info::irqNums[0]);

         // Set priority level
         NVIC_SetPriority(Info::irqNums[0], priority);
      }
      else {
         // Disable interrupts
         NVIC_DisableIRQ(Info::irqNums[0]);
      }
   }

   /**
    * Set callback for ISR and enable NVIC interrupts
    *
    * @param[in]  theCallback The function to call from stub ISR
    *
    * @return E_NO_ERROR on success
    */
   static ErrorCode setCallback(LPTMRCallbackFunction theCallback) {
      if (!Info::irqHandlerInstalled) {
         return setErrorCode(E_NO_HANDLER);
      }
      callback = theCallback;

      enableNvicInterrupts();

      return E_NO_ERROR;
   }

   /**
    * PIT interrupt handler. \n
    * Calls PIT0 callback
    */
   static void irqHandler() {
      // Clear interrupt flag
      lptmr->CSR |= LPTMR_CSR_TCF_MASK;

      if (callback != 0) {
         callback();
      }
   }

   /**
    * Enable LPTMR\n
    * Includes enabling clock and any pins used.\n
    * Sets LPTMR to default configuration
    *
    *  @param[in]  period    Period for the timer in timer ticks
    *  @param[in]  csr       Control Status Register
    *  @param[in]  psr       Prescale Register
    */
   static void configure(uint16_t period=Info::cmr, uint32_t csr=Info::csr, uint32_t psr=Info::psr) {
      enable();
      // Disable timer
      lptmr->CSR  = csr;
      // PCS 0,1,2,3 => MCGIRCLK, LPO, ERCLK32K, OSCERCLK
      lptmr->PSR  = psr;
      // Period/Compare value
      lptmr->CMR  = period;
      // Enable timer
      lptmr->CSR |= LPTMR_CSR_TEN_MASK;

      if (csr & LPTMR_CSR_TIE_MASK) {
         // Enable timer interrupts
         NVIC_EnableIRQ(Info::irqNums[0]);

         // Set priority level
         NVIC_SetPriority(Info::irqNums[0], Info::irqLevel);
      }
   }
   /**
    *   Disable the LPTMR
    */
   static void finalise(void) {
      // Disable timer
      lptmr->CSR = 0;
      NVIC_DisableIRQ(Info::irqNums[0]);
      *clockReg &= ~Info::clockMask;
   }

   /**
    * Converts a time in microseconds to number of ticks
    *
    * @param[in]  time Time in microseconds
    *
    * @return Time in ticks
    *
    * @note Assumes prescale has been chosen appropriately.
    * @note Rudimentary range checking only.
    */
   static uint32_t convertMicrosecondsToTicks(int time) {

      // Calculate period
      uint32_t tickRate = Info::getClockFrequency();
      uint64_t rv       = ((uint64_t)time*tickRate)/1000000;

#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         __BKPT();
      }
      if (rv == 0) {
         // Attempt to set too short a period
         __BKPT();
      }
#endif
      return rv;
   }
   /**
    * Converts a time in milliseconds to number of ticks
    *
    * @param[in]  time Time in milliseconds
    *
    * @return Time in ticks
    *
    * @note Assumes prescale has been chosen appropriately.
    * @note Rudimentary range checking only.
    */
   static uint32_t convertMillisecondsToTicks(int time) {

      // Calculate period
      uint32_t tickRate = Info::getClockFrequency();
      uint64_t rv       = ((uint64_t)time*tickRate)/1000;

#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         __BKPT();
      }
      if (rv == 0) {
         // Attempt to set too short a period
         __BKPT();
      }
#endif
      return rv;
   }

   /**
    * Converts a time in seconds to number of ticks
    *
    * @param[in]  time Time in seconds (float!)
    *
    * @return Time in ticks
    *
    * @note Uses floating point
    */
   static uint32_t convertSecondsToTicks(float time) {

      // Calculate period
      float    tickRate = Info::getClockFrequencyF();
      uint64_t rv       = (time*tickRate);

#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         __BKPT();
      }
      if (rv == 0) {
         // Attempt to set too long a period
         __BKPT();
      }
#endif
      return rv;
   }

   /**
    * Set period of timer
    *
    * @param[in]  period Period in seconds as a float
    *
    * @note Will enable and adjust the pre-scaler to appropriate value.\n
    *       The clock source should be selected by setClock() before using this function.
    *
    * @return true => success, false => failed to find suitable values for PBYP & PRESCALE
    */
   static ErrorCode setPeriod(float period) {
      float    inputClock = Info::getInputClockFrequency();
      int      prescaleFactor=1;
      uint32_t prescalerValue=0;
      while (prescalerValue<=16) {
         float    clock = inputClock/prescaleFactor;
         uint32_t mod   = round(period*clock);
         if (mod < MINIMUM_RESOLUTION) {
            // Too short a period for 1% resolution
            return setErrorCode(E_TOO_SMALL);
         }
         if (mod <= 65535) {
            // Disable LPTMR before prescale change
            uint32_t csr = lptmr->CSR;
            lptmr->CSR = 0;
            __DSB();
            lptmr->CMR  = mod;
            lptmr->PSR  = (lptmr->PSR & ~(LPTMR_PSR_PRESCALE_MASK|LPTMR_PSR_PBYP_MASK))|LPTMR_PSR_PRESCALE(prescalerValue-1)|LPTMR_PSR_PBYP(prescalerValue==0);
            lptmr->CSR  = csr;
            return E_NO_ERROR;
         }
         prescalerValue++;
         prescaleFactor <<= 1;
      }
      // Too long a period
      return setErrorCode(E_TOO_LARGE);
   }
};

template<class Info> LPTMRCallbackFunction LptmrBase_T<Info>::callback = 0;

#ifdef LPTMR0
/**
 * @brief Class representing LPTMR0
 *
 * <b>Example</b>
 * @code
 *
 * // LPTMR callback
 * void flash(void) {
 *    RED_LED::toggle();
 * }
 *
 * ...
 *
 * // Configure LPTMR
 * Lptmr0::configure(1000);
 *
 * // This handler is set programmatically
 * Lptmr0::setCallback(flash);
 *
 * @endcode
 */
using Lptmr0 = LptmrBase_T<Lptmr0Info>;
#endif

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_LPTMR_H_ */

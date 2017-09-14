/**
 * @file     tpm.h (180.ARM_Peripherals/Project_Headers/tpm.h)
 * @brief    Flexitimer Timer Module
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef HEADER_TPM_H
#define HEADER_TPM_H
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stddef.h>
#include <assert.h>
#include "derivative.h"
#include <cmath>

/*
 * Default port information
 */

#ifndef TPM0_CLOCK_MASK
#ifdef SIM_SCGC6_TPM0_MASK
#define TPM0_CLOCK_MASK SIM_SCGC6_TPM0_MASK
#define TPM0_CLOCK_REG  SCGC6
#endif
#ifdef SIM_SCGC6_TPM1_MASK
#define TPM1_CLOCK_MASK SIM_SCGC6_TPM1_MASK
#define TPM1_CLOCK_REG  SCGC6
#endif
#ifdef SIM_SCGC6_TPM2_MASK
#define TPM2_CLOCK_MASK SIM_SCGC6_TPM2_MASK
#define TPM2_CLOCK_REG  SCGC6
#elif defined SIM_SCGC3_TPM2_MASK
#define TPM2_CLOCK_MASK SIM_SCGC3_TPM2_MASK
#define TPM2_CLOCK_REG  SCGC3
#endif
#ifdef SIM_SCGC6_TPM3_MASK
#define TPM3_CLOCK_MASK SIM_SCGC6_TPM3_MASK
#define TPM3_CLOCK_REG  SCGC6
#endif
#ifdef SIM_SCGC3_TPM3_MASK
#define TPM3_CLOCK_MASK SIM_SCGC3_TPM3_MASK
#define TPM3_CLOCK_REG  SCGC3
#endif
#ifdef SIM_SCGC3_TPM3_MASKC
#define TPM3_CLOCK_MASK SIM_SCGC3_TPM3_MASKC
#define TPM3_CLOCK_REG  SCGC3
#endif
#endif

namespace USBDM {

/**
 * @addtogroup TPM_Group TPM, PWM, Input capture and Output compare
 * @brief Pins used for PWM, Input capture and Output compare
 * @{
 */

/**
 * Control mode of operation of shared TPM counter
 */
enum TpmMode {
   //! Up counter: Used for left-aligned PWM, input capture and output compare modes
   TpmMode_LeftAlign   = 0,
   //! Up-down counter: Used for centre-aligned PWM 
   TpmMode_CentreAlign = TPM_SC_CPWMS_MASK,
};

/**
 * Controls basic operation of channel
 */
enum TpmChMode {
   //! Capture rising edge
   TpmChMode_InputCaptureRisingEdge  = TPM_CnSC_MS(0)|TPM_CnSC_ELS(1),
   //! Capture falling edge
   TpmChMode_InputCaptureFallingEdge = TPM_CnSC_MS(0)|TPM_CnSC_ELS(2),
   //! Capture both rising and falling edges
   TpmChMode_InputCaptureEitherEdge  = TPM_CnSC_MS(0)|TPM_CnSC_ELS(3),
   //! Output compare operation
   TpmChMode_OutputCompare           = TPM_CnSC_MS(1),
   //! Toggle pin on output compare
   TpmChMode_OutputCompareToggle     = TPM_CnSC_MS(1)|TPM_CnSC_ELS(1),
   //! Clear pin on output compare
   TpmChMode_OutputCompareClear      = TPM_CnSC_MS(1)|TPM_CnSC_ELS(2),
   //! Set pin on output compare
   TpmChMode_OutputCompareSet        = TPM_CnSC_MS(1)|TPM_CnSC_ELS(3),
   //! PWM with high-true pulses
   TpmChMode_PwmHighTruePulses       = TPM_CnSC_MS(2)|TPM_CnSC_ELS(2),
   //! PWM with low-true pulses
   TpmChMode_PwmLowTruePulses        = TPM_CnSC_MS(2)|TPM_CnSC_ELS(1),
};

/**
 * Control alignment of PWM function
 */
enum TpmClockSource {
   TpmClockSource_None        = TPM_SC_CMOD(0),  //!< Timer is disabled
   TpmClockSource_System      = TPM_SC_CMOD(1),  //!< System clock (usually the bus clock)
   TpmClockSource_External    = TPM_SC_CMOD(2),  //!< External clock provided to TPM_CLKINx pin
   TpmClockSource_Reserved    = TPM_SC_CMOD(3),  //!< Reserved
};

/**
 * Control Prescaler for TPM clock
 */
enum TpmPrescale {
   TpmPrescale_1   = TPM_SC_PS(0),  //!< Divide by 1
   TpmPrescale_2   = TPM_SC_PS(1),  //!< Divide by 2
   TpmPrescale_4   = TPM_SC_PS(2),  //!< Divide by 4
   TpmPrescale_8   = TPM_SC_PS(3),  //!< Divide by 8
   TpmPrescale_16  = TPM_SC_PS(4),  //!< Divide by 16
   TpmPrescale_32  = TPM_SC_PS(5),  //!< Divide by 32
   TpmPrescale_64  = TPM_SC_PS(6),  //!< Divide by 64
   TpmPrescale_128 = TPM_SC_PS(7),  //!< Divide by 128
};

/*
 * Enabled Timer interrupt
 */
enum TpmChannelIrq {
   TpmChannelIrq_Disable = TPM_CnSC_CHIE(0), //!< Disable interrupts from this channel
   TpmChannelIrq_Enable  = TPM_CnSC_CHIE(1), //!< Enable interrupts from this channel
};

/*
 * Enabled Timer DMA
 */
enum TpmChannelDma {
   TpmChannelDma_Disable = TPM_CnSC_DMA(0), //!< Disable DMA requests from this channel
   TpmChannelDma_Enable  = TPM_CnSC_DMA(1), //!< Enable DMA requests from this channel
};

/**
 * Type definition for TPM timer overflow interrupt call back
 */
typedef void (*TPMCallbackFunction)();

/**
 * Type definition for TPM channel interrupt call back
 *
 * @param[in] status Flags indicating interrupt source channel(s)
 */
typedef void (*TPMChannelCallbackFunction)(uint8_t status);

/**
 * Base class representing an TPM
 *
 * Example
 * @code
 * // Using TPM0
 * using Tpm0 = USBDM::TpmBase_T<TPM0_Info)>;
 *
 * // Initialise PWM with initial alignment and default clock source
 * Tpm0::configure(USBDM::TpmMode_LeftAlign);
 *
 * // Set timer period
 * Tpm0::setPeriod(500);
 * @endcode
 *
 * @tparam Info  Class describing TPM hardware instance
 */
template<class Info>
class TpmBase_T {

private:
   /**
    * This class is not intended to be instantiated
    */
   TpmBase_T() = delete;
   TpmBase_T(const TpmBase_T&) = delete;
   TpmBase_T(TpmBase_T&&) = delete;

protected:
   /** Callback to catch unhandled interrupt */
   static void unhandledCallback(uint8_t) {
      setAndCheckErrorCode(E_NO_HANDLER);
   }
   /** Callback to catch unhandled interrupt */
   static void unhandledCallback() {
      setAndCheckErrorCode(E_NO_HANDLER);
   }

public:
   /** Hardware instance pointer */
   static constexpr volatile TPM_Type* tmr      = Info::tpm;

   /** Clock register mask for peripheral */
   static constexpr volatile uint32_t *clockReg = Info::clockReg;

   /**
    * Enables clock to peripheral and configures all pins
    */
   static void __attribute__((always_inline)) enable() {
      // Configure pins
      Info::initPCRs();

      // Enable clock to peripheral
      *clockReg |= Info::clockMask;
      __DMB();
   }
   
   /**
    * Configure with settings from Configure.usbdmProject.\n
    * Includes configuring all pins
    */
   static void defaultConfigure() {
      enable();

      // Disable so immediate effect
      tmr->SC      = 0;

      // Common registers
      tmr->CNT     = 0;
      tmr->MOD     = Info::period;
      tmr->SC      = Info::sc;

      enableNvicInterrupts();
   }

   /**
    * Enables clock to peripheral and configures all pins
    * Configures main operating settings for timer
    *
    * @param[in] tpmMode        Mode of operation see USBDM::TpmMode
    * @param[in] tpmClockSource Clock source for timer
    * @param[in] tpmPrescale    Clock prescaler. Used to divide clock source before use
    */
   static void configure(
         TpmMode        tpmMode,
         TpmClockSource tpmClockSource = TpmClockSource_System,
         TpmPrescale    tpmPrescale    = TpmPrescale_128) {

      enable();
      tmr->SC = tpmMode|tpmClockSource|tpmPrescale;
   }

   /**
    * Check if TPM is enabled\n
    * Just check for clock enable and clock source selection
    *
    * @return True => enabled
    */
   static __attribute__((always_inline)) bool isEnabled() {
      return ((*clockReg & Info::clockMask) != 0) && ((tmr->SC & TPM_SC_CMOD_MASK) != 0);
   }

   /**
    * Set timer mode
    *
    * @param[in] tpmMode        Mode of operation see USBDM::TpmMode
    */
   static void setMode(TpmMode tpmMode=TpmMode_LeftAlign) {
      tmr->SC = (tmr->SC&~TPM_SC_CPWMS_MASK)|tpmMode;
   }

   /**
    * Set timer clock source
    *
    * @param[in] tpmClockSource Clock source for timer
    */
   static void setClockSource(TpmClockSource tpmClockSource=TpmClockSource_System) {
      tmr->SC = (tmr->SC&~TPM_SC_CMOD_MASK)|tpmClockSource;
   }

   /**
    *  Set timer prescaler
    *
    * @param[in] tpmPrescale    Clock prescaler. Used to divide clock source before use
    */
   static void setPrescaler(TpmPrescale tpmPrescale=TpmPrescale_128) {
      tmr->SC = (tmr->SC&~TPM_SC_PS_MASK)|tpmPrescale;
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
    * Enable/disable Timer Overflow interrupts
    *
    * @param[in] enable true to enable, false to disable
    */
   static __attribute__((always_inline)) void enableTimerOverflowInterrupts(bool enable=true) {
      if (enable) {
         tmr->SC |= TPM_SC_TOIE_MASK;
      }
      else {
         tmr->SC &= ~TPM_SC_TOIE_MASK;
      }
   }

   /**
    * Set modulo of counter
    *
    * @param[in] period Period in ticks (<65535)
    */
   void __attribute__((always_inline)) setMod(uint16_t mod) {
      tmr->MOD = mod;
   }

   /**
    * Set period
    *
    * @param[in] period Period in ticks (<65535)
    *
    * @return E_NO_ERROR       Success
    * @return E_TOO_LARGE      Failed to find suitable pre-scaler values
    *
    * @note Assumes prescale has been chosen as a appropriate value. Rudimentary range checking.
    */
   static ErrorCode setPeriodInTicks(uint32_t period) {

      // Check if CPWMS is set (affects period)
      bool centreAlign = (tmr->SC&TPM_SC_CPWMS_MASK);

      if (centreAlign) {
         // Centre-aligned period is 2*MOD value but MOD is
         // limited to 0x7FFF for sensible PWM operation

         // Halve with rounding
         period = (period+1)/2;
#ifdef DEBUG_BUILD
         if (period > 0x7FFFUL) {
            // Attempt to set too long a period
            return setErrorCode(E_TOO_LARGE);
         }
#endif
      }
      else {
         // Left-aligned period is MOD+1 value
         period = period-1;
#ifdef DEBUG_BUILD
         if (period > 0xFFFF) {
            // Attempt to set too long a period
            return setErrorCode(E_TOO_LARGE);
         }
#endif
      }

      // Disable timer so register changes are immediate
      uint8_t sc = tmr->SC;
      tmr->SC = TPM_SC_CMOD(0);

      // Change modulo
      tmr->MOD = period;

      // Restar timer
      tmr->SC  = sc;

      // OK period
      return setErrorCode(E_NO_ERROR);
   }

   /**
    * Set period
    *
    * @param[in] period Period in seconds as a float
    *
    * @note Adjusts Timer pre-scaler to appropriate value.
    *       This will affect all channels of the timer.
    *
    * @return E_NO_ERROR  => success
    * @return E_TOO_SMALL => failed to find suitable values
    * @return E_TOO_LARGE => failed to find suitable values
    */
   static ErrorCode setPeriod(float period) {
      float inputClock = Info::getInputClockFrequency();
      int prescaleFactor=1;
      int prescalerValue=0;

      // Maximum period value in ticks
      uint32_t maxPeriodInTicks = 65536;

      // Check if CPWMS is set (affects period calculation)
      if (tmr->SC&TPM_SC_CPWMS_MASK) {
         // Centre-aligned period is ~double the MOD value but MOD is
         // limited to 0x7FFF for sensible PWM operation so
         // period in ticks is limited to 2*0x7FFF
         maxPeriodInTicks = 65534;
      }
      while (prescalerValue<=7) {
         float    clock = inputClock/prescaleFactor;
         uint32_t periodInTicks   = round(period*clock);
         if (periodInTicks < Info::minimumResolution) {
            // Too short a period for 1% resolution
            return setErrorCode(E_TOO_SMALL);
         }
         if (periodInTicks <= maxPeriodInTicks) {
            tmr->SC     = (tmr->SC&~TPM_SC_PS_MASK)|TPM_SC_PS(prescalerValue);
            setPeriodInTicks(periodInTicks);
            return E_NO_ERROR;
         }
         prescalerValue++;
         prescaleFactor <<= 1;
      }
      // Too long a period
      return setErrorCode(E_TOO_LARGE);
   }

   /**
    * Set measurement period.
    * Input Capture and Output Compare will be able to operate over
    *  at least this period without overflow.
    *
    * @param[in] period Period in seconds as a float
    *
    * @note Adjusts Timer pre-scaler to appropriate value.
    *       Timer period is set to maximum.
    *       This will affect all channels of the timer.
    *
    * @return E_NO_ERROR  => success
    * @return E_TOO_SMALL => failed to find suitable values
    * @return E_TOO_LARGE => failed to find suitable values
    */
   static __attribute__((always_inline)) ErrorCode setMeasurementPeriod(float period) {
      // Try to set capture period
      ErrorCode rc = setPeriod(period);
      // Set actual period to maximum ticks in any case
      // This is the usual value for IC or OC set-up
      setPeriodInTicks(0);
      return rc;
   }
   /**
    * Get frequency of timer tick
    *
    * @return Timer frequency in Hz
    */
   static __attribute__((always_inline)) float getTickFrequency() {

      // Calculate timer prescale factor
      int prescaleFactor = 1<<((tmr->SC&TPM_SC_PS_MASK)>>TPM_SC_PS_SHIFT);

      return (float)Info::getInputClockFrequency() / prescaleFactor;
   }

   /**
    * Set approximate frequency of timer tick
    *
    * @param[in] frequency Frequency as a float
    * @param[in] tolerance Tolerance in percent
    *
    * @note Adjusts Timer pre-scaler to appropriate value.
    *       This will affect all channels
    *
    * @return E_NO_ERROR       Success
    * @return E_ILLEGAL_PARAM  Failed to find suitable pre-scaler values
    */
   static ErrorCode setTickFrequency(float frequency, float tolerance) {
      float inputClockFrequency = Info::getInputClockFrequency();

      int prescaleFactor=1;
      int prescalerValue=0;
      while (prescalerValue<=7) {
         float tickFrequency = inputClockFrequency/prescaleFactor;

         if ((100*std::abs((tickFrequency/frequency)-1)) < tolerance) {
            // Clear SC so immediate effect on prescale change
            uint32_t sc = tmr->SC&~TPM_SC_PS_MASK;
            tmr->SC     = 0;
            __DSB();
            tmr->SC     = sc|TPM_SC_PS(prescalerValue);
            return E_NO_ERROR;
         }
         prescalerValue++;
         prescaleFactor <<= 1;
      }
      // Too long a period
      return setErrorCode(E_ILLEGAL_PARAM);
   }

   /**
    * Converts a time in microseconds to number of ticks
    *
    * @param[in] time Time in microseconds
    *
    * @return Time in ticks
    *
    * @note Assumes prescale has been chosen as a appropriate value. Rudimentary range checking.
    */
   static uint32_t convertMicrosecondsToTicks(int time) {

      // Calculate period
      uint32_t tickRate = Info::getClockFrequency();
      uint64_t rv       = ((uint64_t)time*tickRate)/1000000;
#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         setErrorCode(E_TOO_LARGE);
      }
      if (rv == 0) {
         // Attempt to set too short a period
         setErrorCode(E_TOO_SMALL);
      }
#endif
      return rv;
   }

   /**
    * Converts a time in microseconds to number of ticks
    *
    * @param[in] time Time in microseconds
    *
    * @return Time in ticks
    *
    * @note Assumes prescale has been chosen as a appropriate value. Rudimentary range checking.
    */
   static uint32_t convertSecondsToTicks(float time) {

      // Calculate period
      float    tickRate = Info::getClockFrequencyF();
      uint64_t rv       = time*tickRate;
#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         setErrorCode(E_TOO_LARGE);
      }
      if (rv == 0) {
         // Attempt to set too short a period
         setErrorCode(E_TOO_SMALL);
      }
#endif
      return rv;
   }

   /**
    * Converts ticks to time in microseconds
    *
    * @param[in] tickInterval Time in ticks
    *
    * @return Time in microseconds
    *
    * @note Assumes prescale has been chosen as a appropriate value. Rudimentary range checking.
    */
   static uint32_t convertTicksToMicroseconds(int tickInterval) {

      // Calculate period
      uint32_t tickRate = Info::getClockFrequency();
      uint64_t rv       = ((uint64_t)tickInterval*1000000)/tickRate;
#ifdef DEBUG_BUILD
      if (rv > 0xFFFFUL) {
         // Attempt to set too long a period
         setErrorCode(E_TOO_LARGE);
      }
      if (rv == 0) {
         // Attempt to set too short a period
         setErrorCode(E_TOO_SMALL);
      }
#endif
      return rv;
   }

   /**
    * Converts ticks to time in seconds
    *
    * @param[in] tickInterval Time in ticks as float
    *
    * @return Time in seconds
    */
   static float __attribute__((always_inline)) convertTicksToSeconds(int tickInterval) {
      // Calculate period
      return tickInterval/Info::getClockFrequencyF();
   }


   /**
    * Get Timer count
    *
    * @return Timer count value
    */
   static __attribute__((always_inline)) uint16_t getTime() {
      return tmr->CNT;
   }

   /*
    * *****************************************************************
    *          Channel functions
    * *****************************************************************
    */
   /**
    * Set Timer event time
    *
    * @param[in] eventTime  Absolute event time i.e. value to use as timer comparison value
    * @param[in] channel    Timer channel
    */
   static __attribute__((always_inline)) void setEventTime(uint16_t eventTime, int channel) {
      tmr->CONTROLS[channel].CnV = eventTime;
   }

   /**
    * Get Timer event time
    *
    * @param[in] channel    Timer channel
    *
    * @return Absolute time of last event i.e. value from timer event register
    */
   static __attribute__((always_inline)) uint16_t getEventTime(int channel) {
      return tmr->CONTROLS[channel].CnV;
   }

   /**
    * Set Timer event time relative to current event time
    *
    * @param[in] eventTime  Event time relative to current event time (i.e. Timer channel CnV value)
    * @param[in] channel    Timer channel
    */
   static __attribute__((always_inline)) void setDeltaEventTime(uint16_t eventTime, int channel) {
      tmr->CONTROLS[channel].CnV += eventTime;
   }

   /**
    * Set Timer event time relative to current timer count value
    *
    * @param[in] eventTime  Event time relative to current time (i.e. Timer CNT value)
    * @param[in] channel    Timer channel
    */
   static __attribute__((always_inline)) void setRelativeEventTime(uint16_t eventTime, int channel) {
      tmr->CONTROLS[channel].CnV = tmr->CNT + eventTime;
   }

   /**
    * Set PWM duty cycle\n
    * Higher precision float version
    *
    * @param[in] dutyCycle  Duty-cycle as percentage (float)
    * @param[in] channel    Timer channel
    *
    * @note The actual CnV register update may be delayed by the TPM register synchronisation mechanism
    */
   static void setDutyCycle(float dutyCycle, int channel) {
      if (tmr->SC&TPM_SC_CPWMS_MASK) {
         tmr->CONTROLS[channel].CnV  = round((dutyCycle*tmr->MOD)/100.0f);
      }
      else {
         tmr->CONTROLS[channel].CnV  = round((dutyCycle*(tmr->MOD+1))/100.0f);
      }
   }

   /**
    * Set PWM duty cycle
    *
    * @param[in] dutyCycle  Duty-cycle as percentage
    * @param[in] channel    Timer channel
    *
    * @note The actual CnV register update may be delayed by the TPM register synchronisation mechanism
    */
   static void setDutyCycle(int dutyCycle, int channel) {
      if (tmr->SC&TPM_SC_CPWMS_MASK) {
         tmr->CONTROLS[channel].CnV  = (dutyCycle*tmr->MOD)/100;
      }
      else {
         tmr->CONTROLS[channel].CnV  = (dutyCycle*(tmr->MOD+1))/100;
      }
   }

   /**
    * Set PWM high time in ticks
    * Assumes value is less than period
    *
    * @param[in] highTime   PWM high time in ticks
    * @param[in] channel    Timer channel
    *
    * @return E_NO_ERROR on success
    * @return E_TOO_LARGE on success
    */
   static ErrorCode setHighTime(uint32_t highTime, int channel) {

      if (tmr->SC&TPM_SC_CPWMS_MASK) {
         // In CPWM the pulse width is doubled
         highTime = (highTime+1)/2;
      }
#ifdef DEBUG_BUILD
      if (highTime > tmr->MOD) {
         return setErrorCode(E_TOO_LARGE);
      }
#endif
      tmr->CONTROLS[channel].CnV  = highTime;
      return E_NO_ERROR;
   }

   /**
    * Set PWM high time in seconds
    *
    * @param[in] highTime   PWM high time in seconds
    * @param[in] channel    Timer channel
    *
    * @return E_NO_ERROR on success
    */
   static __attribute__((always_inline)) ErrorCode setHighTime(float highTime, int channel) {
      return setHighTime(convertSecondsToTicks(highTime), channel);
   }

};

/**
 * Template class to provide Timer callback
 */
template<class Info>
class TpmIrq_T : public TpmBase_T<Info> {

protected:
   /** Callback function for TOI ISR */
   static TPMCallbackFunction toiCallback;
   /** Callback function for Channel ISR */
   static TPMChannelCallbackFunction callback;

public:
   /**
    * IRQ handler
    */
   static void irqHandler() {
      if ((TpmBase_T<Info>::tmr->SC&(TPM_SC_TOF_MASK|TPM_SC_TOIE_MASK)) == (TPM_SC_TOF_MASK|TPM_SC_TOIE_MASK)) {
         // Clear TOI flag
         TpmBase_T<Info>::tmr->SC &= ~TPM_SC_TOF_MASK;
         toiCallback();
      }
      uint8_t status = TpmBase_T<Info>::tmr->STATUS;
      if (status) {
         // Clear flags for channel events being handled (w1c register if read)
         TpmBase_T<Info>::tmr->STATUS = status;
         callback(status);
      }
   }

   /**
    * Set TOI Callback function\n
    * Note that one callback is shared by all channels of the TPM
    *
    * @param[in] theCallback Callback function to execute on overflow interrupt.\n
    *                        nullptr to indicate none
    */
   static __attribute__((always_inline)) void setTimerOverflowCallback(TPMCallbackFunction theCallback) {
      if (theCallback == nullptr) {
         toiCallback = TpmIrq_T<Info>::unhandledCallback;
         return;
      }
      toiCallback = theCallback;
   }
   /**
    * Set channel Callback function\n
    * Note that one callback is shared by all channels of the TPM
    *
    * @param[in] theCallback Callback function to execute on channel interrupt.\n
    *                        nullptr to indicate none
    */
   static __attribute__((always_inline)) void setChannelCallback(TPMChannelCallbackFunction theCallback) {
      if (theCallback == nullptr) {
         callback = TpmBase_T<Info>::unhandledCallback;
         return;
      }
      callback = theCallback;
   }
};

template<class Info> TPMCallbackFunction         TpmIrq_T<Info>::toiCallback = TpmBase_T<Info>::unhandledCallback;
template<class Info> TPMChannelCallbackFunction  TpmIrq_T<Info>::callback    = TpmBase_T<Info>::unhandledCallback;

/**
 * Template class representing a timer channel
 *
 * Example
 * @code
 * // Instantiate the timer channel (for TPM0 channel 6)
 * using Tmr0_ch6 = USBDM::TpmChannel<TPM0Info, 6>;
 *
 * // Enable and initialise Base TPM with initial alignment
 * Tmr0_ch6::Tpm::enable();
 * Tmr0_ch6::Tpm::configure(TpmMode_LeftAlign);
 *
 * // Enable channel as PWM
 * Tmr0_ch6::enable(TpmChMode_PwmHighTruePulses);
 *
 * // Change period (in ticks)
 * Tmr0_ch6.setPeriod(500);
 *
 * // Change duty cycle (in percent)
 * Tmr0_ch6.setDutyCycle(45);
 * @endcode
 *
 * @tparam channel TPM timer channel
 */
template <class Info, int channel>
class TpmChannel_T : public TpmIrq_T<Info>, protected PcrTable_T<Info, channel>, CheckSignal<Info, channel> {

protected:
   // Allow more convenient access to template super-classes
   using PcrBase = PcrBase_T<Info::info[channel].pcrAddress>;

public:
   // Allow more convenient access to template super-classes
   using Pcr = PcrTable_T<Info, channel>;
   using Tpm = TpmIrq_T<Info>;

   // Allow access to timer hardware instance
   using TpmBase_T<Info>::tmr;

   // Make these PCR functions available
   using Pcr::setDriveMode;
   using Pcr::setDriveStrength;
   using Pcr::setFilter;
   using Pcr::setPullDevice;
   using Pcr::setSlewRate;

   // Make these shared TPM functions available
   using Tpm::setChannelCallback;
   using Tpm::setPeriod;
   using Tpm::setMeasurementPeriod;
   using Tpm::setPeriodInTicks;
   using Tpm::convertMicrosecondsToTicks;
   using Tpm::convertSecondsToTicks;
   using Tpm::convertTicksToMicroseconds;
   using Tpm::getTickFrequency;
   using Tpm::setTickFrequency;

   /**
    * Set callback for Pin IRQ
    *
    * @note There is a single callback function for all pins on the related port.
    *
    * @param[in] callback The function to call on Pin interrupt.\n
    *                     nullptr to indicate none
    */
   static __attribute__((always_inline)) void setPinCallback(PinCallbackFunction callback) {
      PcrBase::setCallback(callback);
   }

   /** Timer channel number */
   static constexpr uint32_t CHANNEL      = channel;

   /** Mask for Timer channel */
   static constexpr uint32_t CHANNEL_MASK = 1<<channel;

   /**
    * Configure channel and sets mode\n
    * Configures owning TPM with default settings from Configure.usbdmProject if not already enabled.
    *
    * @param[in] ftmChMode      Mode of operation for TPM e.g.TpmChMode_PwmHighTruePulses
    * @param[in] ftmChannelIrq  Whether to enable the interrupt function on this channel
    * @param[in] ftmChannelDma  Whether to enable the DMA function on this channel
    *
    * @note Enables TPM as well
    * @note This method has the side-effect of clearing the register update synchronisation i.e. 
    *       pending CnV register updates are discarded.
    */
   static void defaultConfigure(
         TpmChMode      ftmChMode     = TpmChMode_PwmHighTruePulses,
         TpmChannelIrq  ftmChannelIrq = TpmChannelIrq_Disable,
         TpmChannelDma  ftmChannelDma = TpmChannelDma_Disable) {

      if (!Tpm::isEnabled()) {
         // Enable parent TPM if needed
         Tpm::configure();
      }
      Tpm::tmr->CONTROLS[channel].CnSC = ftmChMode|ftmChannelIrq|ftmChannelDma;
   }

   /**
    * Configure channel and sets channel mode\n
    * Doesn't affect shared settings of owning Timer
    *
    * @param[in] ftmChMode      Mode of operation for channel
    * @param[in] ftmChannelIrq  Whether to enable the interrupt function on this channel
    * @param[in] ftmChannelDma  Whether to enable the DMA function on this channel
    *
    * @note This method has the side-effect of clearing the register update synchronisation i.e.
    *       pending CnV register updates are discarded.
    */
   static __attribute__((always_inline)) void configure(
         TpmChMode      ftmChMode     = TpmChMode_PwmHighTruePulses,
         TpmChannelIrq  ftmChannelIrq = TpmChannelIrq_Disable,
         TpmChannelDma  ftmChannelDma = TpmChannelDma_Disable) {

#ifdef DEBUG_BUILD
      // Check that owning FTM has been enabled
      assert(Tpm::isEnabled());
#endif

      Tpm::tmr->CONTROLS[channel].CnSC = ftmChMode|ftmChannelIrq|ftmChannelDma;
   }

   /**
    * Enable or disable interrupt from this channel\n
    * Note: It is necessary to enableNvicInterrupts() as well
    *
    * @param[in] enable  True => enable, False => disable
    *
    * @note This method has the side-effect of clearing the register update synchronisation i.e.\n
    *       pending CnV register updates are discarded.
    */
   static __attribute__((always_inline)) void enableInterrupts(bool enable=true) {
      if (enable) {
         Tpm::tmr->CONTROLS[channel].CnSC |= TPM_CnSC_CHIE_MASK;
      }
      else {
         Tpm::tmr->CONTROLS[channel].CnSC &= ~TPM_CnSC_CHIE_MASK;
      }
   }

   /**
    * Enable or disable DMA requests from this channel\n
    *
    * @param[in] enable  True => enable, False => disable
    *
    * @note This method has the side-effect of clearing the register update synchronisation i.e.\n
    *       pending CnV register updates are discarded.
    */
   static __attribute__((always_inline)) void enableDma(bool enable=true) {
      if (enable) {
         Tpm::tmr->CONTROLS[channel].CnSC |= TPM_CnSC_DMA_MASK;
      }
      else {
         Tpm::tmr->CONTROLS[channel].CnSC &= ~TPM_CnSC_DMA_MASK;
      }
   }

   /**
    * Enable/disable interrupts in NVIC
    *
    * @param[in] enable true to enable, false to disable
    */
   static __attribute__((always_inline)) void enableNvicInterrupts(bool enable=true) {
      TpmIrq_T<Info>::enableNvicInterrupts(enable);
   }

   /**
    * Enable/disable Pin interrupts in NVIC
    *
    * @param[in] enable true => enable, false => disable
    */
   static __attribute__((always_inline)) void enablePinNvicInterrupts(bool enable=true) {
      Pcr::enableNvicInterrupts(enable);
   }

   /**
    * Set Pin Control Register Value (apart from pin multiplexor value)
    *
    * @param[in] pcrValue PCR value to set
    */
   static __attribute__((always_inline)) void setPCR(PcrValue pcrValue) {
      Pcr::setPCR((pcrValue&~PORT_PCR_MUX_MASK)|(Info::info[channel].pcrValue&PORT_PCR_MUX_MASK));
   }

   /**
    * Set Pin Control Register (PCR) value
    *
    * @param[in] pinPull          One of PinPull_None, PinPull_Up, PinPull_Down (defaults to PinPull_None)
    * @param[in] pinDriveStrength One of PinDriveStrength_Low, PinDriveStrength_High (defaults to PinDriveLow)
    * @param[in] pinDriveMode     One of PinDriveMode_PushPull, PinDriveMode_OpenDrain (defaults to PinPushPull)
    * @param[in] pinIrq           One of PinIrq_None, etc (defaults to PinIrq_None)
    * @param[in] pinFilter        One of PinFilter_None, PinFilter_Passive (defaults to PinFilter_None)
    * @param[in] pinSlewRate      One of PinSlewRate_Slow, PinSlewRate_Fast (defaults to PinSlewRate_Fast)
    * @param[in] pinMux           One of PinMux_Analogue, PinMux_Gpio etc (defaults to TPM selection value)
    */
   static __attribute__((always_inline)) void setPCR(
         PinPull           pinPull,
         PinDriveStrength  pinDriveStrength  = PinDriveStrength_Low,
         PinDriveMode      pinDriveMode      = PinDriveMode_PushPull,
         PinIrq            pinIrq            = PinIrq_None,
         PinFilter         pinFilter         = PinFilter_None,
         PinSlewRate       pinSlewRate       = PinSlewRate_Fast,
         PinMux            pinMux            = (PinMux)(Info::info[channel].pcrValue&PORT_PCR_MUX_MASK)
         ) {
      Pcr::setPCR(pinPull,pinDriveStrength,pinDriveMode,pinIrq,pinFilter,pinSlewRate,pinMux);
   }
   /**
    * Set PWM high time in ticks\n
    * Assumes value is less than period
    *
    * @param[in] highTime   PWM high time in ticks
    *
    * @return E_NO_ERROR on success
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) ErrorCode setHighTime(uint32_t highTime) {
      return Tpm::setHighTime(highTime, channel);
   }

   /**
    * Set PWM high time in seconds\n
    * Higher precision float version
    *
    * @param[in] highTime   PWM high time in seconds
    *
    * @return E_NO_ERROR on success
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) ErrorCode setHighTime(float highTime) {
      return Tpm::setHighTime(highTime, channel);
   }
   /**
    * Set PWM duty cycle
    *
    * @param[in] dutyCycle  Duty-cycle as percentage
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) void setDutyCycle(int dutyCycle) {
      Tpm::setDutyCycle(dutyCycle, channel);
   }

   /**
    * Set PWM duty cycle
    *
    * @param[in] dutyCycle  Duty-cycle as percentage
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) void setDutyCycle(float dutyCycle) {
      Tpm::setDutyCycle(dutyCycle, channel);
   }

   /**
    * Set Timer event time
    *
    * @param[in] eventTime  Event time relative to current event time (i.e. Timer channel CnV value)
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) void setDeltaEventTime(uint16_t eventTime) {
      Tpm::setDeltaEventTime(eventTime, channel);
   }

   /**
    * Set Timer event time relative to current timer count value
    *
    * @param[in] eventTime  Event time relative to current time (i.e. Timer CNT value)
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) void setRelativeEventTime(uint16_t eventTime) {
      Tpm::setRelativeEventTime(eventTime, channel);
   }

   /**
    * Set Absolute Timer event time
    *
    * @param[in] eventTime  Absolute event time i.e. value to use as timer comparison value
    *
    * @note The actual CnV register update will be delayed by the TPM register synchronisation mechanism
    */
   static __attribute__((always_inline)) void setEventTime(uint16_t eventTime) {
      Tpm::setEventTime(eventTime, channel);
   }

   /**
    * Get Absolute Timer event time
    *
    * @return Absolute time of last event i.e. value from timer event register
    */
   static __attribute__((always_inline)) uint16_t getEventTime() {
      return Tpm::getEventTime(channel);
   }

   /**
    * Clear interrupt flag on channel
    */
   static __attribute__((always_inline)) void clearInterruptFlag(void) {
      Tpm::tmr->CONTROLS[channel].CnSC &= ~TPM_CnSC_CHF_MASK;
   }
};

#ifdef USBDM_TPM0_IS_DEFINED
/**
 * Template class representing a Timer channel
 *
 * Example
 * @code
 * // Instantiate the timer channel (for TPM0 channel 6)
 * using Tmr0_ch6 = USBDM::Tpm0Channel<6>;
 *
 * // Enable and initialise Base TPM with initial alignment
 * Tmr0_ch6::Tpm::configure(TpmMode_LeftAlign);
 *
 * // Change timer period (in ticks) (affects ALL channels of timer)
 * Tmr0_ch6.Tpm::setPeriod(500);
 *
 * // Configure channel as PWM
 * Tmr0_ch6::configure(TpmChMode_PwmHighTruePulses);
 *
 * // Change duty cycle (in percent)
 * Tmr0_ch6.setDutyCycle(45);
 * @endcode
 *
 * @tparam channel Timer channel
 */
template <int channel>
class Tpm0Channel : public TpmChannel_T<Tpm0Info, channel> {};

/**
 * Class representing TPM0
 */
using Tpm0 = TpmIrq_T<Tpm0Info>;
#endif

#ifdef USBDM_TPM1_IS_DEFINED
/**
 * Template class representing a TPM1 timer channel
 *
 * Refer @ref Tpm0Channel
 *
 * @tparam channel Timer channel
 */
template <int channel>
class Tpm1Channel : public TpmChannel_T<Tpm1Info, channel> {};

/**
 * Class representing TPM1
 */
using Tpm1 = TpmIrq_T<Tpm1Info>;
#endif

#ifdef USBDM_TPM2_IS_DEFINED
/**
 * Template class representing a TPM2 timer channel
 *
 * Refer @ref Tpm0Channel
 *
 * @tparam channel Timer channel
 */
template <int channel>
class Tpm2Channel : public TpmChannel_T<Tpm2Info, channel> {};

/**
 * Class representing TPM2
 */
using Tpm2 = TpmIrq_T<Tpm2Info>;
#endif

#ifdef USBDM_TPM3_IS_DEFINED
/**
 * Template class representing a TPM3 timer channel
 *
 * Refer @ref Tpm0Channel
 *
 * @tparam channel Timer channel
 */
template <int channel>
class Tpm3Channel : public TpmBase_T<Tpm3Info>, CheckSignal<Tpm2Info, channel> {

   /**
    * Class representing TPM3
    */
   using Tpm3 = TpmIrq_T<Tpm3Info>;
#endif

   /**
    * @}
    */

} // End namespace USBDM

#endif /* HEADER_TPM_H */


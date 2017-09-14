/**
 * @file     pit.h (180.ARM_Peripherals/Project_Headers/pit-MK.h)
 *
 * @brief    Programmable Interrupt Timer interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */

#ifndef INCLUDE_USBDM_PIT_H_
#define INCLUDE_USBDM_PIT_H_
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
 * @addtogroup PIT_Group PIT, Programmable Interrupt Timer
 * @brief Abstraction for Programmable Interrupt Timer
 * @{
 */
/**
 * Type definition for PIT interrupt call back
 */
typedef void (*PitCallbackFunction)(void);

/**
 * Control PIT operation in debug mode (suspended for debugging)
 */
enum PitDebugMode {
   PitDebugMode_Run  = PIT_MCR_FRZ(0),  //!< PIT continues to run in debug mode
   PitDebugMode_Stop = PIT_MCR_FRZ(1),  //!< PIT stops in debug mode
};

/**
 * Enable the PIT interrupts
 */
enum PitChannelIrq {
   PitChannelIrq_Disable  = PIT_TCTRL_TIE(0),  //!< PIT channel interrupt disabled
   PitChannelIrq_Enable   = PIT_TCTRL_TIE(1),  //!< PIT channel interrupt disabled
};

/**
 * Enable the PIT channel
 */
enum PitChannelEnable {
   PitChannelEnable_Disable  = PIT_TCTRL_TEN(0),  //!< PIT channel disabled
   PitChannelEnable_Enable   = PIT_TCTRL_TEN(1),  //!< PIT channel enabled
};

/**
 * @brief Class representing a Programmable Interrupt  Timer
 *
 * <b>Example</b>
 * @code
 *
 * @endcode
 */
template<class Info>
class PitBase_T {

private:
   /**
    * This class is not intended to be instantiated
    */
   PitBase_T() = delete;
   PitBase_T(const PitBase_T&) = delete;
   PitBase_T(PitBase_T&&) = delete;

protected:
   /** Default TCTRL value for timer channel */
   static constexpr uint32_t PIT_TCTRL_DEFAULT_VALUE = (PIT_TCTRL_TEN_MASK);

   /** Callback to catch unhandled interrupt */
   static void unhandledCallback() {
      setAndCheckErrorCode(E_NO_HANDLER);
   }
   
public:
   /**
    * Enable/disable channel interrupts
    *
    * @param[in]  channel Channel being modified
    * @param[in]  enable  True => enable, False => disable
    */
   static void enableInterrupts(unsigned channel, bool enable=true) {
      if (enable) {
         pit->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;
      }
      else {
         pit->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK;
      }
   }

protected:
   /** Pointer to hardware */
   static constexpr volatile PIT_Type *pit       = Info::pit;

   /** Pointer to clock register */
   static constexpr volatile uint32_t *clockReg  = Info::clockReg;

public:
   /**
    * Basic enable of PIT
    */
   static void enable() {
      // Enable clock
      *clockReg |= Info::clockMask;
      __DMB();
   }

   /**
    *  Enable the PIT with default settings.
    *  All channels are enabled with default settings.
    */
   static void defaultConfigure() {
      enable();

      // Enable timer
      pit->MCR = Info::mcr;
      for (unsigned i=0; i<Info::irqCount; i++) {
         configureChannelInTicks(i, Info::pit_ldval);
         enableNvicInterrupts(i, false, Info::irqLevel);
      }
   }

   /**
    *  Enables and configures the PIT
    *
    *  @param[in]  pitDebugMode  Determined whether the PIT halts when suspended during debug
    */
   static void configure(PitDebugMode pitDebugMode=PitDebugMode_Stop) {
      enable();
      pit->MCR = pitDebugMode|PIT_MCR_MDIS(0); // MDIS cleared => enabled!
   }

   /**
    *   Disable the PIT (all channels)
    */
   static void disable() {
      pit->MCR = PIT_MCR_MDIS(1);
      *clockReg &= ~Info::clockMask;
   }

   /**
    * Enable/disable interrupts in NVIC
    *
    * @param[in]  channel   Channel being modified
    * @param[in]  enable    True => enable, False => disable
    * @param[in]  priority  Interrupt priority
    *
    * @return E_NO_ERROR on success
    */
   static ErrorCode enableNvicInterrupts(unsigned channel, bool enable=true, uint32_t priority=NvicPriority_Normal) {
      static const IRQn_Type irqNums[] = {
            Info::irqNums[0], Info::irqNums[1], Info::irqNums[2], Info::irqNums[3],
      };
      if (channel>=Info::irqCount) {
         setAndCheckErrorCode(E_ILLEGAL_PARAM);
      }
      IRQn_Type irqNum = irqNums[channel];
      if (enable) {
         // Enable interrupts
         NVIC_EnableIRQ(irqNum);

         // Set priority level
         NVIC_SetPriority(irqNum, priority);
      }
      else {
         // Disable interrupts
         NVIC_DisableIRQ(irqNum);
      }
      return E_NO_ERROR;
   }
   
   /**
    *  Enable/Disable the PIT channel
    *
    *  @param[in]  channel   Channel to enable
    *  @param[in]  enable    Controls whether channel is enabled or disabled
    */
   static void enableChannel(const uint8_t channel, bool enable=true) {
      if (enable) {
         pit->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
      }
      else {
         pit->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;
      }
   }

   /**
    *  Configure the PIT channel
    *
    *  @param[in]  channel           Channel to configure
    *  @param[in]  interval          Interval in timer ticks (usually bus clock period)
    *  @param[in]  pitChannelIrq     Whether to enable interrupts
    *
    *  @note NVIC is configured
    */
   static void configureChannelInTicks(
         uint8_t           channel,
         uint32_t          interval,
         PitChannelIrq     pitChannelIrq=PitChannelIrq_Disable) {

      pit->CHANNEL[channel].LDVAL = interval-1;
      pit->CHANNEL[channel].TCTRL = pitChannelIrq|PIT_TCTRL_TEN(1);
      pit->CHANNEL[channel].TFLG  = PIT_TFLG_TIF_MASK;

      enableNvicInterrupts(channel);
   }

   /**
    *  Configure the PIT channel
    *
    *  @param[in]  channel           Channel to configure
    *  @param[in]  interval          Interval in seconds
    *  @param[in]  pitChannelIrq     Whether to enable interrupts
    *
    *  @note NVIC is configured
    */
   static void configureChannel(
         uint8_t           channel,
         float             interval,
         PitChannelIrq     pitChannelIrq=PitChannelIrq_Disable) {

      pit->CHANNEL[channel].LDVAL = round((interval*PitInfo::getClockFrequency())-1);
      pit->CHANNEL[channel].TCTRL = pitChannelIrq|PIT_TCTRL_TEN(1);
      pit->CHANNEL[channel].TFLG  = PIT_TFLG_TIF_MASK;

      enableNvicInterrupts(channel);
   }

   /**
    * Set period in seconds
    *
    * @param[in]  channel Channel being modified
    * @param[in]  interval Interval in seconds
    */
   static void setPeriod(unsigned channel, float interval) {
      pit->CHANNEL[channel].LDVAL = round((interval*PitInfo::getClockFrequency())-1);
   }

   /**
    * Set period in seconds
    *
    * @param[in]  channel Channel being modified
    * @param[in]  interval Interval in ticks
    */
   static void setPeriodInTicks(unsigned channel, uint32_t interval) {
      pit->CHANNEL[channel].LDVAL = interval-1;
   }

   /**
    *   Disable the PIT channel
    *
    *   @param[in]  channel Channel to disable
    */
   static void disableChannel(uint8_t channel) {

      // Disable timer channel
      pit->CHANNEL[channel].TCTRL = 0;

      // Disable timer interrupts
      enableNvicInterrupts(channel, false);
   }

   /**
    *  Use a PIT channel to implement a busy-wait delay
    *
    *  @param[in]  channel   Channel to use
    *  @param[in]  interval  Interval to wait in timer ticks (usually bus clock period)
    *
    *  @note Function doesn't return until interval has expired
    */
   static void delayInTicks(uint8_t channel, uint32_t interval) {
      configureChannelInTicks(channel, interval);
      while (pit->CHANNEL[channel].TFLG == 0) {
         __NOP();
      }
      disableChannel(channel);
   }

   /**
    *  Use a PIT channel to implement a busy-wait delay
    *
    *  @param[in]  channel   Channel to use
    *  @param[in]  interval  Interval to wait as a float
    *
    *  @note Function doesn't return until interval has expired
    */
   static void delay(uint8_t channel, float interval) {
      configureChannel(channel, interval);
      while (pit->CHANNEL[channel].TFLG == 0) {
         __NOP();
      }
      disableChannel(channel);
   }
};

/**
 * Class representing a PIT channel
 *
 * @tparam channel Timer channel number
 */
template <class Info, int channel>
class PitChannel_T : public PitBase_T<Info> {

protected:
   static PitCallbackFunction callback;

public:

   /** Timer channel number */
   static constexpr int CHANNEL = channel;

   /**
    * Set interrupt callback
    *
    * @param[in]  callbackFunction  Function to call from stub ISR
    */
   static void setCallback(PitCallbackFunction callbackFunction) {
      if (callbackFunction == nullptr) {
         callbackFunction = PitBase_T<Info>::unhandledCallback;
         enableInterrupts(false);
      }
      callback = callbackFunction;
   }

   /** PIT interrupt handler -  Calls PIT callback */
   static void irqHandler() {
      // Clear interrupt flag
      PitBase_T<Info>::pit->CHANNEL[channel].TFLG = PIT_TFLG_TIF_MASK;
      callback();
   }

   /**
    *  Configure the PIT channel
    *
    *  @param[in]  interval          Interval in timer ticks (usually bus clock)
    *  @param[in]  pitChannelIrq     Whether to enable interrupts
    */
   static void __attribute__((always_inline)) configureInTicks(
         uint32_t          interval,
         PitChannelIrq     pitChannelIrq=PitChannelIrq_Disable) {

      PitBase_T<Info>::configureChannelInTicks(channel, interval, pitChannelIrq);
   }

   /**
    *  Configure the PIT channel
    *
    *  @param[in]  interval          Interval in seconds
    *  @param[in]  pitChannelIrq     Whether to enable interrupts
    */
   static void __attribute__((always_inline)) configure(
         float             interval,
         PitChannelIrq     pitChannelIrq=PitChannelIrq_Disable) {

      PitBase_T<Info>::configureChannel(channel, interval, pitChannelIrq);
   }

   /**
    * Set period in seconds
    *
    * @param[in]  interval Interval in seconds
    */
   static void __attribute__((always_inline)) setPeriod(float interval) {
      PitBase_T<Info>::setPeriod(channel, interval);
   }

   /**
    * Set period in seconds
    *
    * @param[in]  interval Interval in ticks
    */
   static void __attribute__((always_inline)) setPeriodInTicks(uint32_t interval) {
      PitBase_T<Info>::setPeriodInTicks(channel, interval);
   }

   /**
    *   Enable/Disable the PIT channel
    */
   static void __attribute__((always_inline)) enable(bool enable=true) {
      PitBase_T<Info>::enableChannel(channel, enable);
   }

   /**
    * Enable/disable channel interrupts
    *
    * @param[in]  enable  True => enable, False => disable
    */
   static void __attribute__((always_inline)) enableInterrupts(bool enable=true) {
      PitBase_T<Info>::enableInterrupts(channel, enable);
   }

   /**
    * Enable/disable interrupts in NVIC
    *
    * @param[in]  enable      True => enable, False => disable
    * @param[in]  priority    Interrupt priority
    *
    * @return E_NO_ERROR on success
    */
   static ErrorCode __attribute__((always_inline)) enableNvicInterrupts(bool enable=true, uint32_t priority=NvicPriority_Normal) {
      return PitBase_T<Info>::enableNvicInterrupts(channel, enable, NvicPriority_Normal);
   }

};

/**
 * Callback for programmatically set handlers
 */
template <class Info, int channel> PitCallbackFunction PitChannel_T<Info, channel>::callback;

#ifdef PIT
/**
 * @brief class representing the PIT
 */
class Pit : public PitBase_T<PitInfo> {};

/**
 * @brief Class representing a PIT channel
 *
 * @tparam channel Channel number
 */
template <int channel>
class PitChannel : public PitChannel_T<PitInfo, channel> {};
#endif

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_PIT_H_ */

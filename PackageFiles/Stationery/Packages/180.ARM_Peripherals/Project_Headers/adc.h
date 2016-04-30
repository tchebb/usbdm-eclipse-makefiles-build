/**
 * @file     adc.h
 * @brief    ADC routines
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef HEADER_ADC_H
#define HEADER_ADC_H

#include <stddef.h>
#include <assert.h>
#include "derivative.h"

/*
 * Default port information
 */
#ifndef FIXED_PORT_CLOCK_REG
#define FIXED_PORT_CLOCK_REG SCGC5
#endif

#ifndef FIXED_ADC_FN
#define FIXED_ADC_FN    (0)
#endif

#ifndef ADC0_CLOCK_MASK
#ifdef SIM_SCGC6_ADC0_MASK
#define ADC0_CLOCK_MASK SIM_SCGC6_ADC0_MASK
#define ADC0_CLOCK_REG  SCGC6
#endif
#ifdef SIM_SCGC3_ADC1_MASK
#define ADC1_CLOCK_MASK SIM_SCGC3_ADC1_MASK
#define ADC1_CLOCK_REG  SCGC3
#endif
#ifdef SIM_SCGC6_ADC1_MASK
#define ADC1_CLOCK_MASK SIM_SCGC6_ADC1_MASK
#define ADC1_CLOCK_REG  SCGC6
#endif
#ifdef SIM_SCGC6_ADC2_MASK
#define ADC2_CLOCK_MASK SIM_SCGC6_ADC2_MASK
#define ADC2_CLOCK_REG  SCGC6
#endif
#endif

namespace USBDM {

/**
 * @addtogroup AnalogueIO_Group Analogue Input
 * @{
 */

/**
 * PCR multiplexor value for analogue function
 */
static constexpr uint32_t    ADC_PORT_FN = PORT_PCR_MUX(FIXED_ADC_FN);
/**
 * Default PCR value for pins used as GPIO (including multiplexor value)
 */
static constexpr uint32_t    ADC_DEFAULT_PCR = ADC_PORT_FN;

/**
 * ADC Resolutions for use with AnalogueIO::setMode()
 */
enum Adc_Resolution {
   resolution_8bit_se    = ADC_CFG1_MODE(0),
   resolution_10bit_se   = ADC_CFG1_MODE(2),
   resolution_12bit_se   = ADC_CFG1_MODE(1),
   resolution_16bit_se   = ADC_CFG1_MODE(3),
   resolution_9bit_diff  = ADC_CFG1_MODE(0),
   resolution_11bit_diff = ADC_CFG1_MODE(2),
   resolution_12bit_diff = ADC_CFG1_MODE(1),
   resolution_16bit_diff = ADC_CFG1_MODE(3),
};

/**
 * Type definition for ADC interrupt call back
 */
typedef void (*ADCCallbackFunction)(void);

/**
 * Template class representing an ADC
 *
 * Example
 * @code
 *  // Instantiate ADC channel adc0_se8
 *  const AdcBase_T<adc0Info> Adc0;
 *
 *  // Initialise ADC
 *  Adc0.setMode(resolution_16bit_se);
 *  @endcode
 *
 * @tparam info Table of information describing ADC
 */
template<class Info>
class AdcBase_T {

private:
   static constexpr volatile ADC_Type *adc      = reinterpret_cast<volatile ADC_Type *>(Info::basePtr);
   static constexpr volatile uint32_t *clockReg = reinterpret_cast<volatile uint32_t *>(Info::clockReg);

public:
   /**
    * Set port pin as analogue input
    *
    * @param mode Mode for converter (currently only resolution)
    *
    * @note This initialises the ADC
    */
   static void setMode(uint32_t mode = resolution_16bit_se) {
      // Configure pins
      Info::initPCRs();

      // Enable clock to ADC
      *clockReg  |= Info::clockMask;

      __DMB();

      // Configure ADC for software triggered conversion
      adc->CFG1 = Info::CFG1|mode;
      adc->SC2  = 0;
      adc->CFG2 = Info::CFG1|ADC_CFG2_MUXSEL_MASK; // Choose 'b' channels
   }

   /**
    * Initiates a conversion and waits for it to complete
    *
    * @return - the result of the conversion
    */
   static int readAnalogue(int channel) {
      // Trigger conversion
      adc->SC1[0] = ADC_SC1_ADCH(channel)|Info::SC1;

      while ((adc->SC1[0]&ADC_SC1_COCO_MASK) == 0) {
         __asm__("nop");
      }
      return (int)adc->R[0];
   };
};

/**
 * Template class to provide ADC callback
 */
template<class Info>
class AdcIrq_T : public AdcBase_T<Info> {

protected:
   /** Callback function for ISR */
   static ADCCallbackFunction callback;

public:
   /**
    * IRQ handler
    */
   static void irqHandler() {
      if (callback != 0) {
         callback();
      }
   }

   /**
    * Set callback function
    *
    * @param callback Callback function to execute on interrupt
    */
   void setCallback(ADCCallbackFunction callback) {
      AdcIrq_T::callback = callback;
   }
};

template<class Info> ADCCallbackFunction AdcIrq_T<Info>::callback = 0;

#ifdef USBDM_ADC0_IS_DEFINED
/**
 *
 * Template class representing a ADC0 channel
 *
 * Example
 * @code
 * // Instantiate the ADC channel (for ADC0 channel 6)
 * using Adc0_ch6 = USBDM::Adc0Channel<6>;
 *
 * // Set ADC resolution
 * Adc0_ch6.setMode(resolution_16bit_se);
 *
 * // Read ADC value
 * uint32_t value = Adc0_ch6.readAnalogue();
 * @endcode
 *
 * @tparam channel ADC channel
 */
template<int channel>
class Adc0Channel : public AdcBase_T<Adc0Info>, CheckSignal<Adc0Info, channel> {

public:
   /**
    * Initiates a conversion and waits for it to complete
    *
    * @return - the result of the conversion
    */
   static int readAnalogue() {
      return AdcBase_T::readAnalogue(channel);
   };
};

/**
 * Class representing ADC0
 */
using Adc0 = AdcIrq_T<Adc0Info>;
#endif

#ifdef USBDM_ADC1_IS_DEFINED
/**
 *
 * Template class representing a ADC0 channel
 *
 * Refer @ref Adc0Channel
 *
 * @tparam channel ADC channel
 */
template<int channel>
class Adc1Channel : public AdcBase_T<Adc1Info>, CheckSignal<Adc1Info, channel> {

#ifdef DEBUG_BUILD
   static_assert((channel<Adc1Info::NUM_SIGNALS), "Adc0Channel: Non-existent ADC channel - Modify Configure.usbdm");
   static_assert((channel>=Adc1Info::NUM_SIGNALS)||(Adc1Info::info[channel].gpioBit != UNMAPPED_PCR), "Adc0Channel: ADC channel is not mapped to a pin - Modify Configure.usbdm");
   static_assert((channel>=Adc1Info::NUM_SIGNALS)||(Adc1Info::info[channel].gpioBit != INVALID_PCR),  "Adc0Channel: ADC channel doesn't exist in this device/package");
   static_assert((channel>=Adc1Info::NUM_SIGNALS)||((Adc1Info::info[channel].gpioBit == UNMAPPED_PCR)||(Adc1Info::info[channel].gpioBit == INVALID_PCR)||(Adc1Info::info[channel].gpioBit >= 0)), "Adc0Channel: Illegal ADC channel");
#endif

   /**
    * Initiates a conversion and waits for it to complete
    *
    * @return - the result of the conversion
    */
   static int readAnalogue() {
      return readAnalogue(channel);
   };
};
/**
 * Class representing ADC1
 */
using Adc1 = AdcIrq_T<Adc1Info>;
#endif

/**
 * @}
 */
 
} // End namespace USBDM

#endif /* HEADER_ADC_H */

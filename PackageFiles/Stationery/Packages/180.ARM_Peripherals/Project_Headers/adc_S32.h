/**
 * @file     adc.h (180.ARM_Peripherals/Project_Headers/adc_S32.h)
 * @brief    ADC routines
 *
 * @version  V4.12.1.80
 * @date     26 December 2018
 */
#ifndef HEADER_ADC_H
#define HEADER_ADC_H
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <climits>
#include <cstddef>
#include "derivative.h"

/*
 * Default port information
 */
#ifndef FIXED_PORT_CLOCK_REG
#define FIXED_PORT_CLOCK_REG SCGC5
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
 * @addtogroup ADC_Group ADC, Analogue Input
 * @brief Abstraction for Analogue Input
 * @{
 */

/**
 * Default PCR value for pins used as GPIO (including multiplexor value)
 */
static constexpr PcrValue ADC_DEFAULT_PCR = pcrValue(
      PinPull_None, PinDriveStrength_Low, PinDriveMode_PushPull, PinAction_None, PinFilter_None, PinSlewRate_Fast, PinMux_Analogue);

/**
 * ADC Resolutions.
 * The resolutions available vary with single-ended/differential modes\n
 * Note the equivalence between modes e.g. 8-bit-se = 9-bit-diff
 */
enum AdcResolution {
   AdcResolution_8bit_se_9bit_diff   = ADC_CFG1_MODE(0),  //!<  8-bit unsigned/9-bit signed
   AdcResolution_10bit_se_11bit_diff = ADC_CFG1_MODE(2),  //!< 10-bit unsigned/11-bit signed
   AdcResolution_12bit_se_13bit_diff = ADC_CFG1_MODE(1),  //!< 12-bit unsigned/13-bit signed
   AdcResolution_16bit               = ADC_CFG1_MODE(3),  //!< 16-bit unsigned/unsigned
   AdcResolution_8bit_se             = ADC_CFG1_MODE(0),  //!<  8-bit unsigned for use with single-ended mode
   AdcResolution_10bit_se            = ADC_CFG1_MODE(2),  //!< 10-bit unsigned for use with single-ended mode
   AdcResolution_12bit_se            = ADC_CFG1_MODE(1),  //!< 12-bit unsigned for use with single-ended mode
   AdcResolution_16bit_se            = ADC_CFG1_MODE(3),  //!< 16-bit unsigned for use with single-ended mode
   AdcResolution_9bit_diff           = ADC_CFG1_MODE(0),  //!<  9-bit signed for use with differential mode
   AdcResolution_11bit_diff          = ADC_CFG1_MODE(2),  //!< 11-bit signed for use with differential mode
   AdcResolution_13bit_diff          = ADC_CFG1_MODE(1),  //!< 12-bit signed for use with differential mode
   AdcResolution_16bit_diff          = ADC_CFG1_MODE(3),  //!< 16-bit signed for use with differential mode
};

/**
 * ADC Averaging
 */
enum AdcAveraging {
   AdcAveraging_off = ADC_SC3_AVGE(0),                  //!< No averaging - single conversion
   AdcAveraging_4   = ADC_SC3_AVGE(1)|ADC_SC3_AVGS(0),  //!< Average across 4 conversions
   AdcAveraging_8   = ADC_SC3_AVGE(1)|ADC_SC3_AVGS(1),  //!< Average across 8 conversions
   AdcAveraging_16  = ADC_SC3_AVGE(1)|ADC_SC3_AVGS(2),  //!< Average across 16 conversions
   AdcAveraging_32  = ADC_SC3_AVGE(1)|ADC_SC3_AVGS(3),  //!< Average across 32 conversions
   AdcAveraging_Cal = AdcAveraging_32|ADC_SC3_CAL(1),   //!< Average across 32 conversions + start calibration
};

/**
 * ADC input clock source.
 */
enum AdcClockSource {
   AdcClockSource_PccAdcClk  = ADC_CFG1_ADICLK(0), //!< Clock from PCC_ADCn multiplexor
   AdcClockSource_Reserved1  = ADC_CFG1_ADICLK(1), //!< Reserved
   AdcClockSource_Reserved2  = ADC_CFG1_ADICLK(2), //!< Reserved
   AdcClockSource_Reserved3  = ADC_CFG1_ADICLK(3), //!< Reserved
   AdcClockSource_Default    = AdcClockSource_PccAdcClk
};

/**
 * ADC clock divider
 */
enum AdcClockDivider {
   AdcClockDivider_1       = ADC_CFG1_ADIV(0), //!< Clock divide by 1
   AdcClockDivider_2       = ADC_CFG1_ADIV(1), //!< Clock divide by 2
   AdcClockDivider_4       = ADC_CFG1_ADIV(2), //!< Clock divide by 3
   AdcClockDivider_8       = ADC_CFG1_ADIV(3), //!< Clock divide by 4
};

/**
 * Controls whether an interrupt is triggered at the end of a conversion
 */
enum AdcInterrupt {
   AdcInterrupt_Disabled = ADC_SC1_AIEN(0), //!< No interrupt on conversion complete
   AdcInterrupt_Enabled  = ADC_SC1_AIEN(1), //!< Interrupt on conversion complete
};

/**
 * Select the pretrigger
 */
enum AdcPretrigger {
   AdcPretrigger_0  = 0, //!< Use pretrigger A = SC1[0]/R[0]
   AdcPretrigger_1  = 1, //!< Use pretrigger B = SC1[1]/R[1]
};

#ifdef ADC_SC2_DMAEN
/**
 * Selects DMA operation
 */
enum AdcDma {
   AdcDma_Disabled = ADC_SC2_DMAEN(0), //!< DMA disabled
   AdcDma_Enabled  = ADC_SC2_DMAEN(1), //!< DMA enabled
};
#endif

/**
 * Selects between single and continuous conversions
 */
enum AdcContinuous {
   AdcContinuous_Disabled = ADC_SC3_ADCO(0), //!< Normal operation i.e. a single conversion will be done when triggered.
   AdcContinuous_Enabled  = ADC_SC3_ADCO(1), //!< Continuous conversions enabled i.e. a continuous sequence of conversion will e triggered.
};

/**
 * Controls the ADC compare function
 */
enum AdcCompare {
   AdcCompare_Disabled              = ADC_SC2_ACFE(0),                                          //!< Comparisons disabled
   AdcCompare_LessThan              = ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(0)|ADC_SC2_ACREN(0),        //!< ADC_value < low
   AdcCompare_GreaterThanOrEqual    = ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(1)|ADC_SC2_ACREN(0),        //!< ADC_value >= low
   AdcCompare_OutsideRangeExclusive = (0<<8)|ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(0)|ADC_SC2_ACREN(1), //!< (ADC_value < low) || (ADC_value > high)
   AdcCompare_OutsideRangeInclusive = (1<<8)|ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(1)|ADC_SC2_ACREN(1), //!< (ADC_value <= low) || (ADC_value >= high)
   AdcCompare_InsideRangeExclusive  = (1<<8)|ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(0)|ADC_SC2_ACREN(1), //!<  low < ADC_value < high
   AdcCompare_InsideRangeInclusive  = (0<<8)|ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(1)|ADC_SC2_ACREN(1), //!<  low <= ADC_value <= high
};

/**
 * Type definition for ADC interrupt call back.
 *
 * @param[in] value   Conversion result from channel
 * @param[in] channel Channel providing the result
 */
typedef void (*ADCCallbackFunction)(uint32_t result, int channel);

/**
 * Provides common unhandledCallback for all ADCs.
 * This class is not intended to be instantiated.
 */
class AdcBase {

private:
   AdcBase() = delete;
   AdcBase(const PcrBase&) = delete;
   AdcBase(PcrBase&&) = delete;

public:
   /** Class to static check channel pin mapping is valid */
   template<class Info, int channel> class CheckSignal {
      static_assert((channel<Info::numSignals),
            "Non-existent ADC channel - Check Configure.usbdm for available channels");
      static_assert((channel>=Info::numSignals)||(Info::info[channel].gpioBit != UNMAPPED_PCR),
            "ADC channel is not mapped to a pin - Modify Configure.usbdm");
      static_assert((channel>=Info::numSignals)||(Info::info[channel].gpioBit != INVALID_PCR),
            "ADC channel doesn't exist in this device/package - Check Configure.usbdm for available channels");
      static_assert((channel>=Info::numSignals)||((Info::info[channel].gpioBit == UNMAPPED_PCR)||(Info::info[channel].gpioBit == INVALID_PCR)||(Info::info[channel].gpioBit >= 0)),
            "Illegal ADC Channel - Check Configure.usbdm for available channels");
   public:
      /** Dummy function to allow convenient in-line checking */
      static constexpr void check() {}
   };

public:
   /** Callback to catch unhandled interrupt */
   static void unhandledCallback(uint32_t, int) {
      setAndCheckErrorCode(E_NO_HANDLER);
   }
};

/**
 * Template class representing an ADC.
 * This class is not intended to be instantiated.
 *
 * Example
 * @code
 *  // Access to ADC0
 *  using Adc0 = AdcBase_T<adc0Info>;
 *
 *  // Initialise ADC
 *  Adc0::setMode(AdcResolution_16bit_se);
 *  @endcode
 *
 * @tparam info Table of information describing ADC
 */
template<class Info>
class AdcBase_T {

private:
   AdcBase_T() = delete;
   AdcBase_T(const AdcBase_T&) = delete;
   AdcBase_T(AdcBase_T&&) = delete;

protected:
   /** Callback function for ISR */
   static ADCCallbackFunction sCallback;

public:
   /** Hardware instance pointer */
   static volatile ADC_Type &adc() { return Info::adc(); }

public:
   /** Get reference to ADC hardware as struct */
   static volatile ADC_Type &adcPtr() { return Info::adc(); }

   /** @return Base address of SPI hardware as uint32_t */
   static constexpr uint32_t adcBase() { return Info::baseAddress; }
   /** @return Base address of ADC.SC1[index] registers as uint32_t */
   static constexpr uint32_t adcSC(unsigned index) { return adcBase() + offsetof(ADC_Type, SC1[index]); }
   /** @return Base address of ADC.R[index] registers as uint32_t */
   static constexpr uint32_t adcR(unsigned index) { return adcBase() + offsetof(ADC_Type, R[index]); }

public:

   /** Allow convenient access to associate AdcInfo */
   using AdcInfo = Info;

   /**
    * IRQ handler
    */
   static void irqHandler() {
      if (adc().SC1[0] & ADC_SC1_COCO_MASK) {
         sCallback(adc().R[0], adc().SC1[0]&ADC_SC1_ADCH_MASK);
      }
      if (adc().SC1[1] & ADC_SC1_COCO_MASK) {
         sCallback(adc().R[1], adc().SC1[1]&ADC_SC1_ADCH_MASK);
      }
   }

   /**
    * Set callback for conversion complete interrupts
    *
    * @param[in] callback Callback function to execute on interrupt.\n
    *                     Use nullptr to remove callback.
    *
    * @return E_NO_ERROR            No error
    * @return E_HANDLER_ALREADY_SET Handler already set
    *
    * @note There is a single callback function for all channels of the ADC.\n
    *       It is necessary to identify the originating channel in the callback.
    * @note To change between handlers first use setCallback(nullptr).
    */
   static void setCallback(ADCCallbackFunction callback) {
      static_assert(Info::irqHandlerInstalled, "ADC not configured for interrupts. Modify Configure.usbdmProject");
      if (callback == nullptr) {
         sCallback = AdcBase::unhandledCallback;
         return;
      }
      usbdm_assert(
            (sCallback == AdcBase::unhandledCallback) || (sCallback == callback),
            "Handler already set");
      sCallback = callback;
   }

   /**
    * Configures all mapped pins associated with this peripheral
    */
   static void configureAllPins() {
      // Configure pins
      Info::initPCRs();
   }

   /**
    * Basic enable of ADC.
    * Includes enabling clock and configuring all pins of mapPinsOnEnable is selected on configuration
    */
   static void enable() {
      if (Info::mapPinsOnEnable) {
         configureAllPins();
      }

      // Enable clock to ADC
      Info::enableClock();
      __DMB();
   }

   /**
    * Disables the ADC.
    * Does not change ADC pin mapping
    */
   static void disable() {
      adc().CFG1 = 0;
      adc().CFG2 = 0;
      adc().SC2  = 0;

      // Disable clock to ADC
      Info::disableClock();
   }

   /**
    * Configure with settings from Configure.usbdmProject.
    * Includes configuring all pins.
    */
   static void defaultConfigure() {
      enable();

      // Set mode to default
      adc().CFG1 = Info::cfg1;
      adc().CFG2 = Info::cfg2;
      adc().SC2  = Info::sc2;
      adc().CV1  = Info::cv1;
      adc().CV1  = Info::cv2;
	  
      enableNvicInterrupts();
   }

   /**
    * Configure the ADC
    *
    * @param[in] adcResolution   Resolution for converter e.g. AdcResolution_16bit_se
    * @param[in] adcClockSource  Clock source e.g. AdcClockSource_Asynch
    * @param[in] adcClockDivider Clock divider e.g. AdcClockDivider_4
    * @param[in] adcSample       Input sample interval. Allows use of higher input impedance sources
    *
    * @note These settings apply to all channels on the ADC.
    * @note The resulting ADC clock rate should be restricted to [2..40MHz]
    */
   static void configure(
         AdcResolution   adcResolution,
         AdcClockSource  adcClockSource  = AdcClockSource_Default,
         AdcClockDivider adcClockDivider = AdcClockDivider_1,
         unsigned        adcSample       = 10
         ) {
      enable();
      adc().CFG1 = adcResolution|adcClockSource|adcClockDivider;
      adc().CFG2 = ADC_CFG2_SMPLTS(adcSample);
   }

   /**
    * Enable interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    */
   static void enableNvicInterrupts() {
      enableNvicInterrupt(Info::irqNums[0]);
   }

   /**
    * Enable and set priority of interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    *
    * @param[in]  nvicPriority  Interrupt priority
    */
   static void enableNvicInterrupts(uint32_t nvicPriority) {
      enableNvicInterrupt(Info::irqNums[0], nvicPriority);
   }

   /**
    * Disable interrupts in NVIC
    */
   static void disableNvicInterrupts() {
      NVIC_DisableIRQ(Info::irqNums[0]);
   }

   /**
    * Set conversion mode
    *
    * @param[in] adcResolution Resolution for converter e.g. AdcResolution_16bit_se
    *
    * @note This affects all channels on the ADC
    */
   static void setResolution(AdcResolution adcResolution) {
      adc().CFG1 = (adc().CFG1&~ADC_CFG1_MODE_MASK)|adcResolution;
   }

   /**
    * Set clock source
    *
    * @param[in] adcClockSource  Clock source e.g. AdcClockSource_Asynch
    * @param[in] adcClockDivider Clock divider e.g. AdcClockDivider_4
    *
    * @note This affects all channels on the ADC.\n
    * The resulting ADC clock rate should be restricted to the following ranges:\n
    *  [2..12MHz] for 16-bit conversion modes  \n
    *  [1..18MHz] for other conversion modes
    */
   static void setClockSource(AdcClockSource adcClockSource, AdcClockDivider adcClockDivider=AdcClockDivider_1) {
      adc().CFG1 = (adc().CFG1&~(ADC_CFG1_ADIV_MASK|ADC_CFG1_ADICLK_MASK))|adcClockSource|adcClockDivider;
   }

   /**
    * Set averaging mode
    *
    * @param[in] adcAveraging Mode for averaging e.g. AdcAveraging_4 etc
    *
    * @note This affects all channels on the ADC
    */
   static void setAveraging(AdcAveraging adcAveraging) {
      adc().SC3 = (adc().SC3&~(ADC_SC3_AVGE_MASK|ADC_SC3_AVGS_MASK))|adcAveraging;
   }

   /**
    * ADC calibrate.
    * Calibrates the ADC before first use.
    *
    * @note Set up the ADC clock and resolution before calibration
    *
    * @return E_NO_ERROR       Calibration successful
    * @return E_CALIBRATE_FAIL Failed calibration
    */
   static ErrorCode calibrate() {

      // Save current SC3 as modified
      uint8_t sc3 = adc().SC3;

      // Start calibration
      setAveraging(AdcAveraging_Cal);

      // Wait for calibration to complete
      while ((adc().SC1[0] & ADC_SC1_COCO_MASK) == 0) {
         __asm__("nop");
      }

      // Clear COCO
      (void)adc().R[0];

      // Restore original SC3 value
      adc().SC3 = sc3;

      return E_NO_ERROR;
   }

   /**
    * Configure comparison mode.
    *
    * @param[in] adcCompare   Comparison operation to enable
    * @param[in] low          Lower threshold
    * @param[in] high         Higher threshold (if needed)
    */
   static void enableComparison(AdcCompare adcCompare, int low=INT_MIN, int high=INT_MAX) {
#ifdef DEBUG_BUILD
      usbdm_assert (low<=high, "ADC Low level > high level");
#endif
      // Juggle CV1, CV2 values to satisfy comparison rules
      switch (adcCompare) {
         case AdcCompare_Disabled:
            break;
         case AdcCompare_LessThan:
         case AdcCompare_GreaterThanOrEqual:
            adc().CV1 = low;
            break;
         case AdcCompare_OutsideRangeExclusive:
         case AdcCompare_InsideRangeInclusive:
            adc().CV1 = low;
            adc().CV2 = high;
            break;
         case AdcCompare_InsideRangeExclusive:
         case AdcCompare_OutsideRangeInclusive:
            adc().CV1 = high;
            adc().CV2 = low;
            break;
      }
      // Set comparison fields
      adc().SC2 |= (adc().SC2&~(ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(1)|ADC_SC2_ACREN(1)))|
            (adcCompare&(ADC_SC2_ACFE(1)|ADC_SC2_ACFGT(1)|ADC_SC2_ACREN(1)));
   }

   /**
    * Enable/disable continuous conversion mode.
    *
    * @param[in] adcContinuous  Controls continuous conversion mode.
    */
   static void enableContinuousConversions(AdcContinuous adcContinuous) {
      if (adcContinuous) {
         adc().SC3 |= ADC_SC3_ADCO_MASK;
      }
      else {
         adc().SC3 &= ~ADC_SC3_ADCO_MASK;
      }
   }

   /**
    * Enable/disable DMA.
    *
    * @param[in] adcDma  Controls DMA operation.
    */
   static void enableDma(AdcDma adcDma = AdcDma_Enabled) {
      // Set up DMA
      if (adcDma) {
         adc().SC2 |= ADC_SC2_DMAEN_MASK;
      }
      else {
         adc().SC2 &= ~ADC_SC2_DMAEN_MASK;
      }
   }

protected:
   /**
    * Enables hardware trigger mode of operation and configures the channel.
    *
    * @param[in] sc1Value        SC1 register value including the ADC channel, Differential mode and interrupt enable
    * @param[in] adcPretrigger   Hardware pre-trigger to use for this channel\n
    *                            This corresponds to pre-triggers in the PDB channels and SC1[n] register setups
    */
   static void enableHardwareConversion(int sc1Value, AdcPretrigger adcPretrigger) {
      // Set hardware triggers
      adc().SC2 = (adc().SC2)|ADC_SC2_ADTRG(1);
      // Configure channel for hardware trigger input
      adc().SC1[adcPretrigger] = sc1Value;
   }

#ifdef ADC_SC2_DMAEN
   /**
    * Enables hardware trigger mode of operation and configures the channel.
    *
    * @param[in] sc1Value        SC1 register value including the ADC channel, Differential mode and interrupt enable
    * @param[in] adcPretrigger   Hardware pre-trigger to use for this channel.\n
    *                            This corresponds to pre-triggers in the PDB channels and SC1[n] register setups
    * @param[in] adcDma          Whether to generate a DMA request when each conversion completes
    */
   static void enableHardwareConversion(int sc1Value, AdcPretrigger adcPretrigger, AdcDma adcDma) {
      // Set hardware triggers
      adc().SC2 = (adc().SC2)|ADC_SC2_ADTRG(1)|adcDma;
      // Configure channel for hardware trigger input
      adc().SC1[adcPretrigger] = sc1Value;
   }
#endif

   /**
    * Initiates a conversion but does not wait for it to complete.
    * Intended for use with interrupts or DMA.
    *
    * @param[in] sc1Value       SC1 register value. This includes channel, differential mode and interrupts enable.
    */
   static void startConversion(const int sc1Value) {
      // Trigger conversion
      adc().SC1[0] = sc1Value;
   };

   /**
    * Initiates a conversion and waits for it to complete.
    *
    * @param[in] sc1Value SC1 register value including the ADC channel to use
    *
    * @return The result of the conversion. This should be treated as a signed value if differential mode
    */
   static uint16_t readAnalogue(const int sc1Value) {

      // Trigger conversion
      adc().SC1[0] = sc1Value;
      (void)adc().SC1[0];

      while ((adc().SC1[0]&ADC_SC1_COCO_MASK) == 0) {
         __asm__("nop");
      }
      return (uint16_t)adc().R[0];
   };

public:
   /**
    * Template class representing an ADC channel.
    *
    * Example
    * @code
    * // Instantiate the ADC and the channel (for ADC0 channel 6)
    * using Adc0    = AdcBase_T<Adc0Info>;
    * using Adc0Ch6 = Adc0::Channel<6>;
    *
    * // Set ADC resolution
    * Adc0::setMode(AdcResolution_16bit_se);
    *
    * // Read ADC value
    * uint32_t value = Adc0Ch6::readAnalogue();
    * @endcode
    *
    * @tparam channel ADC channel
    */
   template<int channel>
   class Channel {

      AdcBase::CheckSignal<Info, channel> check;

   public:
      using Pcr = PcrTable_T<Info, channel>;

      /** Allow convenient access to owning ADC */
      using Adc =  AdcBase_T<Info>;

      /** Information about this ADC */
      using AdcInfo = Info;

      /** Channel number */
      static constexpr int CHANNEL=channel;

      /**
       * Configure the pin associated with this ADC channel.
       * The pin is in analogue mode so no PCR settings are active.
       * This function is of use if mapAllPins and mapAllPinsOnEnable are not selected in USBDM configuration.
       */
      static void setInput() {
         // Map pin to ADC
         Pcr::setPCR(Info::info[channel].pcrValue);
      }

      /**
       * Enables hardware trigger mode of operation and configures a channel.
       *
       * @param[in] adcPretrigger   Hardware pre-trigger to use for this channel.\n
       *                            This corresponds to pre-triggers in the PDB channels and SC1[n]/R[n] register selection
       * @param[in] adcInterrupt    Whether to generate an interrupt when each conversion completes
       */
      static void enableHardwareConversion(AdcPretrigger adcPretrigger, AdcInterrupt adcInterrupt=AdcInterrupt_Disabled) {
         AdcBase_T<Info>::enableHardwareConversion(channel|adcInterrupt, adcPretrigger);
      }

#ifdef ADC_SC2_DMAEN
      /**
       * Enables hardware trigger mode of operation and configures a channel.
       *
       * @param[in] adcPretrigger   Hardware pre-trigger to use for this channel\n
       *                            This corresponds to pre-triggers in the PDB channels and SC1[n]/R[n] register selection
       * @param[in] adcInterrupt    Whether to generate an interrupt when each conversion completes
       * @param[in] adcDma          Whether to generate a DMA request when each conversion completes
       */
      static void enableHardwareConversion(AdcPretrigger adcPretrigger, AdcInterrupt adcInterrupt, AdcDma adcDma) {
         AdcBase_T<Info>::enableHardwareConversion(channel|adcInterrupt, adcPretrigger, adcDma);
      }
#endif

      /**
       * Initiates a conversion but does not wait for it to complete.
       * Intended for use with interrupts or DMA.
       *
       * @param[in] adcInterrupt   Determines if an interrupt is generated when conversions are complete
       */
      static void startConversion(AdcInterrupt adcInterrupt=AdcInterrupt_Disabled) {
         usbdm_assert(Info::irqHandlerInstalled || (adcInterrupt == AdcInterrupt_Disabled),
               "Enabling interrupts without a handler installed in vector table");
         AdcBase_T<Info>::startConversion(channel|adcInterrupt);
      };

      /**
       * Initiates a conversion and waits for it to complete.
       *
       * @return - the result of the conversion
       */
      static uint32_t readAnalogue() {
         // Zero extended to 32 bits
         return (uint32_t)(uint16_t)Adc::readAnalogue(channel);
      };
   };

#ifdef ADC_SC1_DIFF_MASK
   /**
    * Template class representing an ADC differential channel
    *
    * Example
    * @code
    * // Instantiate the ADC and the differential channel (for ADC_DM0, ADC_DP0)
    * using Adc0 = AdcBase_T<Adc0Info>;
    * using Adc0Ch6 = Adc0::DiffChannel<0>;
    *
    * // Set ADC resolution
    * Adc0.setMode(AdcResolution_11bit_diff );
    *
    * // Read ADC value
    * uint32_t value = Adc0Ch0.readAnalogue();
    * @endcode
    *
    * @tparam channel ADC channel
    */
   template<int channel>
   class DiffChannel {

   private:
      AdcBase::CheckSignal<typename Info::InfoDP, channel> checkPos;
      AdcBase::CheckSignal<typename Info::InfoDM, channel> checkNeg;

   public:
      /** PCR associated with plus channel */
      using PcrP = PcrTable_T<typename Info::InfoDP, channel>;

      /** PCR associated with minus channel */
      using PcrM = PcrTable_T<typename Info::InfoDM, channel>;

      /** Allow convenient access to owning ADC */
      using Adc =  AdcBase_T<Info>;

      /** Information about this ADC */
      using AdcInfo = Info;

      /** Channel number */
      static constexpr int CHANNEL=channel;

      /**
       * Configure the pins associated with this ADC channel.
       * The pins are in analogue mode so no PCR settings are active.
       * This function is of use if mapAllPins and mapAllPinsOnEnable are not selected in USBDM configuration.
       */
      static void setInput() {
         // Map pins to ADC
         PcrP::setPCR(Info::InfoDP::info[channel].pcrValue);
         PcrM::setPCR(Info::InfoDM::info[channel].pcrValue);
      }

      /**
       * Enables hardware trigger mode of operation and configures a channel.
       *
       * @param[in] adcPretrigger   Hardware pre-trigger to use for this channel\n
       *                            This corresponds to pre-triggers in the PDB channels and SC1[n] register setups
       * @param[in] adcInterrupt    Whether to generate interrupt when complete
       * @param[in] adcDma          Whether to generate a DMA request when each conversion completes
       */
      static void enableHardwareConversion(AdcPretrigger adcPretrigger, AdcInterrupt adcInterrupt=AdcInterrupt_Disabled, AdcDma adcDma=AdcDma_Disabled) {
         AdcBase_T<Info>::enableHardwareConversion(channel|ADC_SC1_DIFF_MASK|adcInterrupt, adcPretrigger, adcDma);
      }

      /**
       * Initiates a conversion but does not wait for it to complete.
       * Intended for use with interrupts or DMA.
       *
       * @param[in] adcInterrupt   Determines if an interrupt is generated when conversions are complete
       */
      static void startConversion(AdcInterrupt adcInterrupt=AdcInterrupt_Disabled) {
         usbdm_assert(Info::irqHandlerInstalled || (adcInterrupt == AdcInterrupt_Disabled),
               "Enabling interrupts without a handler installed in vector table");
         AdcBase_T<Info>::startConversion(channel|ADC_SC1_DIFF_MASK|adcInterrupt);
      };

      /**
       * Initiates a conversion and waits for it to complete.
       *
       * @return - the result of the conversion
       */
      static int32_t readAnalogue() {
         // Sign-extended to 32 bits
         return (int32_t)(int16_t)Adc::readAnalogue(channel|ADC_SC1_DIFF_MASK);
      };
   };
#endif

};

template<class Info> ADCCallbackFunction AdcBase_T<Info>::sCallback = AdcBase::unhandledCallback;

#ifdef USBDM_ADC0_IS_DEFINED
/**
 * Class representing ADC0
 */
class Adc0 : public AdcBase_T<Adc0Info> {};

/**
 * Template class representing an ADC0 channel
 *
 * Example
 * @code
 * // Instantiate the ADC channel (for ADC0 channel 6)
 * using Adc = USBDM::Adc0Channel<6>;
 *
 * // Set ADC resolution
 * Adc.setMode(AdcResolution_16bit_se);
 *
 * // Read ADC value
 * uint32_t value = Adc.readAnalogue();
 * @endcode
 *
 * @tparam channel ADC channel
 *
 * @deprecated
 */
template<int channel>
class Adc0Channel : public Adc0::Channel<channel> {};

#ifdef USBDM_ADC0_INFODM_IS_DEFINED
/**
 * Template class representing an ADC0 differential channel
 *
 * Example
 * @code
 * // Instantiate the differential ADC0 channels (for ADC0_DM0, ADC0_DP0)
 * using Adc = USBDM::Adc0DiffChannel<0>;
 *
 * // Set ADC resolution
 * Adc.setMode(AdcResolution_11bit_diff );
 *
 * // Read ADC value
 * uint32_t value = Adc0.readAnalogue();
 * @endcode
 *
 * @tparam channel ADC channel
 *
 * @deprecated
 */
template<int channel>
class Adc0DiffChannel : public Adc0::DiffChannel<channel> {};
#endif

#endif

#ifdef USBDM_ADC1_IS_DEFINED
/**
 * Class representing ADC1
 */
class Adc1 : public AdcBase_T<Adc1Info> {};

/**
 * Template class representing an ADC1 channel
 *
 * Example
 * @code
 * // Instantiate the ADC channel (for ADC1 channel 6)
 * using Adc = USBDM::Adc1Channel<6>;
 *
 * // Set ADC resolution
 * Adc.setMode(AdcResolution_16bit_se);
 *
 * // Read ADC value
 * uint32_t value = Adc.readAnalogue();
 * @endcode
 *
 * @tparam channel ADC channel
 *
 * @deprecated
 */
template<int channel>
class Adc1Channel : public Adc1::Channel<channel> {};

#ifdef USBDM_ADC1_INFODM_IS_DEFINED
/**
 * Template class representing an ADC1 differential channel
 *
 * Example
 * @code
 * // Instantiate the differential ADC1 channels (for ADC1_DM0, ADC1_DP0)
 * using Adc = USBDM::Adc1DiffChannel<0>;
 *
 * // Set ADC resolution
 * Adc.setMode(AdcResolution_11bit_diff );
 *
 * // Read ADC value
 * uint32_t value = Adc.readAnalogue();
 * @endcode
 *
 * @tparam channel ADC channel
 *
 * @deprecated
 */
template<int channel>
class Adc1DiffChannel : public Adc1::DiffChannel<channel> {};
#endif
#endif

/**
 * End ADC_Group
 * @}
 */

} // End namespace USBDM

#endif /* HEADER_ADC_H */


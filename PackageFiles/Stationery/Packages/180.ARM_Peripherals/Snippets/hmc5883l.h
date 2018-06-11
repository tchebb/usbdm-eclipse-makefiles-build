/**
 ============================================================================
 * @file     hmc5883l.h
 * @brief    Interface for HMC5883L 3-axis magnetometer
 * @version  V4.11.1.70
 * @date     18 June 2015
 * @author   podonoghue
 ============================================================================
 */

#ifndef INCLUDE_USBDM_HMC5883L_H_
#define INCLUDE_USBDM_HMC5883L_H_
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stdint.h>
#include "i2c.h"

namespace USBDM {

/**
 * @addtogroup HMC5883L_Group HMC5883L 3-axis magnetometer
 * @brief C++ Class allowing interface to HMC5883L
 * @{
 */

/**
 * @brief Class representing an interface for HMC5883L 3-axis magnetometer over I2C
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *      I2c      *i2c   = new I2C_0();
 *      HMC5883L *compass = new HMC5883L(i2c);
 *
 *      uint32_t id = compass->readID();
 *      console.write("Compass ID = 0x").write(id, Radix_16).writeln("(should be 0x483433);
 *
 *      compass->setGain(1);
 *
 *      int16_t compassX,compassY,compassZ;
 *      compass->doMeasurement(&compassX, &compassY, &compassZ);
 *
 * @endcode
 */
class HMC5883L {

public:
   enum MagBias {
      MagBias_Normal     = 0,
      MagBias_Positive   = 1,
      MagBias_Negative   = 2,
   };

   enum MagDataRate {
      MagDataRate_0_75_Hz  = 0,
      MagDataRate_1_5_Hz   = 1,
      MagDataRate_3_Hz     = 2,
      MagDataRate_7_5_Hz   = 3,
      MagDataRate_15_Hz    = 4,
      MagDataRate_30_Hz    = 5,
      MagDataRate_75_Hz    = 6,
   };

   enum MagAverages {
      MagAverages_1   = 0,
      MagAverages_2   = 1,
      MagAverages_4   = 2,
      MagAverages_8   = 3,
   };

   static constexpr uint8_t craValue(MagAverages, MagBias magBias, MagDataRate dataRate) {
      return ((magBias&0x3)<<0)|((dataRate&0x7)<<2);
   }

   enum MagRange {
//      MagRange_0_88  = 0,
      MagRange_1_3   = 1,
      MagRange_1_9   = 2,
      MagRange_2_5   = 3,
      MagRange_4_0   = 4,
      MagRange_4_7   = 5,
      MagRange_5_6   = 6,
      MagRange_8_1   = 7,
   };

   static constexpr uint8_t crbValue(MagRange magRange) {
      return ((magRange&0x7)<<5);
   }

   enum MagMode {
      MagMode_Continuous  = 0,
      MagMode_Single      = 1,
      MagMode_Sleep       = 3,
   };

protected:
   I2c &i2c;

   static const uint8_t magAddress   = 0x3C;

public:
   /**
    * Constructor
    *
    * @param i2c - I2C interface to use
    */
   HMC5883L(USBDM::I2c &i2c);

   /**
    * Destructor
    */
   virtual ~HMC5883L() {
   }

   /**
    * Read ID from compass
    *
    * @return ID value as 24-bit number (0x483433 for HMC5883L)
    */
   uint32_t readID(void);

   /**
    * Set compass range and hence gain on all channels
    *
    * @param range
    *
    * <pre>
    * G    Recommended    Gain        Resolution
    * 321  Sensor Range   (LSB/Gauss) (mGauss/LSB)
    * 000   +/- 0.88 Ga    1370        0.73
    * 001   +/- 1.3  Ga    1090        0.92 (default)
    * 010   +/- 1.9  Ga     820        1.22
    * 011   +/- 2.5  Ga     660        1.52
    * 100   +/- 4.0  Ga     440        2.27
    * 101   +/- 4.7  Ga     390        2.56
    * 110   +/- 5.6  Ga     330        3.03
    * 111   +/- 8.1  Ga     230        4.35
    * </pre>
    */
   void setRange(MagRange range);

   /**
    * Set Control register values
    *
    * @param cra - Use craValue() to construct
    * @param crb - Use crbValue() to construct
    */
   void setConfiguration(uint8_t cra, uint8_t crb);

   /**
    * Do a single triggered measurement of magnetic field
    *
    * @param x - X intensity
    * @param y - Y intensity
    * @param z - Z intensity
    */
   void doMeasurement(int16_t *x, int16_t *y, int16_t *z);

//   void calibrate();
};

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_HMC5883L_H_ */

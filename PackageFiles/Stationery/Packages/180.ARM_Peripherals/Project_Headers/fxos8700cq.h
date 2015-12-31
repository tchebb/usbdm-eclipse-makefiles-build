/**
 * @file     fxos8700cq.h
 * @brief    Interface for FXOS8700CQ 3-axis accelerometer and magnetometer
 *
 * @version  V4.11.1.70
 * @date     18 June 2015
 */

#ifndef INCLUDE_USBDM_FXOS8700CQ_H_
#define INCLUDE_USBDM_FXOS8700CQ_H_

#include <stdint.h>
#include "i2c.h"

namespace USBDM {

/**
 * @addtogroup FXOS8700CQ_Group FXOS8700CQ 3-axis accelerometer and magnetometer
 * @brief C++ Class providing interface to FXOS8700CQ
 * @{
 */

#define FXOS8700CQ_CTRL_REG1_ACTIVE_MASK         (1<<0)
#define FXOS8700CQ_CTRL_REG1_F_READ_MASK         (1<<1)
#define FXOS8700CQ_CTRL_REG1_LNOISE_MASK         (1<<2)
#define FXOS8700CQ_CTRL_REG1_DR_OFF              (3)
#define FXOS8700CQ_CTRL_REG1_DR_MASK             (0x7<<FXOS8700CQ_CTRL_REG1_DR_OFF)
#define FXOS8700CQ_CTRL_REG1_DR(x)               (((x)<<FXOS8700CQ_CTRL_REG1_DR_OFF)&FXOS8700CQ_CTRL_REG1_DR_MASK)
#define FXOS8700CQ_CTRL_REG1_ASLP_RATE_OFF       (6)
#define FXOS8700CQ_CTRL_REG1_ASLP_RATE_MASK      (0x3F<<FXOS8700CQ_CTRL_REG1_ASLP_RATE_OFF)
#define FXOS8700CQ_CTRL_REG1_ASLP_RATE(x)        (((x)<<FXOS8700CQ_CTRL_REG1_ASLP_RATE_OFF)&FXOS8700CQ_CTRL_REG1_ASLP_RATE_MASK)

#define FXOS8700CQ_CTRL_REG2_MODS_OFF            (0)
#define FXOS8700CQ_CTRL_REG2_MODS_MASK           (0x3<<FXOS8700CQ_CTRL_REG1_DR_OFF)
#define FXOS8700CQ_CTRL_REG2_MODS(x)             (((x)<<FXOS8700CQ_CTRL_REG1_DR_OFF)&FXOS8700CQ_CTRL_REG1_DR_MASK)
#define FXOS8700CQ_CTRL_REG2_SLPE_MASK           (1<<2)
#define FXOS8700CQ_CTRL_REG2_SMODS_OFF           (3)
#define FXOS8700CQ_CTRL_REG2_SMODS_MASK          (0x3<<FXOS8700CQ_CTRL_REG1_DR_OFF)
#define FXOS8700CQ_CTRL_REG2_SMODS(x)            (((x)<<FXOS8700CQ_CTRL_REG1_DR_OFF)&FXOS8700CQ_CTRL_REG1_DR_MASK)
#define FXOS8700CQ_CTRL_REG2_RST_MASK            (1<<6)
#define FXOS8700CQ_CTRL_REG2_ST_MASK             (1<<7)

#define FXOS8700CQ_CTRL_REG3_PP_OD_MASK          (1<<0)
#define FXOS8700CQ_CTRL_REG3_IPOL_MASK           (1<<1)
#define FXOS8700CQ_CTRL_REG3_WAKE_A_VECM_MASK    (1<<2)
#define FXOS8700CQ_CTRL_REG3_WAKE_FFMT_MASK      (1<<3)
#define FXOS8700CQ_CTRL_REG3_WAKE_PULSE_MASK     (1<<4)
#define FXOS8700CQ_CTRL_REG3_WAKE_INDPRT_MASK    (1<<5)
#define FXOS8700CQ_CTRL_REG3_WAKE_TRANS_MASK     (1<<6)
#define FXOS8700CQ_CTRL_REG3_FIFO_GATE_MASK      (1<<7)

#define FXOS8700CQ_CTRL_REG4_INT_EN_DRDY_MASK    (1<<0)
#define FXOS8700CQ_CTRL_REG4_INT_EN_A_VECM_MASK  (1<<1)
#define FXOS8700CQ_CTRL_REG4_INT_EN_FFMT_MASK    (1<<2)
#define FXOS8700CQ_CTRL_REG4_INT_EN_PULSE_MASK   (1<<3)
#define FXOS8700CQ_CTRL_REG4_INT_EN_INDPRT_MASK  (1<<4)
#define FXOS8700CQ_CTRL_REG4_INT_EN_TRANS_MASK   (1<<5)
#define FXOS8700CQ_CTRL_REG4_INT_EN_FIFO_MASK    (1<<6)
#define FXOS8700CQ_CTRL_REG4_INT_EN_ASLP_MASK    (1<<7)

#define FXOS8700CQ_CTRL_REG5_INT_CFG_DRDY_MASK    (1<<0)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_A_VECM_MASK  (1<<1)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_FFMT_MASK    (1<<2)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_PULSE_MASK   (1<<3)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_INDPRT_MASK  (1<<4)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_TRANS_MASK   (1<<5)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_FIFO_MASK    (1<<6)
#define FXOS8700CQ_CTRL_REG5_INT_CFG_ASLP_MASK    (1<<7)

#define FXOS8700CQ_XYZ_DATA_CFG_FS_OFF         (0)
#define FXOS8700CQ_XYZ_DATA_CFG_FS_MASK        (0x03<<FXOS8700CQ_XYZ_DATA_CFG_FS_OFF)
#define FXOS8700CQ_XYZ_DATA_CFG_FS(x)          (((x)<<FXOS8700CQ_XYZ_DATA_CFG_FS_OFF)&FXOS8700CQ_XYZ_DATA_CFG_FS_MASK)
#define FXOS8700CQ_XYZ_DATA_CFG_HPF_OUT_MASK   (1<<4)

#define FXOS8700CQ_STATUS_XYZDR_MASK 	(1<<3)

#define FXOS8700CQ_M_CTRL_REG1_M_HMS_OFF         (0)
#define FXOS8700CQ_M_CTRL_REG1_M_HMS_MASK        (0x3<<FXOS8700CQ_M_CTRL_REG1_M_HMS_OFF)
#define FXOS8700CQ_M_CTRL_REG1_M_HMS(x)          (((x)<<FXOS8700CQ_M_CTRL_REG1_M_HMS_OFF)&FXOS8700CQ_M_CTRL_REG1_M_HMS_MASK)
#define FXOS8700CQ_M_CTRL_REG1_M_OS_OFF          (2)
#define FXOS8700CQ_M_CTRL_REG1_M_OS_MASK         (0x7<<FXOS8700CQ_M_CTRL_REG1_M_OS_OFF)
#define FXOS8700CQ_M_CTRL_REG1_M_OS(x)           (((x)<<FXOS8700CQ_M_CTRL_REG1_M_OS_OFF)&FXOS8700CQ_M_CTRL_REG1_M_OS_MASK)
#define FXOS8700CQ_M_CTRL_REG1_M_OST_MASK        (1<<5)
#define FXOS8700CQ_M_CTRL_REG1_M_RST_MASK        (1<<6)
#define FXOS8700CQ_M_CTRL_REG1_M_ACAL_MASK       (1<<7)

#define FXOS8700CQ_M_CTRL_REG2_M_CNT_OFF                 (0)
#define FXOS8700CQ_M_CTRL_REG2_M_CNT_MASK                (0x3<<FXOS8700CQ_M_CTRL_REG2_M_CNT_OFF)
#define FXOS8700CQ_M_CTRL_REG2_M_CNT(x)                  (((x)<<FXOS8700CQ_M_CTRL_REG2_M_CNT_OFF)&FXOS8700CQ_M_CTRL_REG2_M_CNT_MASK)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_RST_MASK         (1<<2)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_DIS_THS_MASK     (1<<3)
#define FXOS8700CQ_M_CTRL_REG2_M_MAXMIN_DIS_MASK         (1<<4)
#define FXOS8700CQ_M_CTRL_REG2_M_HYB_AUTOINC_MODE_MASK   (1<<5)

#define FXOS8700CQ_M_CTRL_REG3_M_ST_XY_OFF            (0)
#define FXOS8700CQ_M_CTRL_REG3_M_ST_XY_MASK           (0x3<<FXOS8700CQ_M_CTRL_REG3_M_ST_XY_OFF)
#define FXOS8700CQ_M_CTRL_REG3_M_ST_XY(x)             (((x)<<FXOS8700CQ_M_CTRL_REG3_M_ST_XY_OFF)&FXOS8700CQ_M_CTRL_REG3_M_ST_XY_MASK)
#define FXOS8700CQ_M_CTRL_REG3_M_ST_Z_MASK            (1<<2)
#define FXOS8700CQ_M_CTRL_REG3_M_THS_XYZ_UPDATE_MASK  (1<<3)
#define FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS_OFF          (4)
#define FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS_MASK         (0x7<<FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS_OFF)
#define FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS(x)           (((x)<<FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS_OFF)&FXOS8700CQ_M_CTRL_REG3_M_ASLP_OS_MASK)
#define FXOS8700CQ_M_CTRL_REG3_M_RAW_MASK             (1<<7)

/**
 * @brief Class representing an interface for FXOS8700CQ 3-axis accelerometer and magnetometer over I2C
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  I2c *i2c = new I2C_0();
 *  FXOS8700CQ *accelerometer = new FXOS8700CQ(i2c, FXOS8700CQ::ACCEL_2Gmode);
 *
 *  uint8_t id = accelerometer->readID();
 *  printf("Device ID = 0x%02X\n", id);
 *
 *  printf("Before simple calibration (make sure the device is level!)\n");
 *  accelerometer->calibrateAccelerometer();
 *
 *  printf("After calibration\n");
 *  for(;;) {
 *     int accelStatus, magStatus;
 *     int16_t accelX,accelY,accelZ;
 *     int16_t magX,magY,magZ;
 *
 *     accelerometer->readAccelerometerXYZ(&accelStatus, &accelX, &accelY, &accelZ);
 *     accelerometer->readMagnetometerXYZ(&magStatus, &magX, &magY, &magZ);
 *     printf("s=0x%02X, aX=%10d, aY=%10d, aZ=%10d, ", accelStatus, accelX, accelY, accelZ);
 *     printf("s=0x%02X, mX=%10d, mY=%10d, mZ=%10d, ", magStatus,   magX,   magY,   magZ);
 *     printf("a=%d\n", (int)(360*atan2l(magX, magY)/(2*M_PI)));
 *     waitMS(400);
 *  }
 *
 * @endcode
 */
class FXOS8700CQ {

private:
   USBDM::I2c *i2c;
#ifdef MCU_MK22F51212
   static const uint8_t DEVICE_ADDRESS = 0x1C<<1;  // SA1,0 pins : 00=>0x1E, 01=>1D, 10=>1C, 11=>1F
#else
   static const uint8_t DEVICE_ADDRESS = 0x1D<<1;  // SA1,0 pins : 00=>0x1E, 01=>1D, 10=>1C, 11=>1F
#endif
   static const uint8_t  WHO_AM_I_VALUE = 0xC7;

   /**
    * Read Accelerometer register
    *
    * @param regNum  - Register number
    */
   uint8_t readReg(uint8_t regNum);
   /**
    * Write Accelerometer register
    *
    * @param regNum  - Register number
    * @param value   - Value to write
    */
   void    writeReg(uint8_t regNum, uint8_t value);
   /**
    * Reset Accelerometer
    */
   void    reset(void);
   bool    failedInit;

public:

   enum AccelerometerMode {
      ACCEL_2Gmode      = FXOS8700CQ_XYZ_DATA_CFG_FS(0),                                      // 2g Full-scale, no high-pass filter
      ACCEL_4Gmode      = FXOS8700CQ_XYZ_DATA_CFG_FS(1),                                      // 4g Full-scale, no high-pass filter
      ACCEL_8Gmode      = FXOS8700CQ_XYZ_DATA_CFG_FS(2),                                      // 8g Full-scale, no high-pass filter
      ACCEL_2G_HPF_mode = FXOS8700CQ_XYZ_DATA_CFG_FS(0)|FXOS8700CQ_XYZ_DATA_CFG_HPF_OUT_MASK, // 2g Full-scale, high-pass filter
      ACCEL_4G_HPF_mode = FXOS8700CQ_XYZ_DATA_CFG_FS(1)|FXOS8700CQ_XYZ_DATA_CFG_HPF_OUT_MASK, // 4g Full-scale, high-pass filter
      ACCEL_8G_HPF_mode = FXOS8700CQ_XYZ_DATA_CFG_FS(2)|FXOS8700CQ_XYZ_DATA_CFG_HPF_OUT_MASK, // 8g Full-scale, high-pass filter
   } ;

   enum ControlReg2Mode {
      HYB_AUTOINC_MODE = (1<<5),
      M_MAXMIN_DIS     = (1<<4),
      M_MAXMIN_DIS_THS = (1<<3),
      M_MAXMIN_RST     = (1<<2),
      M_RST_CNT_1      = (0),
      M_RST_CNT_16     = (1),
      M_RST_CNT_512    = (2),
      M_RST_CNT_NEVER  = (3),
   };

   /**
    * Data structure to return all measurements
    */
   struct Data {
      uint8_t accelerometerStatus;
      uint8_t accelerometer_X;
      uint8_t accelerometer_Y;
      uint8_t accelerometer_Z;
      uint8_t magnetometerStatus;
      uint8_t magnetometer_X;
      uint8_t magnetometer_Y;
      uint8_t magnetometer_Z;
   };

   enum Mode {
      ACCEL_ONLY = 0,
      MAG_ONLY   = 1,
      ACCEL_MAG  = 3,
   };

   /**
    * Constructor
    *
    * @param i2c  - The I2C interface to use
    * @param mode - Mode of operation (gain and filtering)
    */
   FXOS8700CQ(USBDM::I2c *i2c, AccelerometerMode mode);
   /**
    * Enable accelerometer and/or magnetometer
    *
    * @param mode ACCEL_ONLY, MAG_ONLY or ACCEL_MAG
    */
   void enable(Mode mode);
   /**
    * Put accelerometer into Standby mode
    */
   void standby();
   /**
    * Put accelerometer into Active mode
    */
   void active();
   /**
    * Obtains measurements from the accelerometer
    *
    * @param status  - Indicates status of x, y & z measurements
    * @param x       - X axis value
    * @param y       - Y axis value
    * @param z       - Z axis value
    */
   void readAccelerometerXYZ(int *status, int16_t *x, int16_t *y, int16_t *z);
   /**
    * Set accelerometer mode (gain and filtering)
    *
    * @param mode - one of ACCEL_2Gmode etc.
    */
   void setAccelerometerMode(AccelerometerMode mode);
   /**
    * Set magnetometer mode (gain and filtering)
    *
    * @param mode - one of 2Gmode etc.
    */
   void setMagnetometerMode(ControlReg2Mode mode);
   /**
    * Obtains measurements from the Magnetometer
    *
    * @param status  - Indicates status of x, y & z measurements
    * @param x       - X axis value
    * @param y       - Y axis value
    * @param z       - Z axis value
    */
   void readMagnetometerXYZ(int *status, int16_t *x, int16_t *y, int16_t *z);
   /**
    * Obtains measurements from the Accelerometer & Magnetometer
    *
    * @param data  Reference to structure to contain values read
    */
   void readAll(Data &data);

   /**
    * Read ID from accelerometer
    *
    * @return ID value as 8-bit number (0x1A for MMA8451Q)
    */
   uint32_t readID();
   /**
    * Calibrate accelerometer\n
    * Assumes accelerometer is on a flat surface
    */
   void calibrateAccelerometer();
   /**
    * Simple calibration of magnetometer\n
    * Requires user to rotate the board in all dimensions\n
    * May be used in cumulative fashion
    *
    * @param time How long to run calibration for in seconds
    */
   void calibrateMagnetometer(int time);
};

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_FXOS8700CQ_H_ */
/**
 * @file MAG3310.cpp
 *
 *  Created on: 22/11/2013
 *      Author: podonoghue
 */
#include "mag3310.h"
#include "delay.h"

using namespace USBDM;

// Accelerometer registers
enum {
   // Magnetometer registers
   M_DR_STATUS,   /*  0 */
   M_OUT_X_MSB,   /*  1 */
   M_OUT_X_LSB,   /*  2 */
   M_OUT_Y_MSB,   /*  3 */
   M_OUT_Y_LSB,   /*  4 */
   M_OUT_Z_MSB,   /*  5 */
   M_OUT_Z_LSB,   /*  6 */
   WHO_AM_I,      /*  7 */
   SYSMOD,        /*  8 */
   M_OFF_X_MSB,   /*  9 */
   M_OFF_X_LSB,   /*  A */
   M_OFF_Y_MSB,   /*  B */
   M_OFF_Y_LSB,   /*  C */
   M_OFF_Z_MSB,   /*  D */
   M_OFF_Z_LSB,   /*  E */
   TEMP,          /*  F */
   M_CTRL_REG1,   /* 10 */
   M_CTRL_REG2,   /* 11 */
};

/*
 * Constructor
 *
 * @param i2c  - The I2C interface to use
 * @param mode - Mode of operation (gain and filtering)
 */
MAG3310::MAG3310(USBDM::I2c *i2c) : i2c(i2c) {
   failedInit = false;
   if (readReg(WHO_AM_I) != WHO_AM_I_VALUE) {
      failedInit = true;
      return;
   }
   // Disable before changing settings
   writeReg(M_CTRL_REG1, 0x00);
   writeReg(M_CTRL_REG2,
         MAG3310_CTRL_REG2_AUTO_MRST_EN_MASK); // Automatic magnetic sensor resets
   writeReg(M_CTRL_REG1,
         MAG3310_CTRL_REG1_DR(0)|    // Data rate = 10 Hz @
         MAG3310_CTRL_REG1_OS(3)|    // Over-sample 128
         MAG3310_CTRL_REG1_AC_MASK); // Active
}

/**
 * Read Accelerometer register
 *
 * @param regNum  - Register number
 */
uint8_t MAG3310::readReg(uint8_t regNum) {
   uint8_t command[] = {regNum};

   i2c->txRx(DEVICE_ADDRESS, command, 1, sizeof(command));
   return command[0];
}

/**
 * Write Accelerometer register
 *
 * @param regNum  - Register number
 * @param value   - Value to write
 */
void MAG3310::writeReg(uint8_t regNum, uint8_t value) {
   uint8_t command[] = {regNum, value};

   i2c->transmit(DEVICE_ADDRESS, command, sizeof(command));
}

/**
 * Put accelerometer into Standby mode
 */
void MAG3310::standby() {

   writeReg(M_CTRL_REG1, readReg(M_CTRL_REG1)&~MAG3310_CTRL_REG1_AC_MASK);
}

/**
 * Put accelerometer into Active mode
 */
void MAG3310::active() {

   writeReg(M_CTRL_REG1, readReg(M_CTRL_REG1)|MAG3310_CTRL_REG1_AC_MASK);
}

/*
 * Obtains measurements from the Magnetometer
 *
 * @param status  - Indicates status of x, y & z measurements
 * @param x       - X axis value
 * @param y       - Y axis value
 * @param z       - Z axis value
 */
void MAG3310::readMagnetometerXYZ(int *status, int16_t *x, int16_t *y, int16_t *z) {
   uint8_t dataXYZ[7] = {M_DR_STATUS};

   // Receive 7 registers (status, X-high, X-low, Y-high, Y-low, Z-high & Z-low)
   i2c->txRx(DEVICE_ADDRESS, dataXYZ, 1, sizeof(dataXYZ));

   // Unpack data and return
   *status = dataXYZ[0];
   *x = ((dataXYZ[1]<<8)+dataXYZ[2]);
   *y = ((dataXYZ[3]<<8)+dataXYZ[4]);
   *z = ((dataXYZ[5]<<8)+dataXYZ[6]);
}

/*!
 * Read ID from accelerometer
 *
 * @return ID value as 8-bit number (0x1A for MMA8451Q)
 */
uint32_t MAG3310::readID(void) {
   uint8_t values[] = {WHO_AM_I};
   i2c->txRx(DEVICE_ADDRESS, values, 1, sizeof(values));
   return values[0];
}

/**
 * Simple calibration of magnetometer
 * Requires user to rotate the board in all dimensions
 */
void MAG3310::calibrateMagnetometer() {

   uint8_t originalMControlReg1Value = readReg(M_CTRL_REG1);
   uint8_t originalMControlReg2Value = readReg(M_CTRL_REG2);

   // Disable before changing settings
   writeReg(M_CTRL_REG1, 0x00);
   writeReg(M_CTRL_REG2,
         MAG3310_CTRL_REG2_AUTO_MRST_EN_MASK| // Automatic magnetic sensor resets
         MAG3310_CTRL_REG2_RAW_MASK);         // Raw mode
   writeReg(M_CTRL_REG1,
         MAG3310_CTRL_REG1_DR(0)|    // Data rate = 10 Hz @
         MAG3310_CTRL_REG1_OS(3)|    // Over-sample 128
         MAG3310_CTRL_REG1_AC_MASK); // Active

   waitMS(200);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

   int16_t Xout_Mag_16_bit_max, Yout_Mag_16_bit_max, Zout_Mag_16_bit_max;
   int16_t Xout_Mag_16_bit_min, Yout_Mag_16_bit_min, Zout_Mag_16_bit_min;

#pragma GCC diagnostic pop

   for (int i=0; i<200; i++) {
      // This takes ~20s (200 samples * 1/10Hz)
      int status;
      int16_t Xout_Mag_16_bit, Yout_Mag_16_bit, Zout_Mag_16_bit;

      // Obtain new measurement
      do {
         waitMS(10);
      } while ((~readReg(M_DR_STATUS) & (MAG3310_DR_STATUS_XDR_MASK|MAG3310_DR_STATUS_YDR_MASK|MAG3310_DR_STATUS_ZDR_MASK)) != 0);

      readMagnetometerXYZ(&status, &Xout_Mag_16_bit, &Yout_Mag_16_bit, &Zout_Mag_16_bit);
      if (i == 0) {
         // Assign first sample as both maximum and minimum value
         Xout_Mag_16_bit_max = Xout_Mag_16_bit_min = Xout_Mag_16_bit;
         Yout_Mag_16_bit_max = Yout_Mag_16_bit_min = Yout_Mag_16_bit;
         Zout_Mag_16_bit_max = Zout_Mag_16_bit_min = Zout_Mag_16_bit;
      }
      // Check to see if current sample is new maximum or minimum for any axis
      if (Xout_Mag_16_bit > Xout_Mag_16_bit_max)    {Xout_Mag_16_bit_max = Xout_Mag_16_bit;}
      if (Xout_Mag_16_bit < Xout_Mag_16_bit_min)    {Xout_Mag_16_bit_min = Xout_Mag_16_bit;}
      if (Yout_Mag_16_bit > Yout_Mag_16_bit_max)    {Yout_Mag_16_bit_max = Yout_Mag_16_bit;}
      if (Yout_Mag_16_bit < Yout_Mag_16_bit_min)    {Yout_Mag_16_bit_min = Yout_Mag_16_bit;}
      if (Zout_Mag_16_bit > Zout_Mag_16_bit_max)    {Zout_Mag_16_bit_max = Zout_Mag_16_bit;}
      if (Zout_Mag_16_bit < Zout_Mag_16_bit_min)    {Zout_Mag_16_bit_min = Zout_Mag_16_bit;}
   }

   // Calculate 2*average as hard iron offsets
   int16_t Xout_Mag_16_bit_avg = (Xout_Mag_16_bit_max + Xout_Mag_16_bit_min);
   int16_t Yout_Mag_16_bit_avg = (Yout_Mag_16_bit_max + Yout_Mag_16_bit_min);
   int16_t Zout_Mag_16_bit_avg = (Zout_Mag_16_bit_max + Zout_Mag_16_bit_min);

   // Write calibration values (2*average)
   writeReg(M_OFF_X_MSB, Xout_Mag_16_bit_avg>>8);
   writeReg(M_OFF_X_LSB, Xout_Mag_16_bit_avg);
   writeReg(M_OFF_Y_MSB, Yout_Mag_16_bit_avg>>8);
   writeReg(M_OFF_Y_LSB, Yout_Mag_16_bit_avg);
   writeReg(M_OFF_Z_MSB, Zout_Mag_16_bit_avg>>8);
   writeReg(M_OFF_Z_LSB, Zout_Mag_16_bit_avg);

   // Disable before changing settings
   writeReg(M_CTRL_REG1, 0x00);
   writeReg(M_CTRL_REG2, originalMControlReg2Value);
   writeReg(M_CTRL_REG1, originalMControlReg1Value);
}



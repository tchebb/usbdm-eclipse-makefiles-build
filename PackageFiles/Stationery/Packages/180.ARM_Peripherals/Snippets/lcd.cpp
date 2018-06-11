/*!
 *  @file lcd.cpp
 *
 *  @brief  Driver for Nokia 6610 LCD Display (PCF8833 Controller)
 *
 *     Controller used for LCD Display is a Philips PCF8833 STN RGB-132x132x3 driver
 *     Note: Leadis LDS176 controller (GE-12) is 100% compatible with Philips PCF8833
 *
 *     Nokia 6610 has a 132 x 132 pixel matrix - each pixel has 12 bits of colour information.
 *
 * @verbatim
 *                                 +--+
 *                                 |  |
 *              ___________________|__|_____
 *              |  ======================  |
 *     ^ (131,0)|--------------------------|(131,131)
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |    Rows|   Nokia 6610 Display     | Note: In general, you can't view Row 0 or Row 131
 *     |        |                          |        and Column 0 and Column 131.
 *     Y        |                          |
 *     |        |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     |        |                          |
 *     +        |--------------------------|
 *             (0,0)     Columns          (0,131)
 *              +---------- X ------------->
 *
 * @endverbatim
 *
 *     132 x 132 pixel matrix has three methods to specify the colour info
 *
 *     1. 12 bits per pixel
 *        requires command and 1.5 data bytes to specify a single pixel
 *        (3 data bytes can specify 2 pixels)
 *
 *     2. 8 bits per pixel
 *        requires one command and one data byte to specify the single pixel
 *        note: pixel data byte converted by RGB table to 12-bit format above
 *
 *     3. 16 bits per pixel
 *        requires one command and two data bytes to specify the single pixel
 *        note: pixel data bytes converted by dithering circuitry to 12-bit format above
 *
 *     THIS IMPLEMENTATION USES THE 12 BITS PER PIXEL METHOD!
 *     -------------------------
 *
 *     Notes: To orient the LCD display so that the origin (0, 0) is at the opposite
 *            corner from the connector (to match the orientation of the Olimex SAM7-EX256
 *            boards silk screen printing), I selected "mirror x" and "mirror y" setting
 *            in the MADCTL command (MY=1, MX=1).
 *
 *            Also, while I can't yet explain it, I had to reverse the RGB orientation in the
 *            MADCTL command and invert the entire display using the INVON command to make
 *            0x0=black, 0xF= full red.
 *
 * @author James P Lynch July 7, 2007,
 *         modified for Kinetis PO'Donoghue May, 2013
 *
 */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include "derivative.h"
#include "hardware.h"
#include "utilities.h"
#include "lcd.h"
#include "fonts.h"
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
using namespace USBDM;

#ifdef ELEC_FREAKS
#define   PHILIPS
#undef    EPSON
#endif

#define   LCD_SPI_PRIVATE
#include "lcd_private.h"

/**
 *  Initialises the LCD
 */
void LcdBase::init() {

#ifdef ELEC_FREAKS
   backLightOff();
#endif

   reset();

   // Lock SPI
   spi.startTransaction(0);

   // 10 MHz
   spi.setSpeed(10000000);

   // Mode 0, MSB first
   spi.setMode(SpiMode_0, SpiOrder_MsbFirst);

   // Use 9-bit transfers
   spi.setFrameSize(9);

   // Use software selection of peripheral
   spi.setPeripheralSelect(SpiPeripheralSelect_None, ActiveLow, SpiSelectMode_Idle);

   // Save configuration
   spiConfig = spi.getConfiguration();

   // Release SPI
   spi.endTransaction();

#ifdef PHILIPS
   txCommand(P_SWRESET);                 // Software reset

   txCommand(P_INVOFF);                  // Inversion off
   txCommand(P_SLEEPOUT);                // Sleep out

   txCommand(P_COLMOD);                  // Set Colour Mode
   txData(0x03);                         // 3 = 12-bits/pixel

   txCommand(P_MADCTL);                  // Memory Access Control
   txData(0x80);                         // Mirror x,y

   setContrast(DEFAULT_LCD_CONTRAST);    // Set default contrast
   //   spi_txCommand(P_BSTRON);                   // Boost On

   txCommand(P_DISPON);                  // Display on
#endif
#ifdef EPSON
   txCommand(DISCTL);        // Display control(EPSON)
   txData(0x00);             // P1: 0x00 = 2 divisions, switching period=8 (default)
   txData(0x20);             // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
   txData(0x00);             // P3: 0x00 = no inversely highlighted lines
   txData(0x01);             // P4 ??

   txCommand(COMSCN);        // COM scan
   txData(0x01);             // P1: 0x01 = Scan 1->80, 160<-81

   txCommand(OSCON);         // Internal oscillator ON

   txCommand(SLPOUT);        // Sleep out

   txCommand(PWRCTR);        // Power control
   txData(0x0F);             // Reference voltage regulator on, circuit voltage follower on, BOOST ON

   txCommand(DISINV);        // Inverse display

   txCommand(DATCTL);        // Data control
   txData(0x03);             // P1: //0x01 = page address inverted, col address normal, address scan in col direction
   txData(0x00);             // P2: 0x00 = RGB sequence (default value)
   txData(0x02);             // P3: 0x02 = Grayscale -> 16 (selects 12-bit colour, type A)

   txCommand(VOLCTR);        // Voltage control (contrast setting)
   txData(35);               // P1: 32 volume value (adjust this setting for your display 0 .. 63)
   txData(0x03);             // P2: 3 resistance ratio (determined by experiment)

   waitMS(200);             // Allow power supply to stabilise

   txCommand(DISON);         // Turn on the display

   //   int i;
   //   for (i=0; i<64; i++) {
   //      txCommand(VOLCTR);        // Voltage control (contrast setting)
   //      txData(i);                // P1: 32 volume value (adjust this setting for your display 0 .. 63)
   //      txData(0x03);             // P2: 3 resistance ratio (determined by experiment)
   //      waitMS(200);
   //   }

   //   txCommand(DATCTL);        // data control(EPSON)
   //   txData(0x03);             // correct for normal sin7
   //   txData(0x00);             // normal RGB arrangement
   //   txData(0x01);             // 8-bit Grayscale
   //   txData(0x02);             // 16-bit Grayscale Type A

#endif

   clear(BLACK);
#ifdef ELEC_FREAKS
   waitMS(20);
   backlightOn();
#endif
}

/** Writes the entire LCD screen from a bmp file
 *  Uses Olimex BmpToArray.exe utility
 *
 * @param bmp - bitmap to display
 *
 * @author Olimex, James P Lynch July 7, 2007
 */
void LcdBase::drawBitmap(uint8_t bmp[131*131]) {
   unsigned j; // loop counter

   lock();

#ifdef PHILIPS
   // Memory access controller
   txCommand(P_MADCTL);
   txData(0x48); // no mirror Y (temporary to satisfy Olimex bmptoarray utility)
   // Display OFF
   txCommand(P_DISPOFF);
   // Column address set (command 0x2A)
   txCommand(P_CASET);
   txData(0);
   txData(131);
   // Page address set (command 0x2B)
   txCommand(P_PASET);
   txData(0);
   txData(131);
   // WRITE MEMORY
   txCommand(P_RAMWR);
   for (j = 0; j < sizeof(uint8_t[131*131]); j++) {
      txData(bmp[j]);
   }
   // Memory access controller (command 0x36)
   txCommand(P_MADCTL);
   txData(0xC8); // restore to (mirror x and y, reverse rgb)
   // Display On
   txCommand(P_DISPON);
#endif
#ifdef EPSON
   // Memory access controller (command 0x36)
   txCommand(DATCTL);
   txData(0x48); // no mirror Y (temporary to satisfy Olimex bmptoarray utility)
   // Display OFF
   txCommand(DISOFF);
   // Column address set (command 0x2A)
   txCommand(CASET);
   txData(0);
   txData(131);
   // Page address set (command 0x2B)
   txCommand(PASET);
   txData(0);
   txData(131);
   // WRITE MEMORY
   txCommand(RAMWR);
   for (j = 0; j < sizeof(bmp); j++) {
      txData(bmp[j]);
   }
   // Memory access controller (command 0x36)
   txCommand(DATCTL);
   txData(0xC8); // restore to (mirror x and y, reverse rgb)
   // Display On
   txCommand(DISON);
#endif

  unlock();
}

/** This function will clear the screen to the given colour.
 *
 * @param colour - 12-bit colour value rrrrggggbbbb
 *
 * @author James P Lynch July 7, 2007
 */
void LcdBase::clear(Colour colour) {

   lock();

#ifdef PHILIPS
   txCommand(P_PASET);
   txData(0);
   txData(131);

   txCommand(P_CASET);
   txData(0);
   txData(131);

   txCommand(P_RAMWR);
   txColourBlock(colour, 131*131);
#endif
#ifdef EPSON
   txCommand(PASET);
   txData(0);
   txData(131);

   txCommand(CASET);
   txData(0);
   txData(131);

   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBRRRR|GGGGBBBB in 3 bytes = 2 pixels
   uint8_t data1 = (colour >> 4);
   uint8_t data2 = ((colour & 0xF) << 4) | ((colour >> 8) & 0xF);
   uint8_t data3 = colour & 0xFF;;

   txCommand(RAMWR);

   for (i=0; i < (131*131)/2; i++) {
      txData(data1);
      txData(data2);
      txData(data3);
   }
#endif

   unlock();
}

/** Sets the Row and Column addresses
 *
 * @param x = row address (0 .. 131)
 * @param y = column address (0 .. 131)
 *
 * @author James P Lynch July 7, 2007
 */
void LcdBase::setXY(int x, int y) {
   lock();

   // Row address set (command 0x2B)
#ifdef PHILIPS
   txCommand(P_PASET);
   txData(y);
   txData(y);
   // Column address set (command 0x2A)
   txCommand(P_CASET);
   txData(x);
   txData(x);
#endif

#ifdef EPSON
   txCommand(PASET);
   txData(y);
   txData(y);
   // Column address set (command 0x2A)
   txCommand(CASET);
   txData(x);
   txData(x);
#endif

   unlock();
}

/** Lights a single pixel in the specified colour at the specified x and y addresses
 *
 * @param  x     - row address (0 .. 131)
 * @param  y     - column address (0 .. 131)
 * @param  colour - 12-bit colour value rrrrggggbbbb
 *
 * @note See lcd.h for some sample colour settings
 *
 * @author James P Lynch July 7, 2007
 */
void LcdBase::drawPixel(int x, int y, Colour colour) {

   lock();

#ifdef PHILIPS
   setXY(x, y);
   txCommand(P_RAMWR);
   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBxxxx in 1.5 bytes = 1 pixels
   txData((unsigned char)(colour >> 4));
   txData((unsigned char)(((colour & 0x0F) << 4) | 0x00));
   txCommand(P_NOP);
#endif

#ifdef EPSON
   lcd_setXY(x, y);
   txCommand(RAMWR);
   // Construct RRRRGGGGBBBB=>RRRRGGGG|BBBBxxxx in 1.5 bytes = 1 pixels
   txData((unsigned char)(colour >> 4));
   txData((unsigned char)(((colour & 0x0F) << 4) | 0x0F));
   txCommand(0xFF);
#endif

   unlock();
}

/** Draws a line in the specified colour from (x0,y0) to (x1,y1)
 *
 * @param x0       Top-left-X
 * @param y0       Top-left-Y
 * @param x1       Bottom-right-X
 * @param y1       Bottom-right-Y
 * @param colour    Colour
 *
 * @author James P Lynch July 7, 2007
 *
 * @note Good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm)
 * @note See lcd.h for some sample colour settings
 *
 * @author Authors: Dr. Leonard McMillan, Associate Professor UNC \n
 *                  Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
 *
 * @note Taken verbatim from Professor McMillan's presentation: \n
 *       http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
 */
void LcdBase::drawLine(int x0, int y0, int x1, int y1, Colour colour) {

   lock();

   int dy = y1 - y0;
   int dx = x1 - x0;
   int stepx, stepy;
   if (dy < 0) {
      dy = -dy;
      stepy = -1;
   } else {
      stepy = 1;
   }
   if (dx < 0) {
      dx = -dx;
      stepx = -1;
   } else {
      stepx = 1;
   }
   dy <<= 1; // dy is now 2*dy
   dx <<= 1; // dx is now 2*dx
   drawPixel(x0, y0, colour);
   if (dx > dy) {
      int fraction = dy - (dx >> 1); // same as 2*dy - dx
      while (x0 != x1) {
         if (fraction >= 0) {
            y0 += stepy;
            fraction -= dx; // same as fraction -= 2*dx
         }
         x0 += stepx;
         fraction += dy; // same as fraction -= 2*dy
         drawPixel(x0, y0, colour);
      }
   } else {
      int fraction = dx - (dy >> 1);
      while (y0 != y1) {
         if (fraction >= 0) {
            x0 += stepx;
            fraction -= dy;
         }
         y0 += stepy;
         fraction += dx;
         drawPixel(x0, y0, colour);
      }
   }
   unlock();
}

/** Draws a rectangle in the specified colour from (x1,y1) to (x2,y2)\n
 *  Rectangle can be filled with a colour if desired
 *
 * @param  x0     row address (0 .. 131)
 * @param  y0     column address (0 .. 131)
 * @param  x1     row address (0 .. 131)
 * @param  y1     column address (0 .. 131)
 * @param  fill   indicates if the
 * @param  colour  12-bit colour value rrrrggggbbbb
 *
 * note See lcd.h for some sample colour settings
 *
 * author James P Lynch July 7, 2007
 *
 * note
 *    The best way to fill a rectangle is to take advantage of the "wrap-around" feature
 *    built into the Philips PCF8833 controller. By defining a drawing box, the memory can
 *    be simply filled by successive memory writes until all pixels have been illuminated.
 *
 *    1. Given the coordinates of two opposing corners (x0, y0) (x1, y1)
 *       calculate the minimums and maximums of the coordinates
 *
 *       xmin = (x0 <= x1) ? x0 : x1;
 *       xmax = (x0 > x1) ? x0 : x1;
 *       ymin = (y0 <= y1) ? y0 : y1;
 *       ymax = (y0 > y1) ? y0 : y1;
 *
 *    2. Now set up the drawing box to be the desired rectangle
 *
 *       LCDCommand(PASET); // set the row boundaries
 *       LCDData(xmin);
 *       LCDData(xmax);
 *       LCDCommand(CASET); // set the column boundaries
 *       LCDData(ymin);
 *       LCDData(ymax);
 *
 *    3. Calculate the number of pixels to be written divided by 2
 *
 *       NumPixels = ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1)
 *
 *       You may notice that I added one pixel to the formula.
 *       This covers the case where the number of pixels is odd and we
 *       would lose one pixel due to rounding error. In the case of
 *       odd pixels, the number of pixels is exact.
 *       in the case of even pixels, we have one more pixel than
 *       needed, but it cannot be displayed because it is outside
 *       the drawing box.
 *
 *       We divide by 2 because two pixels are represented by three bytes.
 *       So we work through the rectangle two pixels at a time.
 *
 *    4. Now a simple memory write loop will fill the rectangle
 *
 *       for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1); i++) {
 *          LCDData((colour >> 4) & 0xFF);
 *          LCDData(((colour & 0xF) << 4) | ((colour >> 8) & 0xF));
 *          LCDData(colour & 0xFF);
 *       }
 *
 *       In the case of an unfilled rectangle, drawing four lines with the Bresenham line
 *       drawing algorithm is reasonably efficient.
 */
void LcdBase::drawRect(int x0, int y0, int x1, int y1, int fill, Colour colour) {

   lock();

   int xmin, xmax, ymin, ymax;
   // Check if the rectangle is to be filled
   if (fill) {
      // best way to create a filled rectangle is to define a drawing box
      // and loop two pixels at a time
      // calculate the min and max for x and y directions
      xmin = (x0 <= x1) ? x0 : x1;
      xmax = (x0 > x1) ? x0 : x1;
      ymin = (y0 <= y1) ? y0 : y1;
      ymax = (y0 > y1) ? y0 : y1;
      // specify the controller drawing box according to those limits
      // Row address set (command 0x2B)
#ifdef PHILIPS
      txCommand(P_CASET);
#endif
#ifdef EPSON
      txCommand(CASET);
#endif
      txData(xmin);
      txData(xmax);
      // Column address set (command 0x2A)
#ifdef PHILIPS
      txCommand(P_PASET);
#endif
#ifdef EPSON
      txCommand(PASET);
#endif
      txData(ymin);
      txData(ymax);
      // WRITE MEMORY
#ifdef PHILIPS
      txCommand(P_RAMWR);
#endif
#ifdef EPSON
      txCommand(RAMWR);
#endif
      txColourBlock(colour, (xmax - xmin + 1) * (ymax - ymin + 1));
   } else {
      // best way to draw unfilled rectangle is to draw four lines
      drawLine(x0, y0, x1, y0, colour);
      drawLine(x0, y1, x1, y1, colour);
      drawLine(x0, y0, x0, y1, colour);
      drawLine(x1, y0, x1, y1, colour);
   }

   unlock();
}

/** Draws an ASCII character at the specified (x,y) address and colour
 *
 * @param c           Character to be displayed
 * @param x           Row address (0 .. 131)
 * @param y           Column address (0 .. 131)
 * @param font        Font to use
 * @param fColour     12-bit foreground colour value
 * @param bColour     12-bit background colour value
 *
 * @verbatim
 * Notes: Here's an example to display "E" at address (20,20)
 *
 *           LCDPutChar('E', 20, 20, MEDIUM, WHITE, BLACK);
 *
 *                (27,20)       (27,27)
 *                   |             |
 *                   |             |
 *                 ^ V             V
 *                 : _ # # # # # # #   0x7F
 *                 : _ _ # # _ _ _ #   0x31
 *                 : _ _ # # _ # _ _   0x34
 *                 x _ _ # # # # _ _   0x3C
 *                 : _ _ # # _ # _ _   0x34
 *                 : _ _ # # _ _ _ #   0x31
 *                 : _ # # # # # # #   0x7F
 *                 : _ _ _ _ _ _ _ _   0x00
 *
 *                   ------y------->
 *                   ^             ^
 *                   |             |
 *                   |             |
 *                (20,20)       (20,27)
 *
 *
 *    The most efficient way to display a character is to make use of the "wrap-around" feature
 *    of the Philips PCF8833 LCD controller chip.
 *
 *    Assume that we position the character at (20, 20) that's a (row, col) specification.
 *    With the row and column address set commands, you can specify an 8x8 box for the SMALL and MEDIUM
 *    characters or a 16x8 box for the LARGE characters.
 *
 *            spi_txCommand(PASET);   // set the row drawing limits
 *            spi_txData(20);         //
 *            spi_txData(27);         // limit rows to (20, 27)
 *
 *            spi_txCommand(CASET);   // set the column drawing limits
 *            spi_txData(20);         //
 *            spi_txData(27);         // limit columns to (20,27)
 *
 *    When the algorithm completes col 27, the column address wraps back to 20
 *    At the same time, the row address increases by one (this is done by the controller)
 *
 *    We walk through each row, two pixels at a time. The purpose is to create three
 *    data bytes representing these two pixels in the following format (as specified by Philips
 *    for RGB 4 : 4 : 4 format (see page 62 of PCF8833 controller manual).
 *
 *            Data for pixel 0: RRRRGGGGBBBB
 *            Data for Pixel 1: RRRRGGGGBBBB
 *
 *            spi_txCommand(RAMWR);                 // start a memory write (96 data bytes to follow)
 *
 *            spi_txData(RRRRGGGG);                 // first pixel, red and green data
 *            spi_txData(BBBBRRRR);                 // first pixel, blue data; second pixel, red data
 *            spi_txData(GGGGBBBB);                 // second pixel, green and blue data
 *            :
 *            and so on until all pixels displayed!
 *            :
 *            spi_txCommand(NOP);                   // this will terminate the RAMWR command
 *
 *  @endverbatim
 *
 *  @author James P Lynch July 7, 2007
 */
void LcdBase::putChar(char c, int x, int y, Font &font, Colour fColour, Colour bColour) {

   lock();

   unsigned int nCols;
   unsigned int nRows;
   unsigned int nBytes;
   unsigned char pixelRow;
   unsigned char mask;
   unsigned int word0;
   unsigned int word1;
   const uint8_t *pChar;

   // Get the nColumns, nRows and nBytes
   nCols  = font.width;
   nRows  = font.height;
   nBytes = font.bytesPerChar;

   // Get pointer to the last byte of the desired character
   pChar = &font.data[nBytes*(c-0x20+1)]-1;

   // Row address set
#ifdef PHILIPS
   txCommand(P_CASET);
#endif
#ifdef EPSON
   txCommand(CASET);
#endif
   txData(x);
   txData(x + nCols - 1);

   // Column address set
#ifdef PHILIPS
   txCommand(P_PASET);
#endif
#ifdef EPSON
   txCommand(PASET);
#endif
   txData(y);
   txData(y + nRows - 1);

   // Write memory
#ifdef PHILIPS
   txCommand(P_RAMWR);
#endif
#ifdef EPSON
   txCommand(RAMWR);
#endif

   // Loop on each row, working backwards from the bottom to the top
   int i;
   for (i = nRows - 1; i >= 0; i--) {
      unsigned j;
      // Copy pixel row from font table and then decrement row
      pixelRow = *pChar--;
      // Loop on each pixel in the row (left to right)
      // Note: we do two pixels each loop
      mask = 0x80;
      for (j = 0; j < nCols; j += 2) {
         // If pixel bit set, use foreground colour; else use the background colour
         // Now get the pixel colour for two successive pixels
         if ((pixelRow & mask) == 0) {
            word0 = bColour;
         }
         else {
            word0 = fColour;
         }
         mask = mask >> 1;
         if ((pixelRow & mask) == 0) {
            word1 = bColour;
         }
         else {
            word1 = fColour;
         }
         mask = mask >> 1;

         // Use this information to output three data bytes
         txData((word0 >> 4) & 0xFF);
         txData(((word0 & 0xF) << 4) | ((word1 >> 8) & 0xF));
         txData(word1 & 0xFF);
      }
   }
   // Terminate the Write Memory command
#ifdef PHILIPS
   txCommand(P_NOP);
#endif
#ifdef EPSON
   txCommand(NOP);
#endif

   unlock();
}

/** Draws a nul-terminated character string at the specified (x,y) address and colour
 *
 * @param str       Character string to be displayed
 * @param x         Row address (0 .. 131)
 * @param y         Column address (0 .. 131)
 * @param font      Font to use
 * @param fColour   12-bit foreground colour value rrrrggggbbbb
 * @param bColour   12-bit background colour value rrrrggggbbbb
 *
 * @note Here's an example to display "Hello World!" at address (20,20) \n
 *       lcd_putStr("Hello World!", 20, 20, WHITE, BLACK);
 *
 * @author James P Lynch, August 30, 2007 \n
 *         Edited by Peter Davenport on August 23, 2010
 *
 * @note For more information on how this code does it's thing look at this \n
 *       "http://www.sparkfun.com/tutorial/Nokia%206100%20LCD%20Display%20Driver.pdf"
 */
void LcdBase::putStr(const char *str, int x, int y, Font &font, Colour fColour, Colour bColour) {

   while (*str != '\0') {
      // Draw the character
      putChar(*str++, x, y, font, fColour, bColour);

      // Advance the x position
      x += font.width;

      // Clip at right
      if (x > 131) break;
   }
}

/** Set LCD contrast - varies with device
 *
 *  @param setting - contrast level (0..127) ?
 */
void LcdBase::setContrast(uint8_t setting) {

   lock();

#ifdef EPSON
   txCommand(VOLCTR);       // electronic volume, this is the contrast/brightness(EPSON)
   txData(setting);         // volume (contrast) setting - course adjustment,  -- original was 24
   //     LCDData(0x03);     // internal resistor ratio - coarse adjustment
   //     LCDData(0x30);
   txCommand(NOP);          // nop(EPSON)
#endif
#ifdef PHILIPS
   txCommand(P_SETCON);     // Contrast/brightness
   txData(setting);
#endif

   unlock();
}

/** Draws a line circle in the specified colour at center (x0,y0) with radius
 *
 * @param centreX = row address (0 .. 131)
 * @param centreY = column address (0 .. 131)
 * @param radius = radius in pixels
 * @param colour = 12-bit colour value rrrrggggbbbb
 * @param circleType = controls which segments of the circle are drawn
 *
 * @author Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
 *
 * @note Taken verbatim Wikipedia article on Bresenham's line algorithm \n
 *        http://www.wikipedia.org
 */
void LcdBase::drawCircle(int centreX, int centreY, int radius, Colour colour, int circleType) {

   lock();

   int f = 1 - radius;
   int ddF_x = 0;
   int ddF_y = -2 * radius;
   int x = 0;
   int y = radius;

   if (circleType&(SECTOR_315_360|SECTOR_0_45)) {
      drawPixel(centreX + radius, centreY, colour);  // 0,360
   }
   if (circleType&(SECTOR_45_90|SECTOR_90_135)) {
      drawPixel(centreX, centreY + radius, colour);  // 90
   }
   if (circleType&(SECTOR_135_180|SECTOR_180_225)) {
      drawPixel(centreX - radius, centreY, colour);  // 180
   }
   if (circleType&(SECTOR_225_270|SECTOR_270_315)) {
      drawPixel(centreX, centreY - radius, colour);  // 270
   }
   while (x < y) {
      if (f >= 0) {
         y--;
         ddF_y += 2;
         f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x + 1;
      if (circleType&SECTOR_0_45) {
         drawPixel(centreX + y, centreY + x, colour);   // 0-45
      }
      if (circleType&SECTOR_45_90) {
         drawPixel(centreX + x, centreY + y, colour);   // 45-90
      }
      if (circleType&SECTOR_90_135) {
         drawPixel(centreX - x, centreY + y, colour);   // 90-135
      }
      if (circleType&SECTOR_135_180) {
         drawPixel(centreX - y, centreY + x, colour);   // 135-180
      }
      if (circleType&SECTOR_180_225) {
         drawPixel(centreX - y, centreY - x, colour);   // 180-225
      }
      if (circleType&SECTOR_225_270) {
         drawPixel(centreX - x, centreY - y, colour);   // 225-270
      }
      if (circleType&SECTOR_270_315) {
         drawPixel(centreX + x, centreY - y, colour);   // 270-315
      }
      if (circleType&SECTOR_315_360) {
         drawPixel(centreX + y, centreY - x, colour);   // 315-360
      }
   }

   unlock();
}


/**
 * @file     lpuart.h (180.ARM_Peripherals/Project_Headers/lpuart.h)
 * @brief    Universal Asynchronous Receiver/Transmitter interface
 *
 * @version  V4.12.1.210
 * @date     13 April 2016
 *      Author: podonoghue
 */

#ifndef INCLUDE_USBDM_LPUART_H_
#define INCLUDE_USBDM_LPUART_H_
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stdint.h>
#include "derivative.h"
#include "hardware.h"
#include "formatted_io.h"
#include "queue.h"
#ifdef __CMSIS_RTOS
#include "cmsis.h"
#endif

namespace USBDM {

/**
 * @addtogroup LPUART_Group LPUART, Low Power Universal Asynchronous Receiver/Transmitter
 * @brief C++ Class allowing access to UART interface
 * @{
 */

/**
 * Enumeration selecting interrupt sources
 */
enum LpuartInterrupt {
   LpuartInterrupt_TxHoldingEmpty  = LPUART_CTRL_TIE(1),   //!< Interrupt request on Transmit holding register empty
   LpuartInterrupt_TxComplete      = LPUART_CTRL_TCIE(1),  //!< Interrupt request on Transmit complete
   LpuartInterrupt_RxFull          = LPUART_CTRL_RIE(1),   //!< Interrupt request on Receive holding full
   LpuartInterrupt_IdleDetect      = LPUART_CTRL_ILIE(1),  //!< Interrupt request on Idle detection
};

/**
 * Enumeration selecting direct memory access sources
 */
enum LpuartDma {
   LpuartDma_TxHoldingEmpty  = LPUART_BAUD_TDMAE(1),   //!< DMA request on Transmit holding register empty
   LpuartDma_RxFull          = LPUART_BAUD_RDMAE(1),   //!< DMA request on Receive holding full
};

/**
 * @brief Virtual Base class for UART interface
 */
class Lpuart : public FormattedIO {

protected:
#ifdef __CMSIS_RTOS
   /**
    * Obtain UART mutex.
    *
    * @param[in]  milliseconds How long to wait in milliseconds. Use osWaitForever for indefinite wait
    *
    * @return osOK:                    The mutex has been obtain.
    * @return osErrorTimeoutResource:  The mutex could not be obtained in the given time.
    * @return osErrorResource:         The mutex could not be obtained when no timeout was specified.
    * @return osErrorParameter:        The parameter mutex_id is incorrect.
    * @return osErrorISR:              Cannot be called from interrupt service routines.
    *
    * @note The USBDM error code will also be set on error
    */
   virtual osStatus startTransaction(int milliseconds=osWaitForever) = 0;

   /**
    * Release UART mutex.
    *
    * @return osOK:              The mutex has been correctly released.
    * @return osErrorResource:   The mutex was not obtained before.
    * @return osErrorISR:        Cannot be called from interrupt service routines.
    *
    * @note The USBDM error code will also be set on error
    */
   virtual osStatus endTransaction() = 0;
#else
   /**
    * Obtain UART - dummy routine (non RTOS)
    */
   int startTransaction(int =0) {
      return 0;
   }
   /**
    * Release UART - dummy routine (non RTOS)
    */
   int endTransaction() {
      return 0;
   }
#endif

   /**
    * Check if character is available
    *
    * @return true  Character available i.e. _readChar() will not block
    * @return false No character available
    */
   virtual bool _isCharAvailable() override {
      return (lpuart.STAT & LPUART_STAT_RDRF_MASK);
   }

   /**
    * Receives a single character (blocking)
    *
    * @return Character received
    */
   virtual int _readChar() override {

      // Get status from UART
      uint32_t status;
      do {
         // Get status from UART
         status = lpuart.STAT;
         // Clear & ignore pending errors
         if ((status & (LPUART_STAT_FE_MASK|LPUART_STAT_OR_MASK|LPUART_STAT_PF_MASK|LPUART_STAT_NF_MASK)) != 0) {
            lpuart.STAT = LPUART_STAT_FE_MASK|LPUART_STAT_OR_MASK|LPUART_STAT_PF_MASK|LPUART_STAT_NF_MASK;
         }
         // Check for Rx buffer full
      } while ((status & LPUART_STAT_RDRF_MASK) == 0);
      return (uint8_t)(lpuart.DATA);
   }

   /**
    * Writes a character (blocking)
    *
    * @param[in]  ch - character to send
    */
   virtual void _writeChar(char ch) override {
      while ((lpuart.STAT & LPUART_STAT_TDRE_MASK) == 0) {
         // Wait for Tx buffer empty
         __asm__("nop");
      }
      lpuart.DATA = ch;
      if (ch=='\n') {
         write('\r');
      }
   }

   /**
    * Handler for interrupts when no handler set
    */
   static void unhandledCallback(uint8_t) {
      setAndCheckErrorCode(E_NO_HANDLER);
   }

public:

   /**
    * LPUART hardware instance
    */
   volatile LPUART_Type &lpuart;

   /**
    * Construct UART interface
    *
    * @param[in]  lpuart Reference to LPUART hardware
    */
   Lpuart(volatile LPUART_Type &lpuart) : lpuart(lpuart) {
   }

   /**
    * Destructor
    */
   virtual ~Lpuart() {
   }

   /**
    * Set baud factor value for interface.
    *
    * This is calculated from baud rate and UART clock frequency
    *
    * @param[in]  baudrate       Interface speed in bits-per-second
    * @param[in]  clockFrequency Frequency of UART clock
    * @param[in]  oversample     Over-sample ratio to use when calculating divider
    */
   void __attribute__((noinline)) setBaudRate(uint32_t baudrate, uint32_t clockFrequency, uint32_t oversample) {

      // Disable UART before changing registers
      uint32_t ctrl = lpuart.CTRL;
      lpuart.CTRL = 0;

      // Calculate UART divider with rounding
      uint32_t divider = (clockFrequency<<1)/(oversample * baudrate);
      divider = (divider>>1)|(divider&0b1);

      // Set Baud rate register
      lpuart.BAUD = (lpuart.BAUD&~(LPUART_BAUD_SBR_MASK|LPUART_BAUD_OSR_MASK))|
            LPUART_BAUD_SBR(divider)|LPUART_BAUD_OSR(oversample-1);

      // Restore UART settings
      lpuart.CTRL = ctrl;
   }

   /**
    * Set baud factor value for interface
    *
    * This is calculated from baud rate and LPUART clock frequency
    *
    * @param[in]  baudrate  Interface speed in bits-per-second
    */
   virtual void setBaudRate(unsigned baudrate) = 0;

   /**
    * Enable/disable an interrupt source
    *
    * @param[in] lpuartInterrupt Interrupt source to modify
    * @param[in] enable          True to enable, false to disable
    *
    * @note Changing the enabled interrupt functions may also affect the DMA settings
    */
   void enableInterrupt(LpuartInterrupt lpuartInterrupt, bool enable=true) {
      if (enable) {
         lpuart.CTRL |= lpuartInterrupt;
      }
      else {
         lpuart.CTRL &= ~lpuartInterrupt;
      }
   }

   /**
    * Enable/disable a DMA source
    *
    * @param[in] lpuartDma  DMA source to modify
    * @param[in] enable     True to enable, false to disable
    *
    * @note Changing the enabled DMA functions may also affect the interrupt settings
    */
   void enableDma(LpuartDma lpuartDma, bool enable=true) {
      // Flags are in same positions in the C2 and C5
      if (enable) {
         lpuart.BAUD |= lpuartDma;
      }
      else {
         lpuart.BAUD &= ~lpuartDma;
      }
   }

   /**
    *  Flush output data
    */
   virtual void flushOutput() override {
      while ((lpuart.STAT & LPUART_STAT_TC_MASK) == 0) {
      // Wait until transmission of last character is complete
      }
   };

   /**
    *  Flush input data
    */
   virtual void flushInput() override {
      (void)lpuart.DATA;
      lookAhead = -1;
   };
};

/**
 * Type definition for UART interrupt call back
 *
 *  @param[in]  status - Interrupt flags e.g. UART_S1_TDRE, UART_S1_RDRF etc
 */
typedef void (*LPUARTCallbackFunction)(uint8_t status);

/**
 * @brief Template class representing an UART interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  Uart *uart0 = new USBDM::Lpuart_T<Uart0Info>(115200);
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 *
 * @tparam Info   Class describing UART hardware
 */
template<class Info> class Lpuart_T : public Lpuart {

public:
   /** Get reference to LPUART hardware as struct */
   static volatile LPUART_Type &uartPtr() { return Info::uart(); }

   /** Get base address of LPUART hardware as uint32_t */
   static constexpr uint32_t uartBase() { return Info::baseAddress; }

   /** Get base address of UART.DATA register as uint32_t */
   static constexpr uint32_t uartDATA() { return uartBase() + offsetof(LPUART_Type, DATA); }

#ifdef __CMSIS_RTOS
protected:
   /**
    * Mutex to protect access\n
    * Using a static accessor function avoids issues with static object initialisation order
    *
    * @return mutex
    */
   static CMSIS::Mutex &mutex(int =0) {
      /** Mutex to protect access - static so per UART */
      static CMSIS::Mutex mutex;
      return mutex;
   }

public:
   /**
    * Obtain UART mutex.
    *
    * @param[in]  milliseconds How long to wait in milliseconds. Use osWaitForever for indefinite wait
    *
    * @return osOK:                    The mutex has been obtain.
    * @return osErrorTimeoutResource:  The mutex could not be obtained in the given time.
    * @return osErrorResource:         The mutex could not be obtained when no timeout was specified.
    * @return osErrorParameter:        The parameter mutex_id is incorrect.
    * @return osErrorISR:              Cannot be called from interrupt service routines.
    *
    * @note The USBDM error code will also be set on error
    */
   virtual osStatus startTransaction(int milliseconds=osWaitForever) override {
      // Obtain mutex
      osStatus status = mutex().wait(milliseconds);
      if (status != osOK) {
         setCmsisErrorCode(status);
      }
      return status;
   }

   /**
    * Release UART mutex
    *
    * @return osOK:              The mutex has been correctly released.
    * @return osErrorResource:   The mutex was not obtained before.
    * @return osErrorISR:        Cannot be called from interrupt service routines.
    *
    * @note The USBDM error code will also be set on error
    */
   virtual osStatus endTransaction() override {
      // Release mutex
      osStatus status = mutex().release();
      if (status != osOK) {
         setCmsisErrorCode(status);
      }
      return status;
   }
#endif

protected:
   /** Callback function for RxTx ISR */
   static LPUARTCallbackFunction rxTxCallback;

public:
   /**
    * Configures all mapped pins associated with this peripheral
    */
   static void __attribute__((always_inline)) configureAllPins() {
      // Configure pins
      Info::initPCRs();
   }

   /**
    * Construct LPUART interface
    *
    * @param[in]  baudrate         Interface speed in bits-per-second
    */
   Lpuart_T(unsigned baudrate=Info::defaultBaudRate) : Lpuart(Info::lpuart()) {
      // Enable clock to UART interface
#ifdef PCC_BASE_PTR
      Info::setClockSource(Info::defaultClockSource);
#endif
      Info::enableClock();

      if (Info::mapPinsOnEnable) {
         configureAllPins();
      }

      setBaudRate(baudrate);

      lpuart.CTRL = LPUART_CTRL_TE(1)|LPUART_CTRL_RE(1);
   }

   /**
    * Destructor
    */
   ~Lpuart_T() {}

   /**
    * Set baud factor value for interface
    *
    * This is calculated from baud rate and LPUART clock frequency
    *
    * @param[in]  baudrate Interface speed in bits-per-second
    */
   void setBaudRate(unsigned baudrate) {
      // Over-sample ratio
      static constexpr uint32_t OVER_SAMPLE = Info::oversampleRatio;

      Lpuart::setBaudRate(baudrate, Info::getClockFrequency(), OVER_SAMPLE);
   }

   /**
    * Set baud factor value for interface
    *
    * This is calculated from baud rate and LPUART clock frequency
    *
    * @param[in]  baudrate    Interface speed in bits-per-second
    * @param[in]  oversample  Over-sample ratio to use when calculating divider
    */
   void setBaudRate(unsigned baudrate, unsigned oversample) {
      Lpuart::setBaudRate(baudrate, Info::getClockFrequency(), oversample);
   }

protected:

public:
   /**
    * Receive/Transmit/Error IRQ handler
    */
   static void irqHandler() {
      uint8_t status = Info::lpuart().STAT;
      rxTxCallback(status);
   }

   /**
    * Set Receive/Transmit Callback function
    *
    *   @param[in]  callback Callback function to be executed on UART receive or transmit
    */
   static void setRxTxCallback(LPUARTCallbackFunction callback) {
      if (callback == nullptr) {
         rxTxCallback = unhandledCallback;
      }
      rxTxCallback = callback;
   }

   /**
    * Enable interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    */
   static void enableNvicInterrupts() {
      NVIC_EnableIRQ(Info::irqNums[0]);
      if (Info::irqCount>1) {
         NVIC_EnableIRQ(Info::irqNums[1]);
      }
      if (Info::irqCount>2) {
         NVIC_EnableIRQ(Info::irqNums[2]);
      }
   }

   /**
    * Enable and set priority of interrupts in NVIC
    * Any pending NVIC interrupts are first cleared.
    *
    * @param[in]  nvicPriority  Interrupt priority
    */
   static void enableNvicInterrupts(uint32_t nvicPriority) {
      enableNvicInterrupt(Info::irqNums[0], nvicPriority);
      if (Info::irqCount>1) {
          enableNvicInterrupt(Info::irqNums[1], nvicPriority);
      }
      if (Info::irqCount>2) {
          enableNvicInterrupt(Info::irqNums[2], nvicPriority);
      }
   }

   /**
    * Disable interrupts in NVIC
    */
   static void disableNvicInterrupts() {
      NVIC_DisableIRQ(Info::irqNums[0]);
      if (Info::irqCount>1) {
         NVIC_DisableIRQ(Info::irqNums[1]);
      }
      if (Info::irqCount>2) {
         NVIC_DisableIRQ(Info::irqNums[2]);
      }
   }
};

template<class Info> LPUARTCallbackFunction Lpuart_T<Info>::rxTxCallback  = unhandledCallback;

/**
 * @brief Template class representing an UART interface with buffered reception
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  Uart *lpuart = new USBDM::Lpuart_T<Lpuart0Info>(115200);
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 *
 * @tparam Info   Class describing UART hardware
 */
template<class Info, int rxSize=Info::receiveBufferSize, int txSize=Info::transmitBufferSize>
class LpuartBuffered_T : public Lpuart_T<Info> {

public:
   using Lpuart_T<Info>::lpuart;

   LpuartBuffered_T(unsigned baudrate=Info::defaultBaudRate) : Lpuart_T<Info>(baudrate) {
      Lpuart::enableInterrupt(LpuartInterrupt_RxFull);
      Lpuart_T<Info>::enableNvicInterrupts();
   }

   virtual ~LpuartBuffered_T() {
      Lpuart::enableInterrupt(LpuartInterrupt_RxFull,         false);
      Lpuart::enableInterrupt(LpuartInterrupt_TxHoldingEmpty, false);
   }

protected:

   /** Lock variable for writes */
   static volatile uint32_t fWriteLock;

   /** Lock variable for reads */
   static volatile uint32_t fReadLock;

   /**
    * Queue for Buffered reception (if used)
    */
   static Queue<char, rxSize> rxQueue;
   /**
    * Queue for Buffered transmission (if used)
    */
   static Queue<char, txSize> txQueue;

   /**
    * Writes a character (blocking on queue full)
    *
    * @param[in]  ch - character to send
    */
   virtual void _writeChar(char ch) override {
      lock(&fWriteLock);
      // Add character to buffer
      while (!txQueue.enQueueDiscardOnFull(ch)) {
      }
      lpuart.CTRL |= LPUART_CTRL_TIE_MASK;
      unlock(&fWriteLock);
      if (ch=='\n') {
        _writeChar('\r');
      }
   }

   /**
    * Receives a single character (blocking on queue empty)
    *
    * @return Character received
    */
   virtual int _readChar() override {
      lock(&fReadLock);
      while (rxQueue.isEmpty()) {
         __asm__("nop");
      }
      char t = rxQueue.deQueue();
      unlock(&fReadLock);
      return t;
   }

   /**
    * Check if character is available
    *
    * @return true  Character available i.e. _readChar() will not block
    * @return false No character available
    */
   virtual bool _isCharAvailable() override {
      return (!rxQueue.isEmpty());
   }

public:
   /**
    * Receive/Transmit/Error IRQ handler
    */
   static void irqHandler()  {
      auto status = Info::lpuart().STAT;
      if (status & LPUART_STAT_RDRF_MASK) {
         // Receive data register full - save data
         rxQueue.enQueueDiscardOnFull(Info::lpuart().DATA);
      }
      if (status & LPUART_STAT_TDRE_MASK) {
         // Transmitter ready
         if (txQueue.isEmpty()) {
            // No data available - disable further transmit interrupts
            Info::lpuart().CTRL &= ~LPUART_CTRL_TIE_MASK;
         }
         else {
            // Transmit next byte
            Info::lpuart().DATA = txQueue.deQueue();
         }
      }
   }

   /**
    *  Flush output data.
    *  This blocks until all pending data has been sent
    */
   virtual void flushOutput() override {
      while (!txQueue.isEmpty()) {
         // Wait until queue empty
      }
      while ((lpuart.STAT & LPUART_STAT_TC_MASK) == 0) {
         // Wait until transmission of last character is complete
      }
   }

   /**
    *  Flush input data
    */
   virtual void flushInput() override {
      Lpuart_T<Info>::flushInput();
      rxQueue.clear();
   }

};

template<class Info, int rxSize, int txSize> Queue<char, rxSize> LpuartBuffered_T<Info, rxSize, txSize>::rxQueue;
template<class Info, int rxSize, int txSize> Queue<char, txSize> LpuartBuffered_T<Info, rxSize, txSize>::txQueue;
template<class Info, int rxSize, int txSize> volatile uint32_t   LpuartBuffered_T<Info, rxSize, txSize>::fReadLock  = 0;
template<class Info, int rxSize, int txSize> volatile uint32_t   LpuartBuffered_T<Info, rxSize, txSize>::fWriteLock = 0;

#ifdef USBDM_LPUART0_IS_DEFINED
/**
 * @brief Class representing LPUART0 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart0 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART0/lpuartClass:Lpuart_T)<Lpuart0Info> Lpuart0;
#endif

#ifdef USBDM_LPUART1_IS_DEFINED
/**
 * @brief Class representing LPUART1 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart1 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART1/lpuartClass:Lpuart_T)<Lpuart1Info> Lpuart1;
#endif

#ifdef USBDM_LPUART2_IS_DEFINED
/**
 * @brief Class representing LPUART2 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart2 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART2/lpuartClass:Lpuart_T)<Lpuart2Info> Lpuart2;
#endif

#ifdef USBDM_LPUART3_IS_DEFINED
/**
 * @brief Class representing LPUART3 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart3 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART3/lpuartClass:Lpuart_T)<Lpuart3Info> Lpuart3;
#endif

#ifdef USBDM_LPUART4_IS_DEFINED
/**
 * @brief Class representing LPUART4 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart4 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART4/lpuartClass:Lpuart_T)<Lpuart4Info> Lpuart4;
#endif

#ifdef USBDM_LPUART5_IS_DEFINED
/**
 * @brief Class representing LPUART5 interface
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  USBDM::Lpuart5 lpuart;
 *
 *  for(int i=0; i++;) {
 *     lpuart<<"Hello world, i="<<i<<"\n";
 *  }
 *  @endcode
 */
typedef  $(/LPUART5/lpuartClass:Lpuart_T)<Lpuart5Info> Lpuart5;
#endif

/**
 * End LPUART_Group
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_LPUART_H_ */

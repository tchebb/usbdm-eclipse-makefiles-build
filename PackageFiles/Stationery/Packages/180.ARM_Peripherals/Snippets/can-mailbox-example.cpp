/**
 ============================================================================
 * @file    can-mailbox-example.cpp (180.ARM_Peripherals/Snippets)
 * @brief   Basic C++ demo of using FlexCAN mailboxes
 *
 *  Created on: 6Dec.,2018
 *      Author: podonoghue
 ============================================================================
 */
#include "hardware.h"
#include "can.h"

using namespace USBDM;

using Led = GpioD<0, ActiveLow>;

// CAN interface to use
using Can     = Can0;
using CanLimp = GpioE<13, ActiveLow>;

void canErrorCallback() {
   uint32_t status        = Can::getErrorStatus();
   Can::clearErrorStatus(status);
   console.setWidth(32);
   console.setPadding(Padding_LeadingZeroes);
   console.writeln("                               10987654321098765432109876543210");
   console.writeln("                                         O BSTR10ACFSTRITFFRBE");
   console.writeln("                                         V OYxxEECRRTxxDxLLxOR");
   console.writeln("                                         R FNWWRRKCMFEEL TT FR");
   console.write(  "canErrorCallback(), status = 0b").writeln(status, Radix_2).writeln();
   console.reset();
}

void canWakeupCallback() {
   console.writeln("canWakeupCallback()");
}

/**
 * CAN Message ID for Rx Mailboxes
 */
static constexpr unsigned CAN_TX_MAILBOX_1_ID = 30;
static constexpr unsigned CAN_TX_MAILBOX_2_ID = 31;
/**
 * CAN Message ID for Tx Mailboxes
 */
static constexpr unsigned CAN_RX_MAILBOX_1_ID = 40; // Start of range
static constexpr unsigned CAN_RX_MAILBOX_2_ID = 80; // Start of range
/**
 * CAN Message format
 */
static constexpr CanMode  CAN_MODE = CanMode_Standard;

/**
 * Transmit mailbox interrupt handler
 *
 * @param mailboxNum NUmber of mailbox causing the interrupt
 */
void canTxMailboxCallback(unsigned mailboxNum) {
   auto mailbox = Can::getMailbox(mailboxNum);
   console
      .write("canTxMailboxCallback() Message ID(")
      .write((CAN_MODE==CanMode_Standard)?mailbox->ID.idStd:mailbox->ID.idExt)
      .writeln(") Tx complete");

   //   uint32_t mailboxFlags  = Can::getMailboxFlags();
   //   uint32_t fifoFlags     = Can::getFifoFlags();

   Can::clearMailboxFlag(mailboxNum);

   //   console.setWidth(32);
   //   console.setPadding(Padding_LeadingZeroes);
   //   console.writeln("                                                            OWF");
   //   console.write(  "canOredCallback(), fifoFlags    = 0b").writeln(fifoFlags,    Radix_2);
   //   console.writeln("                                    10987654321098765432109876543210");
   //   console.write(  "canOredCallback(), mailboxFlags = 0b").writeln(mailboxFlags, Radix_2);
   //   console.reset();

}

/**
 * Receive mailbox interrupt handler
 *
 * @param mailboxNum NUmber of mailbox causing the interrupt
 */
void canRxMailboxCallback(unsigned mailboxNum) {
   auto mailbox = Can::getMailbox(mailboxNum);
   console
      .write("canRxMailboxCallback() Message ID(")
      .write((CAN_MODE==CanMode_Standard)?mailbox->ID.idStd:mailbox->ID.idExt)
      .write(") - ");
   for (unsigned index=0; ; index++) {
      console.write(mailbox->data8(index), Radix_16);
      if (index>=mailbox->CS.dlc) {
         break;
      }
      console.write(", ");
   }
   console.writeln();

   // Reinitialise mailbox
   mailbox->CS = CanControlStatus(CanMessageCode_RxEmpty, CanFrameType_Remote);

   Can::clearMailboxFlag(mailboxNum);
}

/**
 * This example sets up the CAN to transmit and receive messages using mailboxes
 */
void mailboxOnlyExample() {

//   usbdm_assert(!Can::FIFO_AVAILABLE, "This example assumes no FIFO is configured");
   usbdm_assert(Can::NUM_MAILBOXES>=3, "This example requires > 3 mailboxes");

   CanParameters canParameters(125000);
   canParameters.idam      = CanAcceptanceMode_FormatA;
   canParameters.wrnen     = true;
   canParameters.errmsk    = true;
   //   canParameters.lpb       = true;

   Can::setErrorCallback(canErrorCallback);
   Can::setWakeupCallback(canWakeupCallback);
   Can::configure(canParameters);

   Can::configureAllPins();

   // Enable all interrupts from FIFO
   Can::enableFifoInterrupts(CAN_FIFO_DATA_FLAG|CAN_FIFO_OVERFLOW_FLAG|CAN_FIFO_WARNING_FLAG);

   Can::enableErrorNvicInterrupts(NvicPriority_Normal);
   Can::enableWakeupNvicInterrupts(NvicPriority_Normal);
   Can::enableMessageBufferNvicInterrupts(NvicPriority_Normal);

   Can::enableOredNvicInterrupts(NvicPriority_Normal);

   static auto txMailbox1 = Can::allocateMailbox();
   txMailbox1.setCallback(canTxMailboxCallback);
   txMailbox1.enableInterrupt();
   auto *canTxMailbox1 = txMailbox1.getMailbox();
   canTxMailbox1->ID = CanId(CAN_MODE, CAN_TX_MAILBOX_1_ID);
   canTxMailbox1->data8(0) = 0x1;
   canTxMailbox1->data8(1) = 0x2;
   canTxMailbox1->data8(2) = 0x3;
   canTxMailbox1->data8(3) = 0x4;
   canTxMailbox1->data8(4) = 0x5;
   canTxMailbox1->data8(5) = 0x6;
   canTxMailbox1->data8(6) = 0x7;
   canTxMailbox1->data8(7) = 0x8;
   canTxMailbox1->CS = CanControlStatus(CanMessageCode_TxData, CAN_MODE, CanDataSize_7, CanFrameType_Data);
   canTxMailbox1->CS;

   static auto txMailbox2 = Can::allocateMailbox();
   txMailbox2.setCallback(canTxMailboxCallback);
   txMailbox2.enableInterrupt();
   auto *canTxMailbox2 = txMailbox2.getMailbox();
   canTxMailbox2->ID = CanId(CAN_MODE, CAN_TX_MAILBOX_2_ID);
   canTxMailbox2->data8(0) = 0x11;
   canTxMailbox2->data8(1) = 0x12;
   canTxMailbox2->data8(2) = 0x13;
   canTxMailbox2->data8(3) = 0x14;
   canTxMailbox2->data8(4) = 0x15;
   canTxMailbox2->data8(5) = 0x16;
   canTxMailbox2->data8(6) = 0x17;
   canTxMailbox2->data8(7) = 0x18;
   canTxMailbox2->CS = CanControlStatus(CanMessageCode_TxData, CAN_MODE, CanDataSize_3, CanFrameType_Data);

   static auto rxMailbox1 = Can::allocateMailbox();
   rxMailbox1.setCallback(canRxMailboxCallback);
   rxMailbox1.getMailboxFilterMask() = CanMailboxFilterMask(CAN_MODE, ~0b111, true, false); // ID 0-7
   rxMailbox1.enableInterrupt();
   auto *canRxMailbox1 = rxMailbox1.getMailbox();
   canRxMailbox1->ID = CanId(CAN_MODE, CAN_RX_MAILBOX_1_ID);
   canRxMailbox1->CS = CanControlStatus(CanMessageCode_RxEmpty, CanFrameType_Remote);

   static auto rxMailbox2 = Can::allocateMailbox();
   rxMailbox2.setCallback(canRxMailboxCallback);
   rxMailbox2.getMailboxFilterMask() = CanMailboxFilterMask(CAN_MODE, ~0b11, true, false); // ID 0-3
   rxMailbox2.enableInterrupt();
   auto *canRxMailbox2 = rxMailbox2.getMailbox();
   canRxMailbox2->ID = CanId(CAN_MODE, CAN_RX_MAILBOX_2_ID);
   canRxMailbox2->CS = CanControlStatus(CanMessageCode_RxEmpty, CanFrameType_Remote);

   Can::start();

   Can::readTimer();

   // This loop monitors the transmit buffer and fills them at min 1 s intervals
   for(;;) {
      __asm__("nop");
      waitMS(1000);
      if (canTxMailbox1->CS.code == CanMessageCode_TxInactive) {
         canTxMailbox1->CS = CanControlStatus(CanMessageCode_TxData, CAN_MODE, CanDataSize_7, CanFrameType_Data);
      }
      waitMS(1000);
      if (canTxMailbox2->CS.code == CanMessageCode_TxInactive) {
         canTxMailbox2->CS = CanControlStatus(CanMessageCode_TxData, CAN_MODE, CanDataSize_3, CanFrameType_Data);
      }
      CanErrorCounts canErrorCounts = Can::getErrorCounters();
      if (canErrorCounts.receiveErrorCount > 0) {
         console.write("receiveErrorCount  = ").write(canErrorCounts.receiveErrorCount);
      }
      if (canErrorCounts.transmitFastErrorCount > 0) {
         console.write(", transmitErrorCount = ").writeln(canErrorCounts.transmitErrorCount);
      }
   }
}

int main() {
   console.writeln("\n\nStarting");

   console.write("NUM_MAILBOXES                 = ").writeln(Can::NUM_MAILBOXES);
   console.write("NUM_FIFO_MESSAGE_FILTERS      = ").writeln(Can::NUM_FIFO_MESSAGE_FILTERS);
   console.write("MAX_NUM_MESSAGE_BUFFERS       = ").writeln(Can::MAX_NUM_MESSAGE_BUFFERS);
   console.write("MAX_NUM_FIFO_MESSAGE_FILTERS  = ").writeln(Can::MAX_NUM_FIFO_MESSAGE_FILTERS);

   CanLimp::setInput();
   console.write("CAN Limp mode is ").writeln(CanLimp::read()?"Active":"Inactive");

   mailboxOnlyExample();

   return 0;
}

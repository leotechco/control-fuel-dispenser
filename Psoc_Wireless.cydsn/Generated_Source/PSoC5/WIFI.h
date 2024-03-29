/*******************************************************************************
* File Name: WIFI.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_WIFI_H)
#define CY_UART_WIFI_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define WIFI_RX_ENABLED                     (1u)
#define WIFI_TX_ENABLED                     (1u)
#define WIFI_HD_ENABLED                     (0u)
#define WIFI_RX_INTERRUPT_ENABLED           (0u)
#define WIFI_TX_INTERRUPT_ENABLED           (0u)
#define WIFI_INTERNAL_CLOCK_USED            (1u)
#define WIFI_RXHW_ADDRESS_ENABLED           (0u)
#define WIFI_OVER_SAMPLE_COUNT              (8u)
#define WIFI_PARITY_TYPE                    (0u)
#define WIFI_PARITY_TYPE_SW                 (0u)
#define WIFI_BREAK_DETECT                   (0u)
#define WIFI_BREAK_BITS_TX                  (13u)
#define WIFI_BREAK_BITS_RX                  (13u)
#define WIFI_TXCLKGEN_DP                    (1u)
#define WIFI_USE23POLLING                   (1u)
#define WIFI_FLOW_CONTROL                   (0u)
#define WIFI_CLK_FREQ                       (0u)
#define WIFI_TX_BUFFER_SIZE                 (4u)
#define WIFI_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define WIFI_CONTROL_REG_REMOVED            (0u)
#else
    #define WIFI_CONTROL_REG_REMOVED            (1u)
#endif /* End WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct WIFI_backupStruct_
{
    uint8 enableState;

    #if(WIFI_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End WIFI_CONTROL_REG_REMOVED */

} WIFI_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void WIFI_Start(void) ;
void WIFI_Stop(void) ;
uint8 WIFI_ReadControlRegister(void) ;
void WIFI_WriteControlRegister(uint8 control) ;

void WIFI_Init(void) ;
void WIFI_Enable(void) ;
void WIFI_SaveConfig(void) ;
void WIFI_RestoreConfig(void) ;
void WIFI_Sleep(void) ;
void WIFI_Wakeup(void) ;

/* Only if RX is enabled */
#if( (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) )

    #if (WIFI_RX_INTERRUPT_ENABLED)
        #define WIFI_EnableRxInt()  CyIntEnable (WIFI_RX_VECT_NUM)
        #define WIFI_DisableRxInt() CyIntDisable(WIFI_RX_VECT_NUM)
        CY_ISR_PROTO(WIFI_RXISR);
    #endif /* WIFI_RX_INTERRUPT_ENABLED */

    void WIFI_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void WIFI_SetRxAddress1(uint8 address) ;
    void WIFI_SetRxAddress2(uint8 address) ;

    void  WIFI_SetRxInterruptMode(uint8 intSrc) ;
    uint8 WIFI_ReadRxData(void) ;
    uint8 WIFI_ReadRxStatus(void) ;
    uint8 WIFI_GetChar(void) ;
    uint16 WIFI_GetByte(void) ;
    uint8 WIFI_GetRxBufferSize(void)
                                                            ;
    void WIFI_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define WIFI_GetRxInterruptSource   WIFI_ReadRxStatus

#endif /* End (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) */

/* Only if TX is enabled */
#if(WIFI_TX_ENABLED || WIFI_HD_ENABLED)

    #if(WIFI_TX_INTERRUPT_ENABLED)
        #define WIFI_EnableTxInt()  CyIntEnable (WIFI_TX_VECT_NUM)
        #define WIFI_DisableTxInt() CyIntDisable(WIFI_TX_VECT_NUM)
        #define WIFI_SetPendingTxInt() CyIntSetPending(WIFI_TX_VECT_NUM)
        #define WIFI_ClearPendingTxInt() CyIntClearPending(WIFI_TX_VECT_NUM)
        CY_ISR_PROTO(WIFI_TXISR);
    #endif /* WIFI_TX_INTERRUPT_ENABLED */

    void WIFI_SetTxInterruptMode(uint8 intSrc) ;
    void WIFI_WriteTxData(uint8 txDataByte) ;
    uint8 WIFI_ReadTxStatus(void) ;
    void WIFI_PutChar(uint8 txDataByte) ;
    void WIFI_PutString(const char8 string[]) ;
    void WIFI_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void WIFI_PutCRLF(uint8 txDataByte) ;
    void WIFI_ClearTxBuffer(void) ;
    void WIFI_SetTxAddressMode(uint8 addressMode) ;
    void WIFI_SendBreak(uint8 retMode) ;
    uint8 WIFI_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define WIFI_PutStringConst         WIFI_PutString
    #define WIFI_PutArrayConst          WIFI_PutArray
    #define WIFI_GetTxInterruptSource   WIFI_ReadTxStatus

#endif /* End WIFI_TX_ENABLED || WIFI_HD_ENABLED */

#if(WIFI_HD_ENABLED)
    void WIFI_LoadRxConfig(void) ;
    void WIFI_LoadTxConfig(void) ;
#endif /* End WIFI_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WIFI) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    WIFI_CyBtldrCommStart(void) CYSMALL ;
    void    WIFI_CyBtldrCommStop(void) CYSMALL ;
    void    WIFI_CyBtldrCommReset(void) CYSMALL ;
    cystatus WIFI_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus WIFI_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WIFI)
        #define CyBtldrCommStart    WIFI_CyBtldrCommStart
        #define CyBtldrCommStop     WIFI_CyBtldrCommStop
        #define CyBtldrCommReset    WIFI_CyBtldrCommReset
        #define CyBtldrCommWrite    WIFI_CyBtldrCommWrite
        #define CyBtldrCommRead     WIFI_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_WIFI) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define WIFI_BYTE2BYTE_TIME_OUT (25u)
    #define WIFI_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define WIFI_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define WIFI_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define WIFI_SET_SPACE      (0x00u)
#define WIFI_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (WIFI_TX_ENABLED) || (WIFI_HD_ENABLED) )
    #if(WIFI_TX_INTERRUPT_ENABLED)
        #define WIFI_TX_VECT_NUM            (uint8)WIFI_TXInternalInterrupt__INTC_NUMBER
        #define WIFI_TX_PRIOR_NUM           (uint8)WIFI_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* WIFI_TX_INTERRUPT_ENABLED */

    #define WIFI_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define WIFI_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define WIFI_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(WIFI_TX_ENABLED)
        #define WIFI_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (WIFI_HD_ENABLED) */
        #define WIFI_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (WIFI_TX_ENABLED) */

    #define WIFI_TX_STS_COMPLETE            (uint8)(0x01u << WIFI_TX_STS_COMPLETE_SHIFT)
    #define WIFI_TX_STS_FIFO_EMPTY          (uint8)(0x01u << WIFI_TX_STS_FIFO_EMPTY_SHIFT)
    #define WIFI_TX_STS_FIFO_FULL           (uint8)(0x01u << WIFI_TX_STS_FIFO_FULL_SHIFT)
    #define WIFI_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << WIFI_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (WIFI_TX_ENABLED) || (WIFI_HD_ENABLED)*/

#if( (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) )
    #if(WIFI_RX_INTERRUPT_ENABLED)
        #define WIFI_RX_VECT_NUM            (uint8)WIFI_RXInternalInterrupt__INTC_NUMBER
        #define WIFI_RX_PRIOR_NUM           (uint8)WIFI_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* WIFI_RX_INTERRUPT_ENABLED */
    #define WIFI_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define WIFI_RX_STS_BREAK_SHIFT             (0x01u)
    #define WIFI_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define WIFI_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define WIFI_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define WIFI_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define WIFI_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define WIFI_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define WIFI_RX_STS_MRKSPC           (uint8)(0x01u << WIFI_RX_STS_MRKSPC_SHIFT)
    #define WIFI_RX_STS_BREAK            (uint8)(0x01u << WIFI_RX_STS_BREAK_SHIFT)
    #define WIFI_RX_STS_PAR_ERROR        (uint8)(0x01u << WIFI_RX_STS_PAR_ERROR_SHIFT)
    #define WIFI_RX_STS_STOP_ERROR       (uint8)(0x01u << WIFI_RX_STS_STOP_ERROR_SHIFT)
    #define WIFI_RX_STS_OVERRUN          (uint8)(0x01u << WIFI_RX_STS_OVERRUN_SHIFT)
    #define WIFI_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << WIFI_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define WIFI_RX_STS_ADDR_MATCH       (uint8)(0x01u << WIFI_RX_STS_ADDR_MATCH_SHIFT)
    #define WIFI_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << WIFI_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define WIFI_RX_HW_MASK                     (0x7Fu)
#endif /* End (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) */

/* Control Register definitions */
#define WIFI_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define WIFI_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define WIFI_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define WIFI_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define WIFI_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define WIFI_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define WIFI_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define WIFI_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define WIFI_CTRL_HD_SEND               (uint8)(0x01u << WIFI_CTRL_HD_SEND_SHIFT)
#define WIFI_CTRL_HD_SEND_BREAK         (uint8)(0x01u << WIFI_CTRL_HD_SEND_BREAK_SHIFT)
#define WIFI_CTRL_MARK                  (uint8)(0x01u << WIFI_CTRL_MARK_SHIFT)
#define WIFI_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << WIFI_CTRL_PARITY_TYPE0_SHIFT)
#define WIFI_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << WIFI_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define WIFI_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define WIFI_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define WIFI_SEND_BREAK                         (0x00u)
#define WIFI_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define WIFI_REINIT                             (0x02u)
#define WIFI_SEND_WAIT_REINIT                   (0x03u)

#define WIFI_OVER_SAMPLE_8                      (8u)
#define WIFI_OVER_SAMPLE_16                     (16u)

#define WIFI_BIT_CENTER                         (WIFI_OVER_SAMPLE_COUNT - 2u)

#define WIFI_FIFO_LENGTH                        (4u)
#define WIFI_NUMBER_OF_START_BIT                (1u)
#define WIFI_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define WIFI_TXBITCTR_BREAKBITS8X   ((WIFI_BREAK_BITS_TX * WIFI_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define WIFI_TXBITCTR_BREAKBITS ((WIFI_BREAK_BITS_TX * WIFI_OVER_SAMPLE_COUNT) - 1u)

#define WIFI_HALF_BIT_COUNT   \
                            (((WIFI_OVER_SAMPLE_COUNT / 2u) + (WIFI_USE23POLLING * 1u)) - 2u)
#if (WIFI_OVER_SAMPLE_COUNT == WIFI_OVER_SAMPLE_8)
    #define WIFI_HD_TXBITCTR_INIT   (((WIFI_BREAK_BITS_TX + \
                            WIFI_NUMBER_OF_START_BIT) * WIFI_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define WIFI_RXBITCTR_INIT  ((((WIFI_BREAK_BITS_RX + WIFI_NUMBER_OF_START_BIT) \
                            * WIFI_OVER_SAMPLE_COUNT) + WIFI_HALF_BIT_COUNT) - 1u)

#else /* WIFI_OVER_SAMPLE_COUNT == WIFI_OVER_SAMPLE_16 */
    #define WIFI_HD_TXBITCTR_INIT   ((8u * WIFI_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define WIFI_RXBITCTR_INIT      (((7u * WIFI_OVER_SAMPLE_COUNT) - 1u) + \
                                                      WIFI_HALF_BIT_COUNT)
#endif /* End WIFI_OVER_SAMPLE_COUNT */

#define WIFI_HD_RXBITCTR_INIT                   WIFI_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 WIFI_initVar;
#if (WIFI_TX_INTERRUPT_ENABLED && WIFI_TX_ENABLED)
    extern volatile uint8 WIFI_txBuffer[WIFI_TX_BUFFER_SIZE];
    extern volatile uint8 WIFI_txBufferRead;
    extern uint8 WIFI_txBufferWrite;
#endif /* (WIFI_TX_INTERRUPT_ENABLED && WIFI_TX_ENABLED) */
#if (WIFI_RX_INTERRUPT_ENABLED && (WIFI_RX_ENABLED || WIFI_HD_ENABLED))
    extern uint8 WIFI_errorStatus;
    extern volatile uint8 WIFI_rxBuffer[WIFI_RX_BUFFER_SIZE];
    extern volatile uint8 WIFI_rxBufferRead;
    extern volatile uint8 WIFI_rxBufferWrite;
    extern volatile uint8 WIFI_rxBufferLoopDetect;
    extern volatile uint8 WIFI_rxBufferOverflow;
    #if (WIFI_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 WIFI_rxAddressMode;
        extern volatile uint8 WIFI_rxAddressDetected;
    #endif /* (WIFI_RXHW_ADDRESS_ENABLED) */
#endif /* (WIFI_RX_INTERRUPT_ENABLED && (WIFI_RX_ENABLED || WIFI_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define WIFI__B_UART__AM_SW_BYTE_BYTE 1
#define WIFI__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define WIFI__B_UART__AM_HW_BYTE_BY_BYTE 3
#define WIFI__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define WIFI__B_UART__AM_NONE 0

#define WIFI__B_UART__NONE_REVB 0
#define WIFI__B_UART__EVEN_REVB 1
#define WIFI__B_UART__ODD_REVB 2
#define WIFI__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define WIFI_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define WIFI_NUMBER_OF_STOP_BITS    (1u)

#if (WIFI_RXHW_ADDRESS_ENABLED)
    #define WIFI_RX_ADDRESS_MODE    (0u)
    #define WIFI_RX_HW_ADDRESS1     (0u)
    #define WIFI_RX_HW_ADDRESS2     (0u)
#endif /* (WIFI_RXHW_ADDRESS_ENABLED) */

#define WIFI_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << WIFI_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << WIFI_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << WIFI_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << WIFI_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << WIFI_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << WIFI_RX_STS_BREAK_SHIFT) \
                                        | (0 << WIFI_RX_STS_OVERRUN_SHIFT))

#define WIFI_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << WIFI_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << WIFI_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << WIFI_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << WIFI_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define WIFI_CONTROL_REG \
                            (* (reg8 *) WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define WIFI_CONTROL_PTR \
                            (  (reg8 *) WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(WIFI_TX_ENABLED)
    #define WIFI_TXDATA_REG          (* (reg8 *) WIFI_BUART_sTX_TxShifter_u0__F0_REG)
    #define WIFI_TXDATA_PTR          (  (reg8 *) WIFI_BUART_sTX_TxShifter_u0__F0_REG)
    #define WIFI_TXDATA_AUX_CTL_REG  (* (reg8 *) WIFI_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define WIFI_TXDATA_AUX_CTL_PTR  (  (reg8 *) WIFI_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define WIFI_TXSTATUS_REG        (* (reg8 *) WIFI_BUART_sTX_TxSts__STATUS_REG)
    #define WIFI_TXSTATUS_PTR        (  (reg8 *) WIFI_BUART_sTX_TxSts__STATUS_REG)
    #define WIFI_TXSTATUS_MASK_REG   (* (reg8 *) WIFI_BUART_sTX_TxSts__MASK_REG)
    #define WIFI_TXSTATUS_MASK_PTR   (  (reg8 *) WIFI_BUART_sTX_TxSts__MASK_REG)
    #define WIFI_TXSTATUS_ACTL_REG   (* (reg8 *) WIFI_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define WIFI_TXSTATUS_ACTL_PTR   (  (reg8 *) WIFI_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(WIFI_TXCLKGEN_DP)
        #define WIFI_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define WIFI_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define WIFI_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define WIFI_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define WIFI_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define WIFI_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define WIFI_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define WIFI_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define WIFI_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define WIFI_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) WIFI_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* WIFI_TXCLKGEN_DP */

#endif /* End WIFI_TX_ENABLED */

#if(WIFI_HD_ENABLED)

    #define WIFI_TXDATA_REG             (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__F1_REG )
    #define WIFI_TXDATA_PTR             (  (reg8 *) WIFI_BUART_sRX_RxShifter_u0__F1_REG )
    #define WIFI_TXDATA_AUX_CTL_REG     (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define WIFI_TXDATA_AUX_CTL_PTR     (  (reg8 *) WIFI_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define WIFI_TXSTATUS_REG           (* (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_REG )
    #define WIFI_TXSTATUS_PTR           (  (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_REG )
    #define WIFI_TXSTATUS_MASK_REG      (* (reg8 *) WIFI_BUART_sRX_RxSts__MASK_REG )
    #define WIFI_TXSTATUS_MASK_PTR      (  (reg8 *) WIFI_BUART_sRX_RxSts__MASK_REG )
    #define WIFI_TXSTATUS_ACTL_REG      (* (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define WIFI_TXSTATUS_ACTL_PTR      (  (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End WIFI_HD_ENABLED */

#if( (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) )
    #define WIFI_RXDATA_REG             (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__F0_REG )
    #define WIFI_RXDATA_PTR             (  (reg8 *) WIFI_BUART_sRX_RxShifter_u0__F0_REG )
    #define WIFI_RXADDRESS1_REG         (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__D0_REG )
    #define WIFI_RXADDRESS1_PTR         (  (reg8 *) WIFI_BUART_sRX_RxShifter_u0__D0_REG )
    #define WIFI_RXADDRESS2_REG         (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__D1_REG )
    #define WIFI_RXADDRESS2_PTR         (  (reg8 *) WIFI_BUART_sRX_RxShifter_u0__D1_REG )
    #define WIFI_RXDATA_AUX_CTL_REG     (* (reg8 *) WIFI_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define WIFI_RXBITCTR_PERIOD_REG    (* (reg8 *) WIFI_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define WIFI_RXBITCTR_PERIOD_PTR    (  (reg8 *) WIFI_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define WIFI_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) WIFI_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define WIFI_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) WIFI_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define WIFI_RXBITCTR_COUNTER_REG   (* (reg8 *) WIFI_BUART_sRX_RxBitCounter__COUNT_REG )
    #define WIFI_RXBITCTR_COUNTER_PTR   (  (reg8 *) WIFI_BUART_sRX_RxBitCounter__COUNT_REG )

    #define WIFI_RXSTATUS_REG           (* (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_REG )
    #define WIFI_RXSTATUS_PTR           (  (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_REG )
    #define WIFI_RXSTATUS_MASK_REG      (* (reg8 *) WIFI_BUART_sRX_RxSts__MASK_REG )
    #define WIFI_RXSTATUS_MASK_PTR      (  (reg8 *) WIFI_BUART_sRX_RxSts__MASK_REG )
    #define WIFI_RXSTATUS_ACTL_REG      (* (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define WIFI_RXSTATUS_ACTL_PTR      (  (reg8 *) WIFI_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) */

#if(WIFI_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define WIFI_INTCLOCK_CLKEN_REG     (* (reg8 *) WIFI_IntClock__PM_ACT_CFG)
    #define WIFI_INTCLOCK_CLKEN_PTR     (  (reg8 *) WIFI_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define WIFI_INTCLOCK_CLKEN_MASK    WIFI_IntClock__PM_ACT_MSK
#endif /* End WIFI_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(WIFI_TX_ENABLED)
    #define WIFI_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End WIFI_TX_ENABLED */

#if(WIFI_HD_ENABLED)
    #define WIFI_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End WIFI_HD_ENABLED */

#if( (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) )
    #define WIFI_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define WIFI_WAIT_1_MS      WIFI_BL_CHK_DELAY_MS   

#define WIFI_TXBUFFERSIZE   WIFI_TX_BUFFER_SIZE
#define WIFI_RXBUFFERSIZE   WIFI_RX_BUFFER_SIZE

#if (WIFI_RXHW_ADDRESS_ENABLED)
    #define WIFI_RXADDRESSMODE  WIFI_RX_ADDRESS_MODE
    #define WIFI_RXHWADDRESS1   WIFI_RX_HW_ADDRESS1
    #define WIFI_RXHWADDRESS2   WIFI_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define WIFI_RXAddressMode  WIFI_RXADDRESSMODE
#endif /* (WIFI_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define WIFI_initvar                    WIFI_initVar

#define WIFI_RX_Enabled                 WIFI_RX_ENABLED
#define WIFI_TX_Enabled                 WIFI_TX_ENABLED
#define WIFI_HD_Enabled                 WIFI_HD_ENABLED
#define WIFI_RX_IntInterruptEnabled     WIFI_RX_INTERRUPT_ENABLED
#define WIFI_TX_IntInterruptEnabled     WIFI_TX_INTERRUPT_ENABLED
#define WIFI_InternalClockUsed          WIFI_INTERNAL_CLOCK_USED
#define WIFI_RXHW_Address_Enabled       WIFI_RXHW_ADDRESS_ENABLED
#define WIFI_OverSampleCount            WIFI_OVER_SAMPLE_COUNT
#define WIFI_ParityType                 WIFI_PARITY_TYPE

#if( WIFI_TX_ENABLED && (WIFI_TXBUFFERSIZE > WIFI_FIFO_LENGTH))
    #define WIFI_TXBUFFER               WIFI_txBuffer
    #define WIFI_TXBUFFERREAD           WIFI_txBufferRead
    #define WIFI_TXBUFFERWRITE          WIFI_txBufferWrite
#endif /* End WIFI_TX_ENABLED */
#if( ( WIFI_RX_ENABLED || WIFI_HD_ENABLED ) && \
     (WIFI_RXBUFFERSIZE > WIFI_FIFO_LENGTH) )
    #define WIFI_RXBUFFER               WIFI_rxBuffer
    #define WIFI_RXBUFFERREAD           WIFI_rxBufferRead
    #define WIFI_RXBUFFERWRITE          WIFI_rxBufferWrite
    #define WIFI_RXBUFFERLOOPDETECT     WIFI_rxBufferLoopDetect
    #define WIFI_RXBUFFER_OVERFLOW      WIFI_rxBufferOverflow
#endif /* End WIFI_RX_ENABLED */

#ifdef WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define WIFI_CONTROL                WIFI_CONTROL_REG
#endif /* End WIFI_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(WIFI_TX_ENABLED)
    #define WIFI_TXDATA                 WIFI_TXDATA_REG
    #define WIFI_TXSTATUS               WIFI_TXSTATUS_REG
    #define WIFI_TXSTATUS_MASK          WIFI_TXSTATUS_MASK_REG
    #define WIFI_TXSTATUS_ACTL          WIFI_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(WIFI_TXCLKGEN_DP)
        #define WIFI_TXBITCLKGEN_CTR        WIFI_TXBITCLKGEN_CTR_REG
        #define WIFI_TXBITCLKTX_COMPLETE    WIFI_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define WIFI_TXBITCTR_PERIOD        WIFI_TXBITCTR_PERIOD_REG
        #define WIFI_TXBITCTR_CONTROL       WIFI_TXBITCTR_CONTROL_REG
        #define WIFI_TXBITCTR_COUNTER       WIFI_TXBITCTR_COUNTER_REG
    #endif /* WIFI_TXCLKGEN_DP */
#endif /* End WIFI_TX_ENABLED */

#if(WIFI_HD_ENABLED)
    #define WIFI_TXDATA                 WIFI_TXDATA_REG
    #define WIFI_TXSTATUS               WIFI_TXSTATUS_REG
    #define WIFI_TXSTATUS_MASK          WIFI_TXSTATUS_MASK_REG
    #define WIFI_TXSTATUS_ACTL          WIFI_TXSTATUS_ACTL_REG
#endif /* End WIFI_HD_ENABLED */

#if( (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) )
    #define WIFI_RXDATA                 WIFI_RXDATA_REG
    #define WIFI_RXADDRESS1             WIFI_RXADDRESS1_REG
    #define WIFI_RXADDRESS2             WIFI_RXADDRESS2_REG
    #define WIFI_RXBITCTR_PERIOD        WIFI_RXBITCTR_PERIOD_REG
    #define WIFI_RXBITCTR_CONTROL       WIFI_RXBITCTR_CONTROL_REG
    #define WIFI_RXBITCTR_COUNTER       WIFI_RXBITCTR_COUNTER_REG
    #define WIFI_RXSTATUS               WIFI_RXSTATUS_REG
    #define WIFI_RXSTATUS_MASK          WIFI_RXSTATUS_MASK_REG
    #define WIFI_RXSTATUS_ACTL          WIFI_RXSTATUS_ACTL_REG
#endif /* End  (WIFI_RX_ENABLED) || (WIFI_HD_ENABLED) */

#if(WIFI_INTERNAL_CLOCK_USED)
    #define WIFI_INTCLOCK_CLKEN         WIFI_INTCLOCK_CLKEN_REG
#endif /* End WIFI_INTERNAL_CLOCK_USED */

#define WIFI_WAIT_FOR_COMLETE_REINIT    WIFI_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_WIFI_H */


/* [] END OF FILE */

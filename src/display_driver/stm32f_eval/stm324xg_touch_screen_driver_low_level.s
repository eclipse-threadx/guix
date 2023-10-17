;/**************************************************************************/
;/*                                                                        */
;/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
;/*                                                                        */
;/**************************************************************************/
;
;
;/**************************************************************************/
;/**************************************************************************/
;/**                                                                       */ 
;/** ThreadX Component                                                     */ 
;/**                                                                       */
;/**   Initialize                                                          */
;/**                                                                       */
;/**************************************************************************/
;/**************************************************************************/
;
;#define TX_SOURCE_CODE
;
;
        EXTERN  _tx_thread_context_save
        EXTERN  _tx_thread_context_restore
        EXTERN  dma1_stream0_isr

        SECTION `.text`:CODE:NOROOT(2)
        THUMB
;/**************************************************************************/ 
;/*                                                                        */ 
;/*  FUNCTION                                               RELEASE        */ 
;/*                                                                        */ 
;/*    __nx_driver_stm32f4xx_ethernet_isr                STM32F4xx/IAR     */ 
;/*                                                           6.0          */
;/*  AUTHOR                                                                */
;/*                                                                        */
;/*    Kenneth Maxwell, Microsoft Corporation                              */
;/*                                                                        */
;/*  DESCRIPTION                                                           */ 
;/*                                                                        */ 
;/*    This function is responsible for fielding the etherent interrupts   */ 
;/*    of the STM32F4xx.                                                   */ 
;/*                                                                        */ 
;/*  INPUT                                                                 */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  OUTPUT                                                                */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  CALLS                                                                 */ 
;/*                                                                        */ 
;/*    dma1_stream0_isr                      NetX driver ethernet ISR      */ 
;/*                                                                        */ 
;/*  CALLED BY                                                             */ 
;/*                                                                        */ 
;/*    _tx_initialize_kernel_enter           ThreadX entry function        */ 
;/*                                                                        */ 
;/*  RELEASE HISTORY                                                       */ 
;/*                                                                        */ 
;/*    DATE              NAME                      DESCRIPTION             */
;/*                                                                        */
;/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
;/*                                                                        */
;/**************************************************************************/
        PUBLIC  DMA1_Stream0_IRQHandler
DMA1_Stream0_IRQHandler:
        PUSH    {lr}
        BL      _tx_thread_context_save
        BL      dma1_stream0_isr
        B       _tx_thread_context_restore

        END
        

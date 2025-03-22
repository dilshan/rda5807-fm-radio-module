//-----------------------------------------------------------------------------
// STM8S103F2, STM8S103F3, STM8S103K3 TIMER4 definitions 
// for SDCC.
// 
// Based on STM8S103 datasheet DocID15441 Rev 14.
// Last updated: Dilshan Jayakody [7th Oct 2020]
//
// Update log:
// [07/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef STM8S103_TIMER4_DEF_H
#define STM8S103_TIMER4_DEF_H

#define TIM4_CR1_RESET_VALUE    ((unsigned char)0x00)   // CR1 reset value. 
#define TIM4_IER_RESET_VALUE    ((unsigned char)0x00)   // IER reset value. 
#define TIM4_SR1_RESET_VALUE    ((unsigned char)0x00)   // SR1 reset value. 
#define TIM4_EGR_RESET_VALUE    ((unsigned char)0x00)   // EGR reset value. 
#define TIM4_CNTR_RESET_VALUE   ((unsigned char)0x00)   // CNTR reset value. 
#define TIM4_PSCR_RESET_VALUE   ((unsigned char)0x00)   // PSCR reset value. 
#define TIM4_ARR_RESET_VALUE    ((unsigned char)0xFF)   // ARR reset value. 

#define TIM4_CR1_ARPE   ((unsigned char)0x80)   // Auto-Reload Preload Enable mask.
#define TIM4_CR1_OPM    ((unsigned char)0x08)   // One Pulse Mode mask.
#define TIM4_CR1_URS    ((unsigned char)0x04)   // Update Request Source mask.
#define TIM4_CR1_UDIS   ((unsigned char)0x02)   // Update DIsable mask.
#define TIM4_CR1_CEN    ((unsigned char)0x01)   // Counter Enable mask.

#define TIM4_IER_UIE    ((unsigned char)0x01)   // Update Interrupt Enable mask.

#define TIM4_SR1_UIF    ((unsigned char)0x01)   // Update Interrupt Flag mask.

#define TIM4_EGR_UG     ((unsigned char)0x01)   // Update Generation mask.

#define TIM4_PSCR_PSC   ((unsigned char)0x07)   // Prescaler Value  mask.

#endif /* STM8S103_TIMER4_DEF_H */
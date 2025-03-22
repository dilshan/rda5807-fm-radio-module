//-----------------------------------------------------------------------------
// STM8S103F2, STM8S103F3, STM8S103K3 instruction mapping 
// header file for SDCC.
//
// Based on STM8S103 datasheet DocID15441 Rev 14.
// Last updated: Dilshan Jayakody [5th Oct 2020]
//
// Update log:
// [05/10/2020] - Initial version - Dilshan Jayakody.
//-----------------------------------------------------------------------------

#ifndef STM8S103_INST_H
#define STM8S103_INST_H

// Interrupts related instruction mappings.

#define sei()   {__asm__("rim\n");}     // Enable interrupts.
#define cli()   {__asm__("sim\n");}     // Disable interrupts.
#define wfi()   {__asm__("wfi\n");}     // Wait for interrupt.

// CPU related instruction mappings.

#define nop()   {__asm__("nop\n");}     // No operation.
#define trap()  {__asm__("trap\n");}    // Trap.
#define halt()  {__asm__("halt\n");}    // Halt.

#endif /* STM8S103_INST_H */
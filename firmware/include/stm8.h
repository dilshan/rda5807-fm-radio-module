//-----------------------------------------------------------------------------
// STM8S base header file.
//
// Last updated: Dilshan Jayakody [1st Nov 2020]
//
// Update log:
// [07/10/2020] - Initial version - Dilshan Jayakody.
// [01/11/2020] - Add support for STM8S001J3 MCU.
//-----------------------------------------------------------------------------

#ifndef STM8S_MAIN_H
#define STM8S_MAIN_H

#include "inst.h"

#if defined(STM8S103F2) || defined(STM8S103F3) || defined(STM8S103K3)
#include "stm8s103f3/stm8.h"
#endif

#if defined(STM8S003F3) || defined(STM8S003K3)
#include "stm8s003f3/stm8.h"
#endif

#if defined(STM8S001J3)
#include "stm8s001j3/stm8.h"
#endif

#endif /* STM8S_MAIN_H */
#ifndef __SRAM_H_
#define __SRAM_H_

#include <stm32f4xx.h>
#include "type.h"

#define  FSMC_Bank1_BCR   FSMC_Bank1->BTCR[4]
#define  FSMC_Bank1_BTR   FSMC_Bank1->BTCR[5]

extern void SRAM_Init(void);

#endif




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MM32F10x_IT_H
#define __MM32F10x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* __MM32F10x_IT_H */


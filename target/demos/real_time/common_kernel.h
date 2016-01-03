#ifndef COMMON_KERNEL_H
#define COMMON_KERNEL_H

/* Common defines used in kernel drivers */

#include <asm/gpio.h>
#ifdef CONFIG_ARCH_MX2
#include <mach/iomux-mx1-mx2.h>
#endif

#include "common.h"



#ifdef CONFIG_ARCH_MX2
// CSI PIXCLK
#define PULSE_OUTPUT_GPIO 	(GPIO_PORTB | 16)
#define INTERRUPT_OUTPUT_GPIO	(GPIO_PORTB | 16)
#define INTERRUPT_INPUT_NB	IRQ_GPIOB(15)
#define INTERRUPT_INPUT_GPIO	(GPIO_PORTB | 15)
#else
#define PULSE_OUTPUT_GPIO 	(GPIO_PORTA | 4)
#define INTERRUPT_OUTPUT_GPIO	(GPIO_PORTA | 9)
#define INTERRUPT_INPUT_GPIO	(GPIO_PORTA | 6)
#define INTERRUPT_INPUT_NB	IRQ_GPIOA(6)
#endif

#endif /* COMMON_KERNEL_H */

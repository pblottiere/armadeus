#ifndef COMMON_H
#define COMMON_H

/* Common defines used in userspace applications */
#ifdef CONFIG_ARCH_MX2
#define INTERRUPT_OUTPUT_DEV "/dev/gpio/PB16"
#define INTERRUPT_INPUT_DEV "/dev/gpio/PB15"

#define PULSE_OUTPUT_DEV "/dev/gpio/PB16"
#else
#define INTERRUPT_OUTPUT_DEV "/dev/gpio/PA9"
#define INTERRUPT_INPUT_DEV "/dev/gpio/PA6"

#define PULSE_OUTPUT_DEV "/dev/gpio/PA4"
#endif
#define TIMESLEEP 10000
#define TASK_PRIO 99 /* Highest RT priority */
#define TASK_MODE 0 /* No flags */
#define TASK_STKSZ 0 /* Stack size (use default one) */

#endif /* COMMON_H */

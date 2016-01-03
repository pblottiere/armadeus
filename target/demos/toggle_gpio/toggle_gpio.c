#include <stdio.h>
#include <stdlib.h>
#include <as_gpio.h>

#define LED_GPIO	174	/* for APF27Dev */
/* #define LED_GPIO	2	 for APF51Dev */

int main(int argc, char *argv[])
{
    struct as_gpio_device *led;
    
    led = as_gpio_open(LED_GPIO);
    if (led == NULL) {
        printf("Error: can't open gpio\n");
        exit(1);
    }

    as_gpio_set_pin_direction(led, "out");

    while (1) {
        as_gpio_set_pin_value(led, 0);
        as_gpio_set_pin_value(led, 1);
    }

    as_gpio_close(led);
    exit(0);
}

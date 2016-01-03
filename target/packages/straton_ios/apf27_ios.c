#include <stdio.h>
#include <stdlib.h>
#include <as_gpio.h>
#include "apf27_ios.h"

static struct as_gpio_device *led;
static struct as_gpio_device *button;

/* set and get specifics functions */
void led_setvalue(long value) {
    if (value == 0)
        as_gpio_set_pin_value(led, 1);
    else
        as_gpio_set_pin_value(led, 0);
    return;
}

long button_getvalue(void){
    return (long)!as_gpio_get_pin_value(button);
}

/* straton api */
int straton_ios_init(void) {
    /* init BUTTON (portF13)*/
    button = as_gpio_open(173);
    if (button == NULL) {
        printf("IO error: Can't initialize button\n");
        goto error;
    }
    as_gpio_set_pin_direction(button, "in");

    /* init LED (portF14)*/
    led = as_gpio_open(174);
    if (led == NULL) {
        printf("IO error: Can't initialize led\n");
        goto close_button_error;
    }
    as_gpio_set_pin_direction(led, "out");
    return 0;

/*close_led_error:*/
    as_gpio_close(led);
close_button_error:
    as_gpio_close(button);
error:
    return -1;
}

void straton_ios_close(void) {
    printf("Straton_ios: close\n");
    /* close BUTTON */
    as_gpio_close(button);
    /* close LED */
    as_gpio_close(led);
    return;
}

char * straton_ios_get_profile_name(void) {
    char *profile_name = PROFIL_NAME;
    return profile_name;
}

int straton_ios_exchange(str_param *pParams, int index,
                         long value_in, long *value_out) {

    switch (pParams->type)
    {
        case IO_LED:
            led_setvalue(value_in);
            return EXCHANGE_SET;
        case IO_BUTTON:
            *value_out = button_getvalue();
            return EXCHANGE_GET;
        default :
            printf("Error unknown type : %d, index %d\n", pParams->type, pParams->index);
            return -1;
    }
    return 0;
}

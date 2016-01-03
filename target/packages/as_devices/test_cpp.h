/*
**    THE ARMadeus Systems
** 
**    Copyright (C) 2009  The armadeus systems team 
**    Fabien Marteau <fabien.marteau@armadeus.com>
** 
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
** 
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <math.h>

#include "as_i2c.hpp"
#include "as_gpio.hpp"
#include "as_adc.hpp"
#include "as_dac.hpp"
#include "as_spi.hpp"
#include "as_pwm.hpp"
#include "as_93lcxx.hpp"
#include "as_backlight.hpp"
#include "as_i2c_eeprom.hpp"

#define _STR(x) #x

void pressEnterToContinue(void)
{
    printf("Press enter to continue\n");
    getc(stdin);//XXX
    std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
}


/* i2c bus test */
void test_i2c(void)
{
    char buffer[20];
    int value;
    int initialized = 0;

    AsI2c *i2c_bus = NULL;
    int i2c_id = 0;

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("*********************************\n");
        printf("*   Testing i2c menu            *\n");
        printf("*********************************\n");
        printf("Choose ('q' to quit):\n");
        if (initialized == 0)
        {
            printf(" 1) Open i2c bus \n");
            printf(" 2) Change bus number (%d)\n",i2c_id);
        } else {
            printf(" 1) Close i2c bus \n");
        }
        printf("> ");
        scanf("%s",buffer);

        if (initialized == 0)
        {
            switch(buffer[0])
            {
                case '1' :  printf("Open i2c bus\n");
                            i2c_bus = new AsI2c(i2c_id);
                            if (i2c_bus == NULL)
                            {
                                printf("Error can't open i2c bus\n");
                            } else {
                                printf("Bus %d opened\n", i2c_id);
                                initialized = 1;
                            }
                            pressEnterToContinue();
                            break;
                case '2' :  printf("Give bus number (max %d): ", sizeof(_STR(AS_I2C_DEV_COUNT))-1);
                            scanf("%d",&value);
                            if ((value >= (signed int)sizeof(_STR(AS_I2C_DEV_COUNT))) || (value < 0))
                            {
                                printf(" Wrong value\n");
                            } else {
                                i2c_id = value;
                            }
                            pressEnterToContinue();
                default : break;
            }
        } else {
            switch(buffer[0])
            {
                case '1' :  printf("Close i2c bus\n");
                            delete i2c_bus;
                            initialized = 0;

                            pressEnterToContinue();
                            break;
                default : break;
            }
        }
    }
}

/* spi bus test */
void test_spi(void)
{
    printf("TODO\n");
    pressEnterToContinue();
}

void test_gpio()
{
    char buffer[50];
    AsGpio *gpio_dev = NULL;
    int32_t ret;
    int32_t value;
    int pin_num = (5 *32) + 13;

    gpio_dev = new AsGpio(pin_num);
    if (gpio_dev == NULL)
    {
        printf("Error can't open gpio %d\nHave you run loadgpio.sh ?\n", pin_num);
        pressEnterToContinue();
        return ;
    }
    ret = gpio_dev->getPinValue();
    if (ret < 0)
    {
        printf("Error, can't get pin value\n");
        return;
    }

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("**************************\n");
        printf("   Testing GPIO %d \n", (int)gpio_dev->getPinNum());
        printf("**************************\n");
        printf("Choose ('q' to quit):\n");
        printf(" 1) Change GPIO number (%d)\n", (int)gpio_dev->getPinNum());
        printf(" 2) Change direction (%s)\n", gpio_dev->getPinDirection());
        printf(" 3) Change value (%d)\n", (int)gpio_dev->getPinValue());
        printf(" 4) Read pin value\n");
        printf(" 5) Set irq mode (%s)\n", gpio_dev->getIrqMode());
        printf(" a) blocking read\n");

        printf("> ");
        scanf("%s",buffer);

        switch (buffer[0])
        {
            case '1' :
                        printf("Give GPIO number (port index (A==0) x 32 + pin index): ");
                        scanf("%d", &pin_num);
                        gpio_dev = new AsGpio(pin_num);
                        if(gpio_dev == NULL)
                        {
                            printf("Error, can't open %d\n", pin_num);
                            pressEnterToContinue();
                            break;
                        }
                        printf("Ok %d is open\n", pin_num);
                        pressEnterToContinue();
                        break;
            case '2' :  printf("Give direction (in / out) : ");
                        scanf("%s", buffer);
                        ret = gpio_dev->setPinDirection(buffer);
                        if (ret < 0)
                        {
                            printf("Error, can't change direction\n");
                            pressEnterToContinue();
                            break;
                        }
                        printf("Ok direction changed\n");
                        pressEnterToContinue();
                        break;
            case '3' :  printf("Give value : ");
                        scanf("%d", &value);
                        ret = gpio_dev->setPinValue(value);
                        if(ret < 0)
                        {
                            printf("Error, can't change pin value\n");
                            pressEnterToContinue();
                            break;
                        }
                        printf("Ok value changed\n");
                        pressEnterToContinue();
                        break;
            case '4' :  printf("Get value \n");
                        ret = gpio_dev->getPinValue();
                        if (ret < 0)
                        {
                            printf("Error, can't get pin value\n");
                            pressEnterToContinue();
                            break;
                        }
                        printf("Value is %d\n", ret);
                        pressEnterToContinue();
                        break;
            case '5' :
			printf("rising / falling / both / none\n");
                        printf("Please choose a mode : ");
                        scanf("%s", buffer);
                        ret = gpio_dev->setIrqMode(buffer);
                        if (ret < 0)
                        {
                            printf("Error, can't change irq value\n");
                            pressEnterToContinue();
                            break;
                        }
                        printf("Ok value changed\n");
                        pressEnterToContinue();
                        break;
            case 'a' :  printf("Blocking read (10 sec timeout)\n");
                        ret = gpio_dev->waitEvent(10000);
                        if (ret < 0)
                        {
                            printf("Error, can't read value\n");
                            pressEnterToContinue();
                            break;
                        }
                        printf("Value read %d\n",ret);
                        pressEnterToContinue();
                        break;
            default : break;
        }
    }
    delete gpio_dev;
}


#define NUM_OF_CHANNEL (8)

void test_max1027()
{
    char buffer[50];
    int ret;
    int value;
    float fValue;
    AsAdc *max1027_dev = NULL;
    int channel=0;
    float vRef = 2500;
    int devNumber = 0;
    int initialized = 0;

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("**************************\n");
        printf("   Testing max1027       *\n");
        printf("**************************\n");
        printf("Choose ('q' to quit):\n");
	
	if (initialized == 0)
	{
	        printf(" 1) Open Max 1027 device\n");
		printf(" 2) Change device number (%i)\n", devNumber);
	}
	else
	{
		printf(" 1) Close Max 1027 device\n");
		printf(" 2) Change Vref (%fmV)\n",vRef);
		printf(" 3) Select channel (%i)\n", channel);
		printf(" 4) Read channel value\n");
	}

        printf("> ");
        scanf("%s",buffer);

	if (initialized == 0)
	{
            switch(buffer[0])
            {
                case '1' :  printf("Open Max 1027 device\n");
       			    max1027_dev = new AsAdc(AS_MAX1027_NAME, devNumber, vRef);
                            if (max1027_dev < 0)
                            {
                                printf("Error, can't open max1027. Is max1027 modprobed ?\n");
                            } else {
                                printf("Max 1027 device opened\n");
                                initialized = 1;
                            }
                            pressEnterToContinue();
                            break;
                case '2' :  printf("Give device number:");
                            scanf("%i",&value);

			    if (value < 0)
			    {
                                printf(" Wrong value\n");
                            } else {
                                devNumber = value;
                            }
                            pressEnterToContinue();
                default : break;
	    }
	}
	else
	{
            switch(buffer[0])
            {
                    case '1' :  printf("Close Max 1027 device\n");
                                delete max1027_dev;
                                initialized = 0;
                                pressEnterToContinue();
                                break;
		    case '2' :  printf("Give wanted Vref (mV): ");
		                scanf("%f",&fValue);
		                delete max1027_dev;
			
		                max1027_dev = new AsAdc(AS_MAX1027_NAME, devNumber, fValue);
		                if (max1027_dev == NULL){
		                    printf("Error, can't open max1027 with Vref = %fmV\n", fValue);
		                    pressEnterToContinue();
		                    break;
		                }

				vRef = fValue;
		                printf("Vref changed to: %fmV\n", vRef);
		                pressEnterToContinue();
		                break;
		    case '3' :  printf("Give channel you want to read (0-6): ");
		                scanf("%d",&value);
		                if ((value >= NUM_OF_CHANNEL) || (value < 0)){
		                    printf("Channel num wrong\n");
		                    break;
		                }
		                channel = value;
		                break;
		    case '4' :  ret = max1027_dev->getValueInMillivolts(channel);
		                if (ret < 0) {
		                    printf("Error reading temperature\n");
		                    pressEnterToContinue();
		                    break;
		                }
		                printf("Channel %d value read : %dmV\n", channel, ret);
		                pressEnterToContinue();
		                break;
		    default : break;
	     }
        }
    }
}

void test_max5821()
{
    char buffer[50];
    int ret;
    int value;
    AsDac *max5821_dev = NULL;
    int channelA=0;
    int channelB=0;
    float vRef = 2500;


    max5821_dev = new AsDac(AS_MAX5821_TYPE, 0, 0x38, vRef);
    if (max5821_dev == NULL)
    {
        printf("Error, can't open max5821.\n");
        pressEnterToContinue();
        return ;
    }
    pressEnterToContinue();

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("**************************\n");
        printf("   Testing max5821       *\n");
        printf("**************************\n");
        printf("Choose ('q' to quit):\n");
        printf(" 1) select channel A value %d\n", channelA);
        printf(" 2) select channel B value %d\n", channelB);
        printf(" 3) select both channel values\n");

        printf("> ");
        scanf("%s",buffer);

        switch(buffer[0])
        {
            case '1' :  printf("Give value between 0-1023\n");
                        printf("> ");
                        scanf("%d", &value);
                        ret = max5821_dev->setValueInMillivolts(0, value);
                        if (ret < 0)
                        {
                            printf("Error, can't select value\n");
                        }
                        pressEnterToContinue();
                        break;
            case '2' :  printf("Give value between 0-1023\n");
                        printf("> ");
                        scanf("%d", &value);
                        ret = max5821_dev->setValueInMillivolts(1, value);
                        if (ret < 0)
                        {
                            printf("Error, can't select value\n");
                        }
                        pressEnterToContinue();
                        break;
            case '3' :  printf("Give value between 0-1023\n");
                        printf("> ");
                        scanf("%d", &value);
                        ret = max5821_dev->setValueInMillivolts(0, value);
                        if (ret < 0)
                        {
                            printf("Error, can't select channel 'A' value\n");
                        }
			else
			{
				ret = max5821_dev->setValueInMillivolts(1, value);
		                if (ret < 0)
		                {
		                    printf("Error, can't select channel 'B' value\n");
		                }	
			}			
                        pressEnterToContinue();
                        break;
            default : break;
        }
    }

    delete max5821_dev;
}

/* PWM test */
void test_pwm(void)
{
    AsPwm *pwm_dev = NULL;
    int ret;
    char buffer[20];
    char buffer2[20];
    int value;
    int devNumber = 0;
    int initialized = 0;

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("**************************\n");
        printf("   Testing PWM       *\n");
        printf("**************************\n");
        printf("Choose ('q' to quit):\n");
	
	if (initialized == 0)
	{
            printf(" 1) Open PWM device\n");
            printf(" 2) Change device number (%i)\n", devNumber);
	}
	else
	{
            printf(" 1) Close PWM device\n");
            printf(" 2) Set Frequency\n");
            printf(" 3) Get Frequency\n");
            printf(" 4) Set Period\n");
            printf(" 5) Get Period\n");
            printf(" 6) Set Duty\n");
            printf(" 7) Get Duty\n");
            printf(" 9) Activate/desactivate\n");
            printf(" 9) Get state\n");
	}

        printf("> ");
        scanf("%s",buffer);

	if (initialized == 0)
	{
            switch(buffer[0])
            {
                case '1' :  printf("Open PWM device\n");
       			    pwm_dev = new AsPwm(devNumber);
                            if (pwm_dev < 0)
                            {
                                printf("Error, can't open PWM device. Is PWM modprobed ?\n");
                            } else {
                                printf("PWM device opened\n");
                                initialized = 1;
                            }
                            pressEnterToContinue();
                            break;
                case '2' :  printf("Give device number:");
                            scanf("%i",&value);

			    if (value < 0)
			    {
                                printf(" Wrong value\n");
                            } else {
                                devNumber = value;
                            }
                            pressEnterToContinue();
                default : break;
	    }
	}
	else
	{
            switch(buffer[0])
            {
                    case '1' :  printf("Close PWM device\n");
                                delete pwm_dev;
                                initialized = 0;
                                pressEnterToContinue();
                                break;
                    case '2' :  printf("Give frequency: ");
                                scanf("%d",&value);

				ret = pwm_dev->setFrequency(value);
                                if (ret < 0)
                                {
                                    printf("Error, can't set frequency\n");
                                }
                                pressEnterToContinue();
                                break;

		    case '3' :  ret = pwm_dev->getFrequency();
		                if (ret < 0) {
		                    printf("Error reading frequency\n");
		                    pressEnterToContinue();
		                    break;
		                }
		                printf("Frequency read : %d\n", ret);
		                pressEnterToContinue();
		                break;
                    case '4' :  printf("Give period: ");
                                scanf("%d",&value);

				ret = pwm_dev->setPeriod(value);
                                if (ret < 0)
                                {
                                    printf("Error, can't set period\n");
                                }
                                pressEnterToContinue();
                                break;

		    case '5' :  ret = pwm_dev->getPeriod();
		                if (ret < 0) {
		                    printf("Error reading period\n");
		                    pressEnterToContinue();
		                    break;
		                }
		                printf("Period read : %d\n", ret);
		                pressEnterToContinue();
		                break;
                    case '6' :  printf("Give duty: ");
                                scanf("%d",&value);

				ret = pwm_dev->setDuty(value);
                                if (ret < 0)
                                {
                                    printf("Error, can't set duty\n");
                                }
                                pressEnterToContinue();
                                break;

		    case '7' :  ret = pwm_dev->getDuty();
		                if (ret < 0) {
		                    printf("Error reading period\n");
		                    pressEnterToContinue();
		                    break;
		                }
		                printf("Duty read : %d\n", ret);
		                pressEnterToContinue();
		                break;
                    case '8' : printf("Activate 'a' or Desactivate 'd' ?");
                               scanf("%s",buffer2);
                               
                               if(buffer2[0] == 'a')
                               {
                                   pwm_dev->setState(1);
                                   printf("PWM activated\n");
                                   pressEnterToContinue();
                               }
                               else if(buffer2[0] == 'd')
                               {
                                   pwm_dev->setState(0);
                                   printf("PWM desactivated\n");
                                   pressEnterToContinue();
                               }else{
                                   printf("Option %c unknown\n",buffer[0]);
                                   pressEnterToContinue();
                               }
                               break;
                    case '9' : if(pwm_dev->getState())
                               {
                                   printf("PWM is active\n");
                                   pressEnterToContinue();
                               }
                               else
                               {
                                   printf("PWM is inactive\n");
                                   pressEnterToContinue();
                               }
                               break;
		    default : break;
	     }
        }
    }
}

/* EEPROM test */
int address_size(int aType, int aWordSize)
{
    return (((aType-46)/10) + 6 + 2 - (aWordSize/8));
}

int max_address(int aType, int aWordSize)
{
    return (pow(2,address_size(aType, aWordSize))-1);
}

void test_93LC(void)
{
    char buffer[50];
    int i;

    As93LCXX *eeprom_dev = NULL;
    int type = 46;
    char spidevpath[50];
    /* defaults values */
    int frequency = 1000000;
    uint8_t word_size = 8;
    int value, value2;
    int initialized=0;
    snprintf(spidevpath, 50, "%s", "/dev/spidev2.0");

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("*********************************\n");
        printf("*   Testing 93LCxx menu         *\n");
        printf("*********************************\n");
        printf("Choose ('q' to quit):\n");
        if (initialized == 0)
        {
            printf(" 1) Change word size (%d)\n", word_size);
            printf(" 2) Change bus frequency (%d)\n", frequency);
            printf(" 3) Change eeprom type (%d)\n", type);
            printf(" 4) Change spidev path (%s)\n", spidevpath);
            printf(" 5) Initialize eeprom\n");
        } else {
            printf(" 1) Close eeprom\n");
            printf(" 2) Read specific address\n");
            printf(" 3) Erase specific address\n");
            printf(" 4) Write specific address\n");
            printf(" 5) Write all\n");
            printf(" 6) Read all\n");
            printf(" 7) Erase all\n");
            printf(" 8) Unlock write\n");
            printf(" 9) Lock write\n");
        }

        printf("> ");
        scanf("%s",buffer);

        if (initialized == 0)
        {
            switch(buffer[0])
            {
                case '1' :  printf("Give new word size (8 or 16): ");
                            scanf("%d", &value);
                            if ( (value != 8) && (value != 16) )
                            {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                word_size = value;
                            }
                            break;
                case '2' :  printf("Give bus frequency: ");
                            scanf("%d", &value);
                            frequency = value;
                            printf("Frequency changed to %dHz\n",frequency);
                            break;
                case '3' :  printf("Give new type of eeprom (46, 56 or 66): ");
                            scanf("%d", &value);
                            if ( (value != 46) && (value != 56) && (value != 66) )
                            {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                type = value;
                            }
                            break;
                case '4' :  printf("Give new path for spidev: \n");
                            scanf("%s", spidevpath);
                            printf("New path is %s\n",spidevpath);
                            break;
                case '5' :  eeprom_dev = new As93LCXX((unsigned char *)spidevpath, type, frequency, word_size);
                            if (eeprom_dev < 0)
                            {
                                printf("Error, can't initialize eeprom. Have you modprobed spidev ?\n");
                            } else {
                                printf("93LCXX device initialized\n");
                                initialized = 1;
                            }
                            pressEnterToContinue();
                            break;
                default : break;
            }
        } else {
            switch(buffer[0])
            {
                case '1' :  printf("EEPROM closed\n");
                            delete eeprom_dev;
                            initialized = 0;
                            pressEnterToContinue();
                            break;
                case '2' :  printf(" Read specific address\n");
                            printf("Give address in hexadecimal (max 0x%03X):",
                                   max_address(type, word_size));
                            scanf("%x",&value);
                            if (value > max_address(type, word_size))
                            {
                                printf(" Error, wrong address\n");
                            } else {
                                value2 = eeprom_dev->read(value);
                                if (value2 < 0)
                                {
                                    printf("Error, can't read value\n");
                                } else {
                                    printf(" Read %02x at %02X", value2, value );
                                }
                            }
                            pressEnterToContinue();
                            break;
                case '3' :  printf("Erase specific address\n");
                            printf("Give address in hexadecimal (max 0x%03X):",
                                   max_address(type, word_size));
                            scanf("%x",&value);
                            if (value > max_address(type, word_size))
                            {
                                printf(" Error, address wrong\n");
                            } else {
                                value2 = eeprom_dev->erase(value);
                                if (value2 < 0)
                                {
                                    printf("Error, can't erase value\n");
                                } else {
                                    printf("Value erased\n");
                                }
                            }
                            pressEnterToContinue();
                            break;
                case '4' :  printf("Write specific address\n");
                            printf("Give address in hexadecimal (max 0x%03X):",
                                   max_address(type, word_size));
                            scanf("%x",&value);
                            if (value > max_address(type, word_size))
                            {
                                printf(" Error, address wrong\n");
                                pressEnterToContinue();
                                break;
                            }
                            printf("Give value in hexadecimal :");
                            scanf("%x",&value2);
                            value2 = eeprom_dev->write(value, value2);
                            if (value2 < 0)
                            {
                                printf("Error, can't write value\n");
                            }
                            pressEnterToContinue();
                            break;
                case '5' :  printf(" Write all\n");
                            printf("Give value in hexadecimal:");
                            scanf("%x",&value);
                            value2 = eeprom_dev->writeAll(value);
                            if (value2 < 0) {
                                printf("Error, can't write value\n");
                            }
                            pressEnterToContinue();
                            break;
                case '6' :  printf(" Read all\n");
                            for (i = 0; i <= max_address(type, word_size); i++) {
                                if (word_size == 8) {
                                    printf("%03X -> %02X\n",
                                           i, (unsigned int)eeprom_dev->read(i));
                                } else {
                                    printf("%03X -> %04X\n",
                                           i, (unsigned int)eeprom_dev->read(i));
                                }
                            }
                            pressEnterToContinue();
                            break;
                case '7' :  printf(" Erase all\n");
                            value = eeprom_dev->eraseAll();
                            if (value < 0)
                            {
                                printf(" Error in erasing eeprom\n");
                            }
                            pressEnterToContinue();
                            break;
                case '8' :  printf("Unlock write\n");
                            if (eeprom_dev->ewen() < 0)
                            {
                                printf("Error can't unlock write\n");
                            }
                            pressEnterToContinue();
                            break;
                case '9' :  printf("lock write\n");
                            if (eeprom_dev->ewds() < 0)
                            {
                                printf("Error can't lock write\n");
                            }
                            pressEnterToContinue();
                            break;
                default : break;
            }
        }
    }
}

void test_backlight(void)
{
    int ret;
    char buffer[20];
    int value;
    AsBacklight *backlight_dev = NULL;

    backlight_dev = new AsBacklight();
    if (backlight_dev == NULL)
    {
        printf("Error, can't open backlight device.\n");
        pressEnterToContinue();
        return ;
    }
    pressEnterToContinue();

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("*********************************\n");
        printf("*   Testing  backlight menu           *\n");
        printf("*********************************\n");
        printf("Choose ('q' to quit):\n");
        printf(" 1) get actual brightness\n");
        printf(" 2) get maximum brightness\n");
        printf(" 3) set brightness\n");

        printf("> ");
        scanf("%s",buffer);

        switch(buffer[0])
        {
            case '1' :  ret = backlight_dev->getActualBrightness();
                        if (ret < 0)
                        {
		            printf("Error reading actual brightness\n");
		            pressEnterToContinue();
		            break;
                        }
                        printf("Actual brightness is %d\n", ret);
                        pressEnterToContinue();
                        break;
            case '2' :  ret = backlight_dev->getMaxBrightness();
                        if (ret < 0)
                        {
		            printf("Error reading maximum brightness\n");
		            pressEnterToContinue();
		            break;
                        }
                        printf("Maximum brightness is %d\n", ret);
                        pressEnterToContinue();
                        break;
            case '3' :  printf("Give brightness :");
                        scanf("%d",&value);

                        ret = backlight_dev->setBrightness(value);
                        if (ret < 0)
                        {
                            printf("Error, can't set brightness\n");	
                        }	

                        pressEnterToContinue();
                        break;
            default : break;
        }
    }

    delete backlight_dev;
}

/* AS1531 test */
void test_as1531(void)
{
    printf("TODO\n");
    pressEnterToContinue();
}

void test_i2c_eeprom()
{
    unsigned char page_buffer[500];
    char buffer[50];

    /* defaults values */
    int bus_number = 0;
    unsigned long eeprom_addr = 0x50;
    unsigned long addr_width = 8;
    unsigned long page_buffer_size = 16;
    unsigned int number_pages = 2;
    unsigned int page_size = 256;

    AsI2CEeprom* dev = NULL;

    unsigned int value, value2;
    int initialized = 0;

    while(buffer[0] != 'q')
    {
        system("clear");
        printf("*********************************\n");
        printf("*   Testing I2C EEPROM menu         *\n");
        printf("*********************************\n");
        printf("Choose ('q' to quit):\n");
        if (initialized == 0) {
            printf(" 1) Change I2C bus number (%d)\n", bus_number);
            printf(" 2) Change eeprom address (%lu)\n", eeprom_addr);
            printf(" 3) Change eeprom address width (%lu)\n", addr_width);
            printf(" 4) Change page write buffer size (%lu)\n", page_buffer_size);
            printf(" 5) Change number of pages (%d)\n", number_pages);
            printf(" 6) Change page size (%d)\n", page_size);
            printf(" 7) Initialize eeprom\n");
        } else {
            printf(" 1) Close eeprom\n");
            printf(" 2) Read specific page\n");
            printf(" 3) Write on specific page\n");
        }

        printf("> ");
        scanf("%s",buffer);

        if (initialized == 0)
        {
            switch(buffer[0])
            {
                case '1' :  printf("Give new I2C bus number (0 or 1): ");
                            scanf("%d", &value);
                            if ((value != 0) && (value != 1)) {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                bus_number = value;
                                printf("Bus number changed to %d\n", bus_number);
                            }
                            break;
                case '2' :  printf("Give eeprom address: ");
                            scanf("%d", &value);
                            eeprom_addr = value;
                            printf("Eeprom address changed to %lu\n", eeprom_addr);
                            break;
                case '3' :  printf("Give eeprom address width: ");
                            scanf("%d", &value);
                            if ((value != 8) && (value != 16)) {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                addr_width = value;
                                printf("Eeprom address width changed to %lu\n", addr_width);
                            }
                            break;
                case '4' :  printf("Give new page write buffer size (16 or 32): ");
                            scanf("%d", &value);
                            if ((value != 16) && (value != 32)) {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                page_buffer_size = value;
                                printf("Page write buffer size changed to %lu\n", page_buffer_size);
                            }
                            break;
                case '5' :  printf("Give number of pages: \n");
                            scanf("%d", &value);
                            if ((value != 1) && (value != 2) && (value != 4)) {
                                printf("Wrong value\n");
                                pressEnterToContinue();
                            } else {
                                number_pages = value;
                                printf("Number of pages changed to %d\n", number_pages);
                            }
                            break;
                case '6' :  printf("Give page size: \n");
                            scanf("%d", &value);
                            page_size = value;
                            printf("Page size changed to %d\n", page_size);
                            break;
                case '7' :  dev = new AsI2CEeprom(bus_number, eeprom_addr, addr_width, page_buffer_size, number_pages, page_size);
                            if (dev == NULL) {
                                printf("Error, can't initialize eeprom.\n");
                            } else {
                                initialized = 1;
                            }
                            pressEnterToContinue();
                            break;
                default : break;
            }
        } else {
            switch(buffer[0])
            {
                case '1' :  ;
                            printf("eeprom closed\n");
                            delete dev;
                            initialized = 0;
                            pressEnterToContinue();
                            break;
                case '2' :  printf(" Read specific page\n");
                            printf("Give page number (max. %d):",
                                   number_pages);
                            scanf("%x",&value);
                            if (value > number_pages) {
                                printf(" Error, wrong page number\n");
                            } else {
                                value2 = dev->read(value, page_buffer);
                                if (value2 < 0)
                                {
                                    printf("Error, can't read eeprom page\n");
                                } else {
                                    printf(" Read %s at page %d", page_buffer, value );
                                }
                            }
                            pressEnterToContinue();
                            break;
                case '3' :  printf("Write on specific page\n");
                            printf("Give page number (max. %d):",
                                   number_pages);
                            scanf("%d",&value);
                            if (value > number_pages)
                            {
                                printf(" Error, wrong page number\n");
                                pressEnterToContinue();
                                break;
                            }
                            printf("Give data to write :");
                            scanf("%s",page_buffer);
                            value2 = dev->write(value, page_buffer, 500);
                            if (value2 < 0)
                            {
                                printf("Error, can't write on page\n");
                            }
                            pressEnterToContinue();
                            break;
                default : break;
            }
        }
    }
}

void test_tcs3472(void)
{
    printf("TODO\n");
    pressEnterToContinue();
}

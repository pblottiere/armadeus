/*
**    THE ARMadeus Systems
**
**    Copyright (C) 2012  The armadeus systems team
**    Jérémie Scheer <jeremie.scheer@armadeus.com>
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "as_i2c_eeprom.h"

//#define DEBUG
#ifdef DEBUG
#define ERROR(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define ERROR(fmt, ...)		/*fmt, ##__VA_ARGS__ */
#endif

/*------------------------------------------------------------------------------*/

struct as_i2c_eeprom_device *as_i2c_eeprom_open(int aBusNumber,
						uint32_t anEeprom_addr,
						uint32_t anAddr_width,
						uint32_t aPage_buffer_size,
						uint16_t aNumber_pages,
						uint16_t aPage_size)
{
	struct as_i2c_eeprom_device *dev;

	/* Open I2C bus */
	struct as_i2c_device *i2c_dev;
	i2c_dev = as_i2c_open(aBusNumber);
	if (i2c_dev == NULL) {
		ERROR("Can't open I2C device");
		return NULL;
	}

	if (as_i2c_set_slave_addr(i2c_dev, anEeprom_addr) < 0) {
		ERROR("Can't set EEPROM address on I2C\n");
		return NULL;
	}

	/* verify datas */
	if (!((anAddr_width == 8) || (anAddr_width == 16)))
		return NULL;
	if (!
	    ((aPage_buffer_size == 4) || (aPage_buffer_size == 16)
	     || (aPage_buffer_size == 32)))
		return NULL;
	if (!
	    ((aNumber_pages == 1) || (aNumber_pages == 2)
	     || (aNumber_pages == 4)))
		return NULL;
	if (!((aPage_size == 256) || (aPage_size == 8 * 1024)))
		return NULL;

	dev =
	    (struct as_i2c_eeprom_device *)
	    malloc(sizeof(struct as_i2c_eeprom_device));
	if (dev == NULL) {
		ERROR
		    ("Can't allocate memory for I2C eeprom structure device\n");
		return NULL;
	}

	/* fill in spidev structure */
	dev->i2c_dev = i2c_dev;
	dev->eeprom_addr = anEeprom_addr;
	dev->addr_width = anAddr_width;
	dev->page_buffer_size = aPage_buffer_size;
	dev->number_pages = aNumber_pages;
	dev->page_size = aPage_size;
	dev->write_time_us = 3000;	// compatibility value

	return dev;
}

/** @brief close the device
 *
 * @param aDev i2c eeprom structure
 *
 */
void as_i2c_eeprom_close(struct as_i2c_eeprom_device *aDev)
{
	as_i2c_close(aDev->i2c_dev);
	free(aDev);
}

int32_t as_i2c_eeprom_read(struct as_i2c_eeprom_device *aDev,
			   uint16_t aPage_number, uint8_t * aBuffer)
{
	int16_t len = 0;
	uint8_t buffer[8 * 1024];

	if (aPage_number > aDev->number_pages) {
		aPage_number = aDev->number_pages;
		ERROR("Wrong page number\n");
	}

	if (aDev->addr_width == 8) {
		buffer[0] = 0;
		len = 1;
	} else {		/* 16 */
		buffer[0] = 0;
		buffer[1] = 0;
		len = 2;
	}

	if (as_i2c_set_slave_addr(aDev->i2c_dev, aDev->eeprom_addr) < 0) {
		ERROR("Can't set EEPROM address on I2C\n");
		return -1;
	}

	if (as_i2c_write(aDev->i2c_dev, buffer, len) < 0) {
		ERROR("Can't write on I2C EEPROM\n");
		return -1;
	}

	if (as_i2c_set_slave_addr
	    (aDev->i2c_dev, aDev->eeprom_addr | aPage_number) < 0) {
		ERROR("Can't set EEPROM address on I2C\n");
		return -1;
	}

	if (as_i2c_read(aDev->i2c_dev, buffer, 256) < 0) {
		ERROR("Can't read on I2C EEPROM\n");
		return -1;
	}

	memcpy(aBuffer, buffer, 256);

	return 0;
}

int32_t as_i2c_eeprom_write(struct as_i2c_eeprom_device * aDev,
			    uint16_t aPage_number, uint8_t * aBuffer,
			    uint16_t aBuffer_size)
{
	int16_t nb_it = 0;
	int16_t current_pos = 0;
	int16_t msg_size = 0;
	uint8_t buffer[8 * 1024];

	if (aBuffer_size > aDev->page_size) {
		ERROR("Size too long -> truncated to page size\n");
		aBuffer_size = aDev->page_size;
	}
	// Only buffer size write (16 or 32) are taken into account
	nb_it = aBuffer_size / aDev->page_buffer_size;

	int16_t i;
	for (i = 0; i < nb_it; i++) {
		if (aDev->addr_width == 8) {
			buffer[0] = current_pos;
			memcpy(buffer + 1, aBuffer + current_pos,
			       aDev->page_buffer_size);
			msg_size = aDev->page_buffer_size + 1;
		} else {	// 16
			buffer[0] = (current_pos >> 8) & 0xFF;
			buffer[1] = current_pos & 0xFF;
			memcpy(buffer + 2, aBuffer + current_pos,
			       aDev->page_buffer_size);
			msg_size = aDev->page_buffer_size + 2;
		}

		if (as_i2c_set_slave_addr
		    (aDev->i2c_dev, aDev->eeprom_addr | aPage_number) < 0) {
			ERROR("Can't set EEPROM address on I2C\n");
			return -1;
		}

		if (as_i2c_write(aDev->i2c_dev, buffer, msg_size) < 0) {
			ERROR("Can't write on I2C EEPROM\n");
			return -1;
		}

		usleep(aDev->write_time_us);

		current_pos += aDev->page_buffer_size;
	}

	return 0;
}

int32_t as_i2c_eeprom_set_write_time(struct as_i2c_eeprom_device * aDev,
				     uint32_t aWriteTime)
{
	aDev->write_time_us = aWriteTime;
	return 0;
}

static int32_t as_i2c_eeprom_read_write_mid(struct as_i2c_eeprom_device *aDev,
					    uint16_t aAddr, uint16_t aLen,
					    uint8_t * aBuffer, uint32_t isWrite)
{
	static uint8_t buffer[8 * 1024 + 2];
	int16_t addr_len = 0;
	uint16_t fragment_len;
	uint16_t current_addr = aAddr;
	uint16_t current_page;

	if (aAddr + aLen > aDev->number_pages * aDev->page_size) {
		ERROR("Wrong address and length : %04x %d\n", aAddr, aLen);
		return -1;
	}

	if (aDev->addr_width == 8) {
		addr_len = 1;
	} else {		/* 16 */
		addr_len = 2;
	}

	while (aLen) {
		/* calculate page and offset */
		current_page = current_addr / aDev->page_size;
		if (addr_len == 2) {
			*((uint16_t *) buffer) =
			    htons(current_addr % aDev->page_size);
		} else {
			*((uint8_t *) buffer) =
			    (uint8_t) (current_addr % aDev->page_size);
		}

		/* select page to read or write to */
		if (as_i2c_set_slave_addr
		    (aDev->i2c_dev, aDev->eeprom_addr | current_page) < 0) {
			ERROR("Can't set EEPROM address on I2C\n");
			return -1;
		}

		/* calculate len of fragment regarding page_buffer_size */
		fragment_len =
		    aDev->page_buffer_size -
		    (current_addr % aDev->page_buffer_size);
		if (fragment_len > aLen) {
			fragment_len = aLen;
		}

		if (isWrite) {
			memcpy(buffer + 2, aBuffer, fragment_len);
			/* select offset address inside page */
			if (as_i2c_write
			    (aDev->i2c_dev, buffer,
			     addr_len + fragment_len) < 0) {
				ERROR("Can't write on I2C EEPROM\n");
				return -1;
			}
			usleep(aDev->write_time_us);
		} else {
			/* select offset address inside page */
			if (as_i2c_write(aDev->i2c_dev, buffer, addr_len) < 0) {
				ERROR("Can't write on I2C EEPROM\n");
				return -1;
			}
			/* read data */
			if (as_i2c_read(aDev->i2c_dev, aBuffer, fragment_len) <
			    0) {
				ERROR("Can't read on I2C EEPROM\n");
				return -1;
			}
		}

		current_addr += fragment_len;
		aBuffer += fragment_len;
		aLen -= fragment_len;
	}

	return 0;
}

int32_t as_i2c_eeprom_read_mid(struct as_i2c_eeprom_device * aDev,
			       uint16_t aAddr, uint16_t aLen, uint8_t * aBuffer)
{
	return as_i2c_eeprom_read_write_mid(aDev, aAddr, aLen, aBuffer, 0);
}

int32_t as_i2c_eeprom_write_mid(struct as_i2c_eeprom_device * aDev,
				uint16_t aAddr, uint16_t aLen,
				uint8_t * aBuffer)
{
	return as_i2c_eeprom_read_write_mid(aDev, aAddr, aLen, aBuffer, 1);
}

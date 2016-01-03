/*
 * Abstracts framebuffer access
 *
 * Copyright (C) 2009 <gwenhael.goavec-merou@armadeus.com>
 *                         Armadeus Project / Armadeus Systems
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/fb.h>

#include "lcd.h"


char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
unsigned long xres = 0;
unsigned long yres = 0;

int init_lcd()
{
	int fbfd = 0;

	/* Open the file for reading and writing */
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd < 0) {
		printf("Error: cannot open framebuffer device.\n");
		return -1;
	}
	printf("The framebuffer device was opened successfully.\n");
  
	/* Get fixed screen information */
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		return -1;
	}
  
	/* Get variable screen information */
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		return -1;
	}
  
	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	/* Figure out the size of the screen in bytes */
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	xres = vinfo.xres;
	yres = vinfo.yres;

	/* Map the device to memory */
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		printf("Error: failed to map framebuffer device to memory.\n");
		return -1;
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	return fbfd;
}

void close_lcd(int fbfd)
{
	munmap(fbp, screensize);
	close(fbfd);
}

unsigned long lcd_get_xres(void)
{
	return xres;
}

unsigned long lcd_get_yres(void)
{
	return yres;
}

void draw_pixel(unsigned int x, unsigned int y, struct color rgb)
{
	long int location = 0;

	if ((x >= vinfo.xres) || (y >= vinfo.yres)) {
		printf("%s: position (%d,%d) outside boundaries !\n", __func__, x, y);
		if (x >= vinfo.xres)
			x = vinfo.xres-1;
		if (y >= vinfo.yres)
			y = vinfo.yres-1;
	}
	location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
		(y+vinfo.yoffset) * finfo.line_length;
  
	unsigned short int t = ((rgb.r&0xf8)>>3)<<11 | ((rgb.g&0xfc)>>2) << 5 | ((rgb.b&0xf8)>>3);
	*((unsigned short int*)(fbp + location)) = t; 
}

void clear_vline(unsigned int x)
{
	unsigned int y = 0;
	struct color black = {0,0,0};

	for (y=0; y<vinfo.yres; y++) {
		draw_pixel(x, y, black);
	}
}


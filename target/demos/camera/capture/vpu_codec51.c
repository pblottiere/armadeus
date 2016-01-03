/*
 * Copyright 2004-2006 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * Copyright (c) 2006, Chips & Media.  All rights reserved.
 *
 * Copyright (c) 2010, Armadeus Systems.  All rights reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file vpu_codec.h
 *
 * @brief This file implement codec application.
 *
 * @ingroup VPU
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <linux/videodev2.h>

#include "rtp.h"
/*#include "vpu_voip_test.h"
#include "vpu_display.h"
#include "vpu_capture.h"*/
#include "vpu_io.h"
#include "vpu_lib.h"

#include "vpu_codec.h"

static int quitflag;
int gdest = PATH_NET;
static char *g_ip_addr = "0.0.0.0";

static void export_bitstream(char *packet, int size)
{
	if (gdest & PATH_NET) {
		rtp_send(packet, size, g_ip_addr);
	}
}

void vpu_codec_stop(void)
{
	quitflag = 1;
}

void vpu_codec_pause(void)
{
	quitflag = 0; /* TBDL */
}

int g_initialized = 0;

int vpu_codec_init(int mode, int picWidth, int picHeight, int bitRate, Uint32 dest, char *ip_addr)
{
	int err;
	vpu_versioninfo ver;
	
	err = vpu_Init(NULL);
	if (err) {
		fprintf(stderr, "VPU Init Failure.\n");
		return -1;
	}

	err = vpu_GetVersionInfo(&ver);
	if (err) {
		fprintf(stderr, "Cannot get version info\n");
		vpu_UnInit();
		return -1;
	}

	printf("VPU firmware version: %d.%d.%d\n",
			ver.fw_major, ver.fw_minor, ver.fw_release);
	printf("VPU library version: %d.%d.%d\n",
			ver.lib_major,ver.lib_minor, ver.lib_release);

	g_initialized = 1;

	return 0;
}

int vpu_codec_encode_next_frame(size_t frame_offset, unsigned char* image)
{
	int ret;
	static int first_time = 1;

	if (!g_initialized) {
		printf("%s: please initialize codec first !\n", __func__);
		return -1;
	}

	if (first_time) {
		printf("%s Not implemented yet on APF51\n", __func__);
		first_time = 0;
	}

	return 0;
}

int vpu_SystemShutdown(void)
{
	vpu_UnInit();
	return 0;
}

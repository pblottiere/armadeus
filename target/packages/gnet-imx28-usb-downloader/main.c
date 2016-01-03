/*
 * 2011 Grid-Net Inc Steve Iribarne <siribarne@grid-net.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <usb.h>

#include "main.h"
#include "libusb_handler.h"

struct params {
	char file_name[1024];
};

void checkargs(int argc, char *argv[], struct params *param)
{
	int arg = 0;
	while (arg < argc) {
		if (argv[arg][0] != '-') {
			arg++;
			continue;
		}

		if (strcmp("-h", argv[arg]) == 0) {
			printf(HELP_MSG);
			exit(0);
		}

		if (strcmp("-v", argv[arg]) == 0) {
			printf(VERSION_MSG);
			exit(0);
		}

		if ((strcmp("-f", argv[arg]) == 0)
		    && (strcmp("", param->file_name) == 0)) {
			if (argc > arg + 1) {
				strcpy(param->file_name, argv[arg + 1]);
			}
		}
		arg++;
	}

	if (strcmp("", param->file_name) == 0) {
		printf("No filename specified\n");
		printf(USAGE_MSG);
		exit(-EINERR);
	}
}

int main(int argc, char *argv[])
{
	FILE *infd;
	void *data;
	int retval;
	unsigned int sz, bytesRead;
	struct params params = { "" };

	checkargs(argc, argv, &params);

	infd = fopen(params.file_name, "rb");
	if (infd == NULL) {
		printf("Error opening input file: %s\n", strerror(errno));
		exit(-EINERR);
	}

	fseek(infd, 0L, SEEK_END);
	sz = ftell(infd);
	fseek(infd, 0L, SEEK_SET);

	data = malloc(sz);
	if (data < 0) {
		printf("Error allocating internal buffer: %s\n",
		       strerror(errno));
		return -EMEM;
	}
	bytesRead = 0;
	while (bytesRead < sz) {
		bytesRead += fread(data + bytesRead, 1, sz, infd);

	}

	printf("attempting to write writing [%s] bootstream...",
	       params.file_name);
	retval = usb_write(sz, data);

	switch (retval) {
	case -ERR_TX:
	case -ERR_DEV_BUSY:
	case -ERR_NO_DEV:
		printf("Failed to write data to device.\n");
		break;
	case SUCCESS:
		printf("SUCCESS\n");
		break;
	default:
		printf("Unknown error\n");
	}

	free(data);
	fclose(infd);
	return retval;
}

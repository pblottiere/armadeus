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
#include <usb.h>
#include <string.h>
#include <unistd.h>

#include "libusb_handler.h"

inline int cpu_to_le16(int in)
{
	int tmp = ((in & 0x000000ff) >> 8) | ((in & 0x0000ff00) << 8);
	return tmp;
}

int transferData(struct usb_dev_handle *dev, size_t tx_size, char *data)
{
	int retval = 0;
	struct _ST_HID_CBW *cbw = NULL;
	size_t writesize = 0;
	int payload_size = 0;
	int numbufs;
	unsigned int tag = 0;
	unsigned int userbufidx = 0;
	unsigned int curbuf_ref = 0;
	char *buf;

	writesize = TX_MSG_SIZE_1K + 1;

	cbw = malloc(sizeof(struct _ST_HID_CBW));
	memset(cbw, 0, sizeof(struct _ST_HID_CBW));
	cbw->Cdb.Command = BLTC_DOWNLOAD_FW;
	cbw->Tag = ++tag;
	cbw->cmd = 1;
	cbw->Signature = CBW_BLTC_SIGNATURE;
	cbw->XferLength = tx_size;
	cbw->Flags = CBW_HOST_TO_DEVICE_DIR;
	cbw->Cdb.Length = 0xf0804a00;	//tx_size;

	payload_size = sizeof(struct _ST_HID_CBW);

	retval = usb_control_msg(dev,
				 HID_BLTC_REQUEST_TYPE,
				 HID_BLTC_REQUEST,
				 0x0200 | HID_BLTC_REPORT_TYPE_COMMAND_OUT,
				 0, (char *)cbw, payload_size, (int)2000);
	if (retval != payload_size) {
		printf("\n%s: %s\n", __func__, usb_strerror());
		return -ERR_TX;
	}
	free(cbw);

	/* find the number of buffer's to transfer */
	payload_size = writesize - 1;
	buf = malloc(writesize);

	numbufs = tx_size / payload_size;
	if (tx_size % payload_size)
		numbufs++;

	userbufidx = 0;
	for (curbuf_ref = 0; curbuf_ref < (numbufs - 1); curbuf_ref++) {
		if (!(curbuf_ref / 333))
			printf(".");
		memcpy(buf + 1, data + userbufidx, payload_size);
		userbufidx += payload_size;
		buf[0] = HID_BLTC_REPORT_TYPE_DATA_OUT;
		retval = usb_control_msg(dev,
					 HID_BLTC_REQUEST_TYPE,
					 HID_BLTC_REQUEST,
					 0x0200 | HID_BLTC_REPORT_TYPE_DATA_OUT,
					 0, buf, writesize, 2000);
		if (retval != writesize) {
			printf("\n%s: buffer %d: %s", __func__, curbuf_ref,
			       usb_strerror());
			free(buf);
			return -ERR_TX;
		}
	}
	payload_size = tx_size - userbufidx;
	memcpy(buf + 1, data + userbufidx, payload_size);

	buf[0] = HID_BLTC_REPORT_TYPE_DATA_OUT;
	retval = usb_control_msg(dev,
				 HID_BLTC_REQUEST_TYPE,
				 HID_BLTC_REQUEST,
				 0x0200 | HID_BLTC_REPORT_TYPE_DATA_OUT,
				 0, buf, writesize, 2000);
	if (retval != writesize) {
		printf("\n%s: buffer %d: %s", __func__, curbuf_ref,
		       usb_strerror());
		return -ERR_TX;
	}

	if (!(retval = usb_interrupt_read(dev, 1, buf, 14, 500))) {
		printf("\n%s: error durring read: %s", __func__,
		       usb_strerror());
		return -ERR_TX;
	}

	free(buf);

	return SUCCESS;
}

int usb_write(int size, char *dataBuf)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	char found_dev = 0;
	int retval = 0;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for (bus = usb_busses; bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			usb_dev_handle *udev;

			if ((dev->descriptor.idVendor == FREESCALE_VENDOR_ID)
			    && (dev->descriptor.idProduct ==
				FREESCALE_PRODUCT_ID)) {
				found_dev = 1;
				udev = usb_open(dev);

				if (udev == NULL) {
					retval = -ERR_NO_DEV;
					printf("\n%s: %s\n", __func__,
					       usb_strerror());
				} else {
					if (usb_claim_interface(udev, 0) != 0) {
						if (usb_detach_kernel_driver_np
						    (udev, 0) != 0) {
							printf("\n%s: %s\n",
							       __func__,
							       usb_strerror());
							retval = -ERR_DEV_BUSY;
						} else {
							retval =
							    transferData(udev,
									 size,
									 dataBuf);
							usb_release_interface
							    (udev, 0);
						}
					} else {
						retval =
						    transferData(udev, size,
								 dataBuf);
						usb_release_interface(udev, 0);
					}
					usb_close(udev);
				}
			}

		}
	}
	if (found_dev == 0) {
		retval = -ERR_NO_DEV;
	}
	return retval;
}

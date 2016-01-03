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
#define NUM_ARGS_EXPECTED   2
#define VERSION_MSG    "\ngnet_imx28_usb_downloader: \n version 0.5\n\n"
#define USAGE_MSG   "\nusage is: gnet_imx28_usb_downloader -f fileName \n\tfor help type: imx_usb_loader -h\n\n"
#define HELP_MSG    "gnet_imx28_usb_downloader:\n\nThis tool will stream the specified bootstream specified over USB to a Freescale imx280 device\nsupported switches: \n\t-f:\t specifies the input stream file (mandatory paramter)\n\t-h:\t prints this help message\n\t-v:\t prints version number\n\n"
#define IMX_TRANSFER_PARAM_STR "/sys/module/imxldr/parameters/transfer_size"

/* the first 4 are defined by libusb_handler.h */
//#define SUCCESS         0
//#define ERR_TX          1
//#define ERR_DEV_BUSY    2
//#define ERR_NO_DEV      3

#define EINERR  4
#define EMEM    5

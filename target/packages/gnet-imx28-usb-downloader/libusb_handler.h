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

#define FREESCALE_VENDOR_ID     0x15a2
#define FREESCALE_PRODUCT_ID    0x004f

#define ERR_TX          1
#define ERR_DEV_BUSY    2
#define ERR_NO_DEV      3
#define SUCCESS         0
    
    
int usb_write( int size, char * dataBuf);


/*  USB DRIVER INFO     */

#define HID_BLTC_REQUEST_TYPE               0x21
#define HID_BLTC_REQUEST                    0x09

#define BLTC_DOWNLOAD_FW                    2
#define HID_BLTC_REPORT_TYPE_DATA_OUT       2
#define HID_BLTC_REPORT_TYPE_COMMAND_OUT    1

#define TX_MSG_SIZE_1K                  1024


#pragma pack(1)
 //------------------------------------------------------------------------------
 // HID Download Firmware CDB
 //------------------------------------------------------------------------------
 typedef struct _CDBHIDDOWNLOAD {
    unsigned char   Command;
    unsigned int    Length;
    unsigned char   Reserved[11];
} CDBHIDDOWNLOAD, *PCDBHIDDOWNLOAD;

//------------------------------------------------------------------------------
// HID Command Block Wrapper (CBW)
//------------------------------------------------------------------------------
struct _ST_HID_CBW
{
    unsigned char cmd;
    unsigned int Signature;        // Signature: 0x43544C42:1129598018, o "BLTC" (little endian) for the BLTC CBW
    unsigned int Tag;              // Tag: to be returned in the csw
    unsigned int XferLength;       // XferLength: number of bytes to transfer
    unsigned char Flags;            // Flags:
    //   Bit 7: direction - device shall ignore this bit if the
    //     XferLength field is zero, otherwise:
    //     0 = data-out from the host to the device,
    //     1 = data-in from the device to the host.
    //   Bits 6..0: reserved - shall be zero.
    unsigned char Reserved[2];       // Reserved - shall be zero.
    CDBHIDDOWNLOAD Cdb;        // cdb: the command descriptor block
};
// Signature value for _ST_HID_CBW
static const unsigned int CBW_BLTC_SIGNATURE = 0x43544C42; // "BLTC" (little endian)
static const unsigned int CBW_PITC_SIGNATURE = 0x43544950; // "PITC" (little endian)
// Flags values for _ST_HID_CBW
static const unsigned char CBW_DEVICE_TO_HOST_DIR = 0x80; // "Data Out"
static const unsigned char CBW_HOST_TO_DEVICE_DIR = 0x00; // "Data In"

//------------------------------------------------------------------------------
// HID Command Status Wrapper (CSW)
//------------------------------------------------------------------------------
struct _ST_HID_CSW
{
    unsigned char cmd;
    unsigned int Signature;        // Signature: 0x53544C42, or "BLTS" (little endian) for the BLTS CSW
    unsigned int Tag;              // Tag: matches the value from the CBW
    unsigned int Residue;          // Residue: number of bytes not transferred
    unsigned char  Status;           // Status:
    //  00h command passed ("good status")
    //  01h command failed
    //  02h phase error
    //  03h to FFh reserved
};
// Signature value for _ST_HID_CSW
static const unsigned int CSW_BLTS_SIGNATURE = 0x53544C42; // "BLTS" (little endian)
static const unsigned int CSW_PITS_SIGNATURE = 0x53544950; // "PITS" (little endian)
// Status values for _ST_HID_CSW
static const unsigned char CSW_CMD_PASSED = 0x00;
static const unsigned char CSW_CMD_FAILED = 0x01;
static const unsigned char CSW_CMD_PHASE_ERROR = 0x02;

struct _TSS_PACKET
{
    unsigned char ReportId;
    unsigned char Payload[32];
};

struct _ST_HID_COMMMAND_REPORT
{
    unsigned char ReportId;
    struct _ST_HID_CBW Cbw;
};

struct _ST_HID_STATUS_REPORT
{
    unsigned char ReportId;
    struct _ST_HID_CSW Csw;
};
#pragma pack()

/* Code taken from parse_edid tool from read-edid-1.4.2 suite which was 
 * written by:
 *   John Fremlin <vii@users.sourceforge.net>
 *   Martin Kavalec <xkavm04@vse.cz>
 *
 * Source is released under the:
 *   GNU GENERAL PUBLIC LICENSE
 *   Version 2, June 1991 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse-edid.h"
#include "drm_edid.h"

// TODO: rewrite
// FIXME: cleanup 'static' variables

//typedef unsigned char byte;
#define byte	unsigned char
#define u8	unsigned char
typedef unsigned int bool;
#define true	1
#define false	0

/* byte must be 8 bits */

/* int must be at least 16 bits */

/* long must be at least 32 bits */

#define DIE_MSG( x ) \
        { MSG( x ); exit( 1 ); }

#define UPPER_NIBBLE( x ) \
        (((128|64|32|16) & (x)) >> 4)

#define LOWER_NIBBLE( x ) \
        ((1|2|4|8) & (x))

#define COMBINE_HI_8LO( hi, lo ) \
        ( (((unsigned)hi) << 8) | (unsigned)lo )

#define COMBINE_HI_4LO( hi, lo ) \
        ( (((unsigned)hi) << 4) | (unsigned)lo )

const char edid_v1_header[] = { 0x00, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x00
};

const char edid_v1_descriptor_flag[] = { 0x00, 0x00 };

static char* edid_est_modes[] = {
	"800x600@60Hz",
	"800x600@56Hz",
	"640x480@75Hz",
	"640x480@72Hz",
	"640x480@67Hz",
	"640x480@60Hz",
	"720x400@88Hz",
	"720x400@70Hz",
	"1280x1024@75Hz",
	"1024x768@75Hz",
	"1024x768@70Hz",
	"1024x768@60Hz",
	"1024x768@43Hz",
	"832x624@75Hz",
	"800x600@75Hz",
	"800x600@72Hz",
	"1152x864@75Hz",
};

enum {
	EST_800x600_60	= 0,
	EST_640x480_60	= 5,
	EST_1024x768_60	= 11,
};

//#define EDID_LENGTH                             0x80

#define EDID_HEADER                             0x00
#define EDID_HEADER_END                         0x07

#define ID_MANUFACTURER_NAME                    0x08
#define ID_MANUFACTURER_NAME_END                0x09
#define ID_MODEL				0x0a

#define ID_SERIAL_NUMBER			0x0c

#define MANUFACTURE_WEEK			0x10
#define MANUFACTURE_YEAR			0x11

#define EDID_STRUCT_VERSION                     0x12
#define EDID_STRUCT_REVISION                    0x13

#define DPMS_FLAGS				0x18
#define ESTABLISHED_TIMING_1                    0x23
#define ESTABLISHED_TIMING_2                    0x24
#define MANUFACTURERS_TIMINGS                   0x25

#define DETAILED_TIMING_DESCRIPTIONS_START      0x36
#define DETAILED_TIMING_DESCRIPTION_SIZE        18
#define NO_DETAILED_TIMING_DESCRIPTIONS         4

#define DETAILED_TIMING_DESCRIPTION_1           0x36
#define DETAILED_TIMING_DESCRIPTION_2           0x48
#define DETAILED_TIMING_DESCRIPTION_3           0x5a
#define DETAILED_TIMING_DESCRIPTION_4           0x6c

#define PIXEL_CLOCK_LO     (unsigned)dtd[ 0 ]
#define PIXEL_CLOCK_HI     (unsigned)dtd[ 1 ]
#define PIXEL_CLOCK        (COMBINE_HI_8LO( PIXEL_CLOCK_HI,PIXEL_CLOCK_LO )*10000)

#define H_ACTIVE_LO        (unsigned)dtd[ 2 ]

#define H_BLANKING_LO      (unsigned)dtd[ 3 ]

#define H_ACTIVE_HI        UPPER_NIBBLE( (unsigned)dtd[ 4 ] )

#define H_ACTIVE           COMBINE_HI_8LO( H_ACTIVE_HI, H_ACTIVE_LO )

#define H_BLANKING_HI      LOWER_NIBBLE( (unsigned)dtd[ 4 ] )

#define H_BLANKING         COMBINE_HI_8LO( H_BLANKING_HI, H_BLANKING_LO )

#define V_ACTIVE_LO        (unsigned)dtd[ 5 ]

#define V_BLANKING_LO      (unsigned)dtd[ 6 ]

#define V_ACTIVE_HI        UPPER_NIBBLE( (unsigned)dtd[ 7 ] )

#define V_ACTIVE           COMBINE_HI_8LO( V_ACTIVE_HI, V_ACTIVE_LO )

#define V_BLANKING_HI      LOWER_NIBBLE( (unsigned)dtd[ 7 ] )

#define V_BLANKING         COMBINE_HI_8LO( V_BLANKING_HI, V_BLANKING_LO )

#define H_SYNC_OFFSET_LO   (unsigned)dtd[ 8 ]
#define H_SYNC_WIDTH_LO    (unsigned)dtd[ 9 ]

#define V_SYNC_OFFSET_LO   UPPER_NIBBLE( (unsigned)dtd[ 10 ] )
#define V_SYNC_WIDTH_LO    LOWER_NIBBLE( (unsigned)dtd[ 10 ] )

#define V_SYNC_WIDTH_HI    ((unsigned)dtd[ 11 ] & (1|2))
#define V_SYNC_OFFSET_HI   (((unsigned)dtd[ 11 ] & (4|8)) >> 2)

#define H_SYNC_WIDTH_HI    (((unsigned)dtd[ 11 ] & (16|32)) >> 4)
#define H_SYNC_OFFSET_HI   (((unsigned)dtd[ 11 ] & (64|128)) >> 6)

#define V_SYNC_WIDTH       COMBINE_HI_4LO( V_SYNC_WIDTH_HI, V_SYNC_WIDTH_LO )
#define V_SYNC_OFFSET      COMBINE_HI_4LO( V_SYNC_OFFSET_HI, V_SYNC_OFFSET_LO )

#define H_SYNC_WIDTH       COMBINE_HI_4LO( H_SYNC_WIDTH_HI, H_SYNC_WIDTH_LO )
#define H_SYNC_OFFSET      COMBINE_HI_4LO( H_SYNC_OFFSET_HI, H_SYNC_OFFSET_LO )

#define H_SIZE_LO          (unsigned)dtd[ 12 ]
#define V_SIZE_LO          (unsigned)dtd[ 13 ]

#define H_SIZE_HI          UPPER_NIBBLE( (unsigned)dtd[ 14 ] )
#define V_SIZE_HI          LOWER_NIBBLE( (unsigned)dtd[ 14 ] )

#define H_SIZE             COMBINE_HI_8LO( H_SIZE_HI, H_SIZE_LO )
#define V_SIZE             COMBINE_HI_8LO( V_SIZE_HI, V_SIZE_LO )

#define H_BORDER           (unsigned)dtd[ 15 ]
#define V_BORDER           (unsigned)dtd[ 16 ]

#define FLAGS              (unsigned)dtd[ 17 ]

#define INTERLACED         (FLAGS&128)
#define SYNC_TYPE	   (FLAGS&3<<3)	/* bits 4,3 */
#define SYNC_SEPARATE	   (3<<3)
#define HSYNC_POSITIVE	   (FLAGS & 4)
#define VSYNC_POSITIVE     (FLAGS & 2)

#define MONITOR_NAME            0xfc
#define MONITOR_LIMITS          0xfd
#define UNKNOWN_DESCRIPTOR      -1
#define DETAILED_TIMING_BLOCK   -2

#define DESCRIPTOR_DATA         5
#define V_MIN_RATE              block[ 5 ]
#define V_MAX_RATE              block[ 6 ]
#define H_MIN_RATE              block[ 7 ]
#define H_MAX_RATE              block[ 8 ]

#define MAX_PIXEL_CLOCK         (((int)block[ 9 ]) * 10)
#define GTF_SUPPORT		block[10]

#define DPMS_ACTIVE_OFF		(1 << 5)
#define DPMS_SUSPEND		(1 << 6)
#define DPMS_STANDBY		(1 << 7)

char *myname;

void MSG(const char *x)
{
	fprintf(stderr, "%s: %s\n", myname, x);
}

int parse_timing_description(byte * dtd);

int parse_monitor_limits(byte * block);

int block_type(byte * block);

char *get_monitor_name(byte const *block);

char *get_vendor_sign(byte const *block);

int parse_dpms_capabilities(byte flags);

bool drm_detect_hdmi_monitor(struct edid * edid);
bool drm_detect_monitor_audio(struct edid * edid);
static int check_established_modes(struct edid *edid);

#ifdef STANDALONE
int main(int argc, char **argv)
{
	byte edid[EDID_LENGTH];
	FILE *edid_file;

	myname = argv[0];
	fprintf(stderr, "%s: parse-edid (version 1.4)\n", myname);

	if (argc > 2) {
		DIE_MSG("syntax: [input EDID file]");
	} else {
		if (argc == 2) {
			edid_file = fopen(argv[1], "rb");
			if (!edid_file)
				DIE_MSG("unable to open file for input");
		}

		else
			edid_file = stdin;
	}

	if (fread(edid, sizeof(byte), EDID_LENGTH, edid_file)
	    != EDID_LENGTH) {
		DIE_MSG("IO error reading EDID");
	}

	fclose(edid_file);

	return parse_edid(edid);
}
#endif

int parse_edid(byte * edid)
{
	unsigned i;
	byte *block;
	char *monitor_name = NULL;
	char monitor_alt_name[100];
	byte checksum = 0;
	char *vendor_sign;
	int ret = 0;

	for (i = 0; i < EDID_LENGTH; i++)
		checksum += edid[i];

	if (checksum != 0) {
		MSG("EDID checksum failed - data is corrupt. Continuing anyway.");
		ret = 1;
	} else
		MSG("EDID checksum passed.");

	if (strncmp
	    ((char *)edid + EDID_HEADER, edid_v1_header, EDID_HEADER_END + 1)) {
		MSG("first bytes don't match EDID version 1 header");
		MSG("do not trust output (if any).");
		ret = 1;
	}

	printf("\n\t# EDID version %d revision %d\n",
	       (int)edid[EDID_STRUCT_VERSION], (int)edid[EDID_STRUCT_REVISION]);

	vendor_sign = get_vendor_sign(edid + ID_MANUFACTURER_NAME);

	block = edid + DETAILED_TIMING_DESCRIPTIONS_START;

	for (i = 0; i < NO_DETAILED_TIMING_DESCRIPTIONS; i++,
	     block += DETAILED_TIMING_DESCRIPTION_SIZE) {

		if (block_type(block) == MONITOR_NAME) {
			monitor_name = get_monitor_name(block);
			break;
		}
	}

	if (!monitor_name) {
		/* Stupid djgpp hasn't snprintf so we have to hack something together */
		if (strlen(vendor_sign) + 10 > sizeof(monitor_alt_name))
			vendor_sign[3] = 0;

		sprintf(monitor_alt_name, "%s:%02x%02x",
			vendor_sign, edid[ID_MODEL], edid[ID_MODEL + 1]);
		monitor_name = monitor_alt_name;
	}

	printf("\tVendor Name \"%s\"\n", vendor_sign);
	printf("\tModel Name \"%s\"\n", monitor_name);

	block = edid + DETAILED_TIMING_DESCRIPTIONS_START;

	for (i = 0; i < NO_DETAILED_TIMING_DESCRIPTIONS; i++,
	     block += DETAILED_TIMING_DESCRIPTION_SIZE) {

		if (block_type(block) == MONITOR_LIMITS)
			parse_monitor_limits(block);
	}

	parse_dpms_capabilities(edid[DPMS_FLAGS]);

	block = edid + DETAILED_TIMING_DESCRIPTIONS_START;

	for (i = 0; i < NO_DETAILED_TIMING_DESCRIPTIONS; i++,
	     block += DETAILED_TIMING_DESCRIPTION_SIZE) {

		if (block_type(block) == DETAILED_TIMING_BLOCK)
			parse_timing_description(block);
	}

	printf("\n");

	if (drm_detect_hdmi_monitor((struct edid *) edid)) {
		printf("It's an HDMI monitor\n");
	} else {
		printf("It's NOT an HDMI monitor\n");
	}

	if (!drm_detect_monitor_audio((struct edid *) edid)) {
		printf("Monitor has NO audio\n");
	}

	check_established_modes((struct edid *) edid);

	return ret;
}

int parse_timing_description(byte * dtd)
{
	int htotal, vtotal;
	htotal = H_ACTIVE + H_BLANKING;
	vtotal = V_ACTIVE + V_BLANKING;

	printf("\tMode \t\"%dx%d\"", H_ACTIVE, V_ACTIVE);
	printf("\t# vfreq %3.3fHz, hfreq %6.3fkHz\n",
	       (double)PIXEL_CLOCK / ((double)vtotal * (double)htotal),
	       (double)PIXEL_CLOCK / (double)(htotal * 1000));

	printf("\t\tDotClock\t%f\n", (double)PIXEL_CLOCK / 1000000.0);

	printf("\t\tHTimings\t%u %u %u %u\n", H_ACTIVE,
	       H_ACTIVE + H_SYNC_OFFSET,
	       H_ACTIVE + H_SYNC_OFFSET + H_SYNC_WIDTH, htotal);

	printf("\t\tVTimings\t%u %u %u %u\n", V_ACTIVE,
	       V_ACTIVE + V_SYNC_OFFSET,
	       V_ACTIVE + V_SYNC_OFFSET + V_SYNC_WIDTH, vtotal);

	if (INTERLACED || (SYNC_TYPE == SYNC_SEPARATE)) {
		printf("\t\tFlags\t%s\"%sHSync\" \"%sVSync\"\n",
		       INTERLACED ? "\"Interlace\" " : "",
		       HSYNC_POSITIVE ? "+" : "-", VSYNC_POSITIVE ? "+" : "-");
	}

	printf("\tEndMode\n");

	return 0;
}

int block_type(byte * block)
{
	if (!strncmp(edid_v1_descriptor_flag, (char *)block, 2)) {
		/* printf("\t# Block type: 2:%x 3:%x\n", block[2], block[3]); */

		/* descriptor */

		if (block[2] != 0)
			return UNKNOWN_DESCRIPTOR;

		return block[3];
	} else {

		/* detailed timing block */

		return DETAILED_TIMING_BLOCK;
	}
}

char *get_monitor_name(byte const *block)
{
	static char name[13];
	unsigned i;
	byte const *ptr = block + DESCRIPTOR_DATA;

	for (i = 0; i < 13; i++, ptr++) {

		if (*ptr == 0xa) {
			name[i] = 0;
			return name;
		}

		name[i] = *ptr;
	}

	return name;
}

char *get_vendor_sign(byte const *block)
{
	static char sign[4];
	unsigned short h;

	/*
	   08h        WORD    big-endian manufacturer ID (see #00136)
	   bits 14-10: first letter (01h='A', 02h='B', etc.)
	   bits 9-5: second letter
	   bits 4-0: third letter
	 */
	h = COMBINE_HI_8LO(block[0], block[1]);
	sign[0] = ((h >> 10) & 0x1f) + 'A' - 1;
	sign[1] = ((h >> 5) & 0x1f) + 'A' - 1;
	sign[2] = (h & 0x1f) + 'A' - 1;
	sign[3] = 0;
	return sign;
}

int parse_monitor_limits(byte * block)
{
	printf("\tHorizSync %u-%u\n", H_MIN_RATE, H_MAX_RATE);
	printf("\tVertRefresh %u-%u\n", V_MIN_RATE, V_MAX_RATE);
	if (MAX_PIXEL_CLOCK == 10 * 0xff)
		printf("\t# Max dot clock not given\n");
	else
		printf("\t# Max dot clock (video bandwidth) %u MHz\n",
		       (int)MAX_PIXEL_CLOCK);

	if (GTF_SUPPORT) {
		printf("\t# EDID version 3 GTF given: contact author\n");
	}

	return 0;
}

int parse_dpms_capabilities(byte flags)
{
	printf
	    ("\t# DPMS capabilities: Active off:%s  Suspend:%s  Standby:%s\n\n",
	     (flags & DPMS_ACTIVE_OFF) ? "yes" : "no",
	     (flags & DPMS_SUSPEND) ? "yes" : "no",
	     (flags & DPMS_STANDBY) ? "yes" : "no");
	return 0;
}

/* Following functions are taken from Linux sources (drivers/gpu/drm/drm_edid.c)
 * and are:
 * Copyright (c) 2007-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 * Copyright 2010 Red Hat, Inc.
 */

#define DRM_DEBUG_KMS	printf

#define EDID_EST_TIMINGS 16
#define EDID_STD_TIMINGS 8
#define EDID_DETAILED_TIMINGS 4

#define HDMI_IDENTIFIER 0x000C03
#define AUDIO_BLOCK     0x01
#define VENDOR_BLOCK    0x03
#define EDID_BASIC_AUDIO        (1 << 6)

/**
 * Search EDID for CEA extension block.
 */
u8 *drm_find_cea_extension(struct edid * edid)
{
	u8 *edid_ext = NULL;
	int i;

	/* No EDID or EDID extensions */
	if (edid == NULL || edid->extensions == 0) {
		printf("NO CEA extension\n");
		return NULL;
	}

	/* Find CEA extension */
	for (i = 0; i < edid->extensions; i++) {
		edid_ext = (u8 *) edid + EDID_LENGTH * (i + 1);
		if (edid_ext[0] == CEA_EXT)
			break;
	}
	if (i == edid->extensions)
		return NULL;

	return edid_ext;
}

/**
 * drm_detect_hdmi_monitor - detect whether monitor is hdmi.
 * @edid: monitor EDID information
 *
 * Parse the CEA extension according to CEA-861-B.
  * Return true if HDMI, false if not or unknown.
  */
bool drm_detect_hdmi_monitor(struct edid * edid)
{
	u8 *edid_ext;
	int i, hdmi_id;
	int start_offset, end_offset;
	bool is_hdmi = false;

	edid_ext = drm_find_cea_extension(edid);
	if (!edid_ext)
		goto end;

	/* Data block offset in CEA extension block */
	start_offset = 4;
	end_offset = edid_ext[2];

	/*
	 * Because HDMI identifier is in Vendor Specific Block,
	 * search it from all data blocks of CEA extension.
	 */
	for (i = start_offset; i < end_offset;
	     /* Increased by data block len */
	     i += ((edid_ext[i] & 0x1f) + 1)) {
		/* Find vendor specific block */
		if ((edid_ext[i] >> 5) == VENDOR_BLOCK) {
			hdmi_id = edid_ext[i + 1] | (edid_ext[i + 2] << 8) |
			    edid_ext[i + 3] << 16;
			/* Find HDMI identifier */
			if (hdmi_id == HDMI_IDENTIFIER)
				is_hdmi = true;
			break;
		}
	}

end:
	return is_hdmi;
}

 /**
  * drm_detect_monitor_audio - check monitor audio capability
  *
  * Monitor should have CEA extension block.
  * If monitor has 'basic audio', but no CEA audio blocks, it's 'basic
  * audio' only. If there is any audio extension block and supported
  * audio format, assume at least 'basic audio' support, even if 'basic
  * audio' is not defined in EDID.
  *
  */
bool drm_detect_monitor_audio(struct edid * edid)
{
	u8 *edid_ext;
	int i, j;
	bool has_audio = false;
	int start_offset, end_offset;

	edid_ext = drm_find_cea_extension(edid);
	if (!edid_ext)
		goto end;

	has_audio = ((edid_ext[3] & EDID_BASIC_AUDIO) != 0);

	if (has_audio) {
		DRM_DEBUG_KMS("Monitor has basic audio support\n");
		goto end;
	}

	/* Data block offset in CEA extension block */
	start_offset = 4;
	end_offset = edid_ext[2];

	for (i = start_offset; i < end_offset; i += ((edid_ext[i] & 0x1f) + 1)) {
		if ((edid_ext[i] >> 5) == AUDIO_BLOCK) {
			has_audio = true;
			for (j = 1; j < (edid_ext[i] & 0x1f); j += 3)
				DRM_DEBUG_KMS("CEA audio format %d\n",
					      (edid_ext[i + j] >> 3) & 0xf);
			goto end;
		}
	}
end:
	return has_audio;
}

/**
 * add_established_modes - get est. modes from EDID and add them
 * @edid: EDID block to scan
 *
 * Each EDID block contains a bitmap of the supported "established modes" list
 * (defined above).  Tease them out and add them to the global modes list.
 */
static int
check_established_modes(struct edid *edid)
{
	unsigned long est_bits = edid->established_timings.t1 |
		(edid->established_timings.t2 << 8) |
		((edid->established_timings.mfg_rsvd & 0x80) << 9);
	int i, modes = 0;

	for (i = 0; i <= EDID_EST_TIMINGS; i++) {
		if (est_bits & (1<<i)) {
			if ((i == EST_800x600_60) || (i == EST_640x480_60) || (i == EST_1024x768_60)) 
				printf("%s is supported\n", edid_est_modes[i]);
			modes++;
		}
	}

/*	if (version_greater(edid, 1, 0))
		    drm_for_each_detailed_block((u8 *)edid,
						do_established_modes, &closure);
*/
	return modes;
}


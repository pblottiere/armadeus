/*
 * Copyright (C) 2005-2013 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION	11
#define CONFIG_IDENT_STRING	" apf9328 patch 5.9"
#define CONFIG_BOARD_NAME	apf9328

/*
 * SoC configurations
 */
#define CONFIG_ARM920T			/* this is an ARM920T CPU */
#define CONFIG_IMX			/* in a Motorola MC9328MXL Chip */
#define CONFIG_MACH_TYPE	906	/* APF9328 */

/*
 * Enable the call to miscellaneous platform dependent initialization.
 */
#define CONFIG_SYS_DCACHE_OFF

/*
 * Board display option
 */
#undef CONFIG_DISPLAY_BOARDINFO
#undef CONFIG_DISPLAY_CPUINFO

/*
 * SPL - APF9328 does not need SPL
 */

/*
 * BOOTP options
 */
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2

#define CONFIG_HOSTNAME		CONFIG_BOARD_NAME
#define CONFIG_ROOTPATH		"/tftpboot/" __stringify(CONFIG_BOARD_NAME) "-root"

/*
 * U-Boot Commands
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_ASKENV	/* ask for env variable		*/
#define CONFIG_CMD_BSP		/* Board Specific functions	*/
#define CONFIG_CMD_CACHE	/* icache, dcache		*/
#define CONFIG_CMD_DATE		/* support for RTC, date/time.. */
#define CONFIG_CMD_DHCP		/* DHCP Support			*/
#define CONFIG_CMD_DNS
#define CONFIG_CMD_EEPROM	/* EEPROM read/write support	*/
#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_IMLS		/* List all found images	*/
#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/
#define CONFIG_CMD_MTDPARTS	/* MTD partition support	*/
#define CONFIG_CMD_NFS
#define CONFIG_CMD_PING		/* ping support			*/
/*#define CONFIG_CMD_SAVES*/	/* save S record dump		*/
#define CONFIG_CMD_SETEXPR	/* setexpr support		*/


/*
 * Memory configurations
 */
#define CONFIG_NR_DRAM_BANKS 1

#define CONFIG_SYS_SDRAM_MBYTE_SYZE 16

#define PHYS_SDRAM_1			0x08000000 /* SDRAM bank #1 32bits */
#define	CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1

#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (128<<10))
#define CONFIG_SYS_MEMTEST_START	0x08000000	/* memtest test area */
#define CONFIG_SYS_MEMTEST_END		0x08700000

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE	\
		+ PHYS_SDRAM_1_SIZE - 0x0100000)

/* lowlevel_linit copy U-Boot at CONFIG_SYS_INIT_SP_ADDR  (here in RAM) */
/* making u-boot runnable from flash and also RAM that is usefull to boot */
/* from serial port and from flash with only one version of U-Boot */
#define CONFIG_SYS_TEXT_BASE	0x08000000

/*
 * FLASH organization
 */
#define	CONFIG_SYS_MONITOR_BASE	0x10000000
#define	CONFIG_SYS_MONITOR_LEN	0x00040000	/* 256kB ( 2 flash sector ) */
#define	CONFIG_ENV_IS_IN_FLASH
#define	CONFIG_ENV_OVERWRITE
#define	CONFIG_ENV_ADDR		0x10040000	/* absolute address for now */
#define	CONFIG_ENV_SIZE		0x00020000
#define	CONFIG_ENV_SECT_SIZE	0x00020000
#define	CONFIG_ENV_ADDR_REDUND	\
		(CONFIG_ENV_ADDR + CONFIG_ENV_SIZE)	/* +128kB */
#define	CONFIG_ENV_SIZE_REDUND	CONFIG_ENV_SIZE	/* 128kB */
#define	CONFIG_FIRMWARE_ADDR	0x10080000
#define	CONFIG_FIRMWARE_SIZE	0x00040000	/* 256kB ( 2 flash sector ) */
#define	CONFIG_KERNEL_ADDR	0x100C0000
#define	CONFIG_ROOTFS_ADDR	0x102C0000

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTDMAP	"physmap-flash.0"
#define MTDIDS_DEFAULT	"nor0=" CONFIG_MTDMAP
#define MTDPARTS_DEFAULT "mtdparts=" CONFIG_MTDMAP \
		":256k(u-boot)ro,"	\
		"128k(env),"		\
		"128k(env2),"		\
		"256k(firmware),"	\
		"2048k(kernel),"	\
		"-(rootfs)"

/*
 * U-Boot general configurations
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT		"BIOS> "	/* prompt string */
#define CONFIG_SYS_CBSIZE		2048		/* console I/O buffer*/
#define CONFIG_SYS_PBSIZE		\
				(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
						/* Print buffer size */
#define CONFIG_SYS_MAXARGS		16		/* max command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
						/* Boot argument buffer size */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_HUSH_PARSER			/* enable the "hush" shell */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "	/* secondary prompt string */
#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_PREBOOT			"run check_flash checknload_firmware " \
					"check_env;"

/*
 * Boot Linux
 */
#define CONFIG_CMDLINE_TAG		/* send commandline to Kernel	*/
#define CONFIG_SETUP_MEMORY_TAGS	/* send memory definition to kernel */
#define CONFIG_INITRD_TAG		/* send initrd params	*/
/*
#define CONFIG_REVISION_TAG
#define CONFIG_OF_LIBFDT
*/
#define CONFIG_BOOTDELAY	5
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		__stringify(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," \
				__stringify(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
				" root=/dev/mtdblock5 rootfstype=jffs2 "

#define ACFG_CONSOLE_DEV	ttySMX0
#define CONFIG_BOOTCOMMAND		"run jffsboot"
#define CONFIG_SYS_AUTOLOAD		"no"
/*
 * Default load address for user programs and kernel
 */
#define	CONFIG_LOADADDR			0x08000000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/*
 * Extra Environments
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"env_version="		__stringify(CONFIG_ENV_VERSION)		"\0" \
	"consoledev="		__stringify(ACFG_CONSOLE_DEV)		"\0" \
	"mtdparts="		MTDPARTS_DEFAULT			"\0" \
	"partition=nor0,5\0"						\
	"uboot_addr="		__stringify(CONFIG_SYS_MONITOR_BASE)	"\0" \
	"env_addr="		__stringify(CONFIG_ENV_ADDR)		"\0" \
	"firmware_addr="	__stringify(CONFIG_FIRMWARE_ADDR)	"\0" \
	"firmware_size="	__stringify(CONFIG_FIRMWARE_SIZE)	"\0" \
	"kernel_addr="		__stringify(CONFIG_KERNEL_ADDR)		"\0" \
	"rootfs_addr="		__stringify(CONFIG_ROOTFS_ADDR)		"\0" \
	"board_name="		__stringify(CONFIG_BOARD_NAME)	"\0"	\
	"check_env=if test -n ${flash_env_version}; "			\
		"then env default env_version; "			\
		"else env set flash_env_version ${env_version}; env save; "\
		"fi; "							\
		"if itest ${flash_env_version} != ${env_version}; then " \
			"echo \"*** Warning - Environment version"	\
			" change suggests: run flash_reset_env; reset\"; "\
			"env default flash_reset_env; "			\
		"else exit; fi; \0"					\
	"check_flash=protect on nor0,0;\0"				\
	"flash_reset_env=env default -f -a && saveenv &&"		\
		"echo Flash environment variables erased!\0"		\
	"download_uboot=tftpboot ${loadaddr} ${board_name}-u-boot.bin\0"\
	"flash_uboot=protect off nor0,0;"				\
		"echo Erasing FLASH;"					\
		"era nor0,0;"						\
		"if cp.b ${fileaddr} ${uboot_addr} ${filesize};"	\
		"then protect on nor0,0;"				\
			"echo Flashing of uboot succeed;"		\
		"else echo Flashing of uboot failed;"			\
		"fi; \0"						\
	"update_uboot=run download_uboot flash_uboot\0"			\
	"download_env=tftpboot ${loadaddr} ${board_name}"		\
		"-u-boot-env.txt\0"					\
	"flash_env=env import -t ${loadaddr} && env save; \0"		\
	"update_env=run download_env flash_env\0"			\
	"download_firmware=tftpboot ${loadaddr} ${board_name}-firmware.bin\0"\
	"flash_firmware=echo Erasing FLASH;era nor0,3;"			\
		"if cp.b ${fileaddr} ${firmware_addr} ${filesize} ;"	\
		"then echo Flashing of Firmware succeed;"		\
		"else echo Flashing of Firmware failed;fi\0"		\
	"update_firmware=run download_firmware flash_firmware\0"	\
	"load_firmware=fpga loadb 0 ${firmware_addr} ${firmware_size} || "\
		"fpga load 0 ${firmware_addr} ${firmware_size};"	\
		"if test $? -eq 0;"					\
		"then echo Firmware successfully loaded;"		\
		"else echo Loading firmware failed!; fi;\0"		\
	"checknload_firmware=if test -n ${firmware_autoload};"		\
		"then run load_firmware; else exit; fi;\0"		\
	"download_kernel=tftpboot ${loadaddr} ${board_name}-linux.bin\0"\
	"flash_kernel=echo Erasing FLASH;era nor0,4;"			\
		"if cp.b ${fileaddr} ${kernel_addr} ${filesize} ;"	\
		"then echo Flashing of kernel succeed;"			\
		"else echo Flashing of kernel failed;fi\0"		\
	"update_kernel=run download_kernel flash_kernel\0"	\
	"download_rootfs=tftpboot ${loadaddr} ${board_name}-rootfs.jffs2\0" \
	"flash_rootfs=echo Erasing FLASH;era nor0,5;"			\
		"if cp.b ${fileaddr} ${rootfs_addr} ${filesize};"	\
		"then echo Flashing of rootfs succeed;"			\
		"else echo Flashing of rootfs failed;fi\0"		\
	"update_rootfs=run download_rootfs flash_rootfs\0"		\
	"update_all=run update_kernel update_rootfs update_uboot &&"	\
		"if test -n ${firmware_autoload};"			\
		"then run update_firmware; else exit; fi;\0"		\
	"addipargs=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}"\
		":${gatewayip}:${netmask}:${hostname}:eth0:off\0"	\
	"addmmcargs=setenv bootargs ${bootargs} root=/dev/mmcblk0p1 "	\
		"rootfstype=ext2\0"					\
	"mmcboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addmmcargs addipargs;"\
		"bootm ${kernel_addr}\0"				\
	"addnfsargs=setenv bootargs ${bootargs} root=/dev/nfs rw "	\
		"nfsroot=${serverip}:${rootpath}\0"			\
	"nfsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addnfsargs addipargs;"\
		"nfs ${loadaddr} ${serverip}:${rootpath}/boot/${board_name}-"\
		"linux.bin && bootm ${loadaddr}\0"			\
	"addjffsargs=setenv bootargs ${bootargs} root=/dev/mtdblock5 "	\
		"rootfstype=jffs2\0"					\
	"jffsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addjffsargs addipargs;"\
		"bootm ${kernel_addr}\0"				\
	"unlock_regs=mw 00200008 0; mw 00210008 0\0"			\

/*
 * Serial Driver
 */
#define CONFIG_IMX_SERIAL
#define CONFIG_IMX_SERIAL1
#define CONFIG_BAUDRATE		115200

/*
 * GPIO
 */

/*
 * NOR
 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* FLASH banks count */
#define CONFIG_SYS_MAX_FLASH_SECT	256	/* max 32MB of FLASH memory */
#define CONFIG_SYS_FLASH_BASE		0x10000000

#define	CONFIG_SYS_FLASH_CFI
#define CONFIG_FLASH_CFI_DRIVER
#define CONFIG_FLASH_CFI_MTD
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE
#define CONFIG_SYS_FLASH_PROTECTION
#define CONFIG_SYS_DIRECT_FLASH_TFTP
#define CONFIG_FLASH_SHOW_PROGRESS		45
/*
 * This is setting for JFFS2 support in u-boot.
 * NOTE: Enable CONFIG_SYS_CMD_JFFS2 for JFFS2 support.
 */
#define CONFIG_SYS_JFFS_CUSTOM_PART

/*
 * Ethernet
 */
/* DM9000 ethernet chipset assigned to CS4 */
/* For the apf board DM9000 base is 0x15C00000 and data is base+2	*/
/* For the apm board DM9000 base is 0x15C3FFFC and data is base+4	*/
#define CONFIG_DRIVER_DM9000
#define CONFIG_DM9000_BASE	0x15C00000
#define DM9000_IO		CONFIG_DM9000_BASE
#define DM9000_DATA		(CONFIG_DM9000_BASE+2)
#define CONFIG_NETCONSOLE

/*
 * FPGA
 */
#define CONFIG_FPGA
#define CONFIG_FPGA_COUNT	1
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_SPARTAN3
#define CONFIG_SYS_FPGA_WAIT		250	/* 250 ms */
#define CONFIG_SYS_FPGA_PROG_FEEDBACK
#define CONFIG_SYS_FPGA_CHECK_CTRLC
#define CONFIG_SYS_FPGA_CHECK_ERROR

/* FPGA program pin configuration */
#define CONFIG_SYS_FPGA_PRG	(GPIO_PORTB | 18) /* prog pin (SSI output) */
#define CONFIG_SYS_FPGA_CLK	(GPIO_PORTB | 19) /* clk pin (SSI output) */
#define CONFIG_SYS_FPGA_DATA	(GPIO_PORTB | 17) /* data pin (SSI output) */
#define CONFIG_SYS_FPGA_INIT	(GPIO_PORTB | 15) /* init pin (SSI input) */
#define CONFIG_SYS_FPGA_DONE	(GPIO_PORTB | 16) /* done pin (SSI input) */

/*
 * I2C bus
 */
#define	CONFIG_HARD_I2C			/* I2C with hardware support	*/

#define CONFIG_SYS_I2C_SPEED		400000	/* 400 kHz */
#define CONFIG_SYS_I2C_SLAVE		0x7F

#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* EEPROM 24LC256	*/
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	2	/* bytes of address	*/
/* mask of address bits that overflow into the "EEPROM chip address"	*/
/*#define CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW	0x00*/
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	6
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10 /* takes up to 10 msec */

/*
 * RTC
 */
#define CONFIG_RTC_DS1374

/*
 * Clocks
 */
/*
 * PLL configuration
 *
 * f_{dpll}=2*f{ref}*(MFI+MFN/(MFD+1))/(PD+1)
 * f_ref=16,777216MHz
 * 32768 Hz xtal
 * 0x07B32DA5: 192.0000173
 * 0x002a141f: 191,9944MHz
 * 0x040b2007: 144MHz
 * 0x0FB32DA5: 96.00000864 MHz
 * 0x042a141f: 96MHz
 * 0x0811140d: 64MHz
 * 0x040e200e: 150MHz
 * 0x00321431: 200MHz
 *
 *16 MHz xtal
 * 0x08001800: 64MHz mit 16er Quarz
 * 0x04001800: 96MHz mit 16er Quarz
 * 0x04002400: 144MHz mit 16er Quarz
 *
 * 31	|x x x x|x x x x|x x x x|x x x x|x x x x|x x x x|x x x x|x x x x| 0
 *	|XXX|--PD---|-------MFD---------|XXX|--MFI--|-----MFN-----------|
 */

#define	CONFIG_SYS_HZ		1000	/* Ticks per second */
#define CONFIG_SYS_OSC32	32768	/* 32768 or 32000 Hz crystal */
#undef	CONFIG_SYS_OSC16	/* there is no external 16MHz external clock */

/* MPU CLOCK source before PLL */
#define CONFIG_SYS_CLK_FREQ	(512*CONFIG_SYS_OSC32)
#define CONFIG_SYS_MPCTL0_VAL		0x07B32DA5	/* 192.000017 MHz */
#define CONFIG_SYS_MPCTL1_VAL		0

/* system clock source before PLL */
#ifndef CONFIG_SYS_OSC16
#define CONFIG_SYSPLL_CLK_FREQ		(512*CONFIG_SYS_OSC32)
#if (CONFIG_SYS_OSC32 == 32000)
#define CONFIG_SYS_SPCTL0_VAL		0x043F1437	/* 96 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0
#define CONFIG_SYS_FREQ			96		/* MHz */
#else /* CONFIG_SYS_OSC32 == 32768 */
#define CONFIG_SYS_SPCTL0_VAL		0x0FB32DA5	/* 96.000009 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0
#define CONFIG_SYS_FREQ			96		/* MHz */
#endif /* CONFIG_SYS_OSC32 */
#else /* CONFIG_SYS_OSC16 in use */
#define CONFIG_SYSPLL_CLK_FREQ		CONFIG_SYS_OSC16
#define CONFIG_SYS_SPCTL0_VAL		0x04001401	/* 96 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0x0C000040
#define CONFIG_SYS_FREQ			96		/* MHz */
#endif /* CONFIG_SYS_OSC16 */

/* external bus frequency */
#define CONFIG_SYS_BUS_FREQ	96	/* 96|48... MHz (BCLOCK and HCLOCK) */
#define CONFIG_USB_FREQ		48	/* 48 MHz */
#define CONFIG_PERIF1_FREQ	16	/* 16 MHz UART, Timer PWM */
#define CONFIG_PERIF2_FREQ	48	/* 48 MHz LCD SD SPI */
#define CONFIG_PERIF3_FREQ	16	/* 16 MHz SSI */

/*
 * SDRAM definition parameter
 */
/*
 * CONFIG_SYS_SDRAM_NUM_COL 8, 9, 10 or 11 column address bits
 * CONFIG_SYS_SDRAM_NUM_ROW 11, 12 or 13 row address bits
 * CONFIG_SYS_SDRAM_REFRESH 0=OFF 1=2048 2=4096 3=8192 refresh
 * CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1 X ns clock cycle time when CL=1
 * CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY X ns SRP
 * CONFIG_SYS_SDRAM_ROW_2_COL_DELAY X ns SRCD
 * CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY X ns SRC
 * CONFIG_SYS_SDRAM_BURST_LENGTH 2^N BYTES (N=0..3)
 * CONFIG_SYS_SDRAM_SINGLE_ACCESS 1= single access; 0 = Burst mode
 */

#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 16)
/* micron 16MB */
#define PHYS_SDRAM_1_SIZE	0x01000000	/* 16 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		8
#define CONFIG_SYS_SDRAM_NUM_ROW		12
#define CONFIG_SYS_SDRAM_REFRESH		2
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	20
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_2_COL_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY	70
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0
#endif

#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 32)
/* micron 32MB */
#define PHYS_SDRAM_1_SIZE	0x02000000	/* 32 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		9
#define CONFIG_SYS_SDRAM_NUM_ROW		12
#define CONFIG_SYS_SDRAM_REFRESH		2
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	20
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_2_COL_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY	70
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0
#endif

#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 64)
/* micron 64MB */
#define PHYS_SDRAM_1_SIZE	0x04000000	/* 64 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		9
#define CONFIG_SYS_SDRAM_NUM_ROW		13
#define CONFIG_SYS_SDRAM_REFRESH		3
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	20
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_2_COL_DELAY	20
#define CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY	70
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0
#endif

/*
 * External interface
 */
/*
 * CSxU_VAL:
 * 63|    x    |x|x x|x x x x|x x| x | x  |x x x x|48
 *   |DTACK_SEL|0|BCD|  BCS  |PSZ|PME|SYNC|  DOL  |
 *
 * 47| x x  | x x x x x x | x | x x x x | x x x x |32
 *   | CNC  |     WSC     | 0 |   WWS   |   EDC   |
 *
 * CSxL_VAL:
 * 31|  x x x x  | x x x x  | x x x x  | x x x x  |24
 *   |    OEA    |   OEN    |   WEA    |   WEN    |
 * 23|x x x x| x | x x x | x x  x x |x x| x |  x  | 0
 *   |  CSA  |EBC|  DSZ  | 0|SP|0|WP|0 0|PA |CSEN |
 */
/* CS0 configuration for flash memory Micron MT28F128J3-150 */
#define CONFIG_SYS_CS0_CHIP_SELECT_ENABLE	1 /* 1 : enable CS0 */
#define CONFIG_SYS_CS0_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits..*/
#define CONFIG_SYS_CS0_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS0_WRITE_PROTECT		0 /* 1 wr access prohibited*/
#define CONFIG_SYS_CS0_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/
#define CONFIG_SYS_CS0_READ_CYC_LGTH		150	/* ns */
#define CONFIG_SYS_CS0_OE_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS0_OE_NEG_DLY		0	/* ns */

#define CONFIG_SYS_CS0_CS_NEG_LGTH		0 /* max 30ns CS HIGH to LOW */
#define CONFIG_SYS_CS0_XTRA_DEAD_CYC		35 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS0_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS0_EB_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS0_EB_NEG_DLY		0	/* ns */
#define CONFIG_SYS_CS0_CS_ASSERT_NEG_DLY	0	/* ns */

#define CONFIG_SYS_CS0_SYNC_ENABLE		1 /* enable sync burst mode */
#define CONFIG_SYS_CS0_SYNC_PAGE_MODE_EMUL	1
#define CONFIG_SYS_CS0_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS0_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS0_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS0_SYNC_DAT_OUT_LGTH	25	/* ns */

/* CS1 configuration for */
#define CONFIG_SYS_CS1_CHIP_SELECT_ENABLE	1 /* 1: enable CS1 */
#define CONFIG_SYS_CS1_PIN_ASSERT		0 /* chip select pin state */
						/* when chip select disabled */
#define CONFIG_SYS_CS1_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits.. */
#define CONFIG_SYS_CS1_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS1_WRITE_PROTECT		0 /* 1 wr access prohibited */
#define CONFIG_SYS_CS1_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/

#define CONFIG_SYS_CS1_READ_CYC_LGTH		50	/* ns */
#define CONFIG_SYS_CS1_OE_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS1_OE_NEG_DLY		0	/* ns */

#define CONFIG_SYS_CS1_CS_NEG_LGTH		0 /* max 30ns CS HIGH to LOW*/
#define CONFIG_SYS_CS1_XTRA_DEAD_CYC		0 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS1_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS1_EB_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS1_EB_NEG_DLY		0	/* ns */
#define CONFIG_SYS_CS1_CS_ASSERT_NEG_DLY	0	/* ns */

#define CONFIG_SYS_CS1_SYNC_ENABLE		0 /* enable sync burst mode */
#define CONFIG_SYS_CS1_SYNC_PAGE_MODE_EMUL	0
#define CONFIG_SYS_CS1_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS1_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS1_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS1_SYNC_DAT_OUT_LGTH	0 /* ns */

/* CS2 configuration for */
#define CONFIG_SYS_CS2_CHIP_SELECT_ENABLE	0 /* 1 : enable CS0 */
#define CONFIG_SYS_CS2_PIN_ASSERT		0 /* chip select pin state */
						/* when chip select disabled */
#define CONFIG_SYS_CS2_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits.. */
#define CONFIG_SYS_CS2_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS2_WRITE_PROTECT		0 /* 1 wr access prohibited */
#define CONFIG_SYS_CS2_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/

#define CONFIG_SYS_CS2_READ_CYC_LGTH		0	/* ns */
#define CONFIG_SYS_CS2_OE_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS2_OE_NEG_DLY		0	/* ns */

#define CONFIG_SYS_CS2_CS_NEG_LGTH		0 /* max 30ns CS HIGH to LOW*/
#define CONFIG_SYS_CS2_XTRA_DEAD_CYC		0 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS2_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS2_EB_ASSERT_DLY		0	/* ns */
#define CONFIG_SYS_CS2_EB_NEG_DLY		0	/* ns */
#define CONFIG_SYS_CS2_CS_ASSERT_NEG_DLY	0	/* ns */

#define CONFIG_SYS_CS2_SYNC_ENABLE		0 /* enable sync burst mode */
#define CONFIG_SYS_CS2_SYNC_PAGE_MODE_EMUL	0
#define CONFIG_SYS_CS2_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS2_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS2_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS2_SYNC_DAT_OUT_LGTH	0 /* ns */

/* CS3 configuration for ISP1760 */
#define CONFIG_SYS_CS3_CHIP_SELECT_ENABLE	1 /* 1 : enable CS0 */
#define CONFIG_SYS_CS3_PIN_ASSERT		0 /* chip select pin state */
						/* when chip select disabled */
#define CONFIG_SYS_CS3_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits.. */
#define CONFIG_SYS_CS3_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS3_WRITE_PROTECT		0 /* 1 wr access prohibited */
#define CONFIG_SYS_CS3_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/

#define CONFIG_SYS_CS3_READ_CYC_LGTH		110	/* ns */
#define CONFIG_SYS_CS3_OE_ASSERT_DLY		45	/* ns */
#define CONFIG_SYS_CS3_OE_NEG_DLY		40	/* ns */

#define CONFIG_SYS_CS3_CS_NEG_LGTH		30 /* max 30ns CS HIGH to LOW*/
#define CONFIG_SYS_CS3_XTRA_DEAD_CYC		0 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS3_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS3_EB_ASSERT_DLY		45	/* ns */
#define CONFIG_SYS_CS3_EB_NEG_DLY		45	/* ns */
#define CONFIG_SYS_CS3_CS_ASSERT_NEG_DLY	35	/* ns */

#define CONFIG_SYS_CS3_SYNC_ENABLE		0 /* enable sync burst mode */
#define CONFIG_SYS_CS3_SYNC_PAGE_MODE_EMUL	0
#define CONFIG_SYS_CS3_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS3_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS3_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS3_SYNC_DAT_OUT_LGTH	0 /* ns */

/* CS4 configuration for DM9000 ethernet chipset */
#define CONFIG_SYS_CS4_CHIP_SELECT_ENABLE	1 /* 1 : enable CS0 */
#define CONFIG_SYS_CS4_PIN_ASSERT		0 /* chip select pin state */
						/* when chip select disabled */
#define CONFIG_SYS_CS4_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits.. */
#define CONFIG_SYS_CS4_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS4_WRITE_PROTECT		0 /* 1 wr access prohibited */
#define CONFIG_SYS_CS4_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/

#define CONFIG_SYS_CS4_READ_CYC_LGTH		72 /* ns */
#define CONFIG_SYS_CS4_OE_ASSERT_DLY		5 /* ns */
#define CONFIG_SYS_CS4_OE_NEG_DLY		45 /* ns */

#define CONFIG_SYS_CS4_CS_NEG_LGTH		30 /* max 30ns CS HIGH to LOW*/
#define CONFIG_SYS_CS4_XTRA_DEAD_CYC		0 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS4_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS4_EB_ASSERT_DLY		5	/* ns */
#define CONFIG_SYS_CS4_EB_NEG_DLY		45	/* ns */
#define CONFIG_SYS_CS4_CS_ASSERT_NEG_DLY	0	/* ns */

#define CONFIG_SYS_CS4_SYNC_ENABLE		0 /* enable sync burst mode */
#define CONFIG_SYS_CS4_SYNC_PAGE_MODE_EMUL	0
#define CONFIG_SYS_CS4_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS4_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS4_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS4_SYNC_DAT_OUT_LGTH	0 /* ns */

/* CS5 configuration for */
#define CONFIG_SYS_CS5_CHIP_SELECT_ENABLE	1 /* 1 : enable CS0 */
#define CONFIG_SYS_CS5_PIN_ASSERT		0 /* chip select pin state */
						/* when chip select disabled */
#define CONFIG_SYS_CS5_DATA_PORT_SIZE		0x5 /* 5=16 bits on D[15:0] */
					/* 3=8bits on D[7:0] 6=32 bits.. */
#define CONFIG_SYS_CS5_SUPERVISOR_PROTECT	0 /* 1 user access prohibited*/
#define CONFIG_SYS_CS5_WRITE_PROTECT		0 /* 1 wr access prohibited */
#define CONFIG_SYS_CS5_EB_SIGNAL_CONTROL_WRITE	1 /* 1 EB used as wr signal*/
#define CONFIG_SYS_CS5_DTACK_SELECT		1 /* Activate DTACK fonction */
						/* (asynchrone bus) */

#define CONFIG_SYS_CS5_READ_CYC_LGTH		-1 /* ns or -1 DTACK function*/
#define CONFIG_SYS_CS5_OE_ASSERT_DLY		10	/* ns */
#define CONFIG_SYS_CS5_OE_NEG_DLY		10	/* ns */

#define CONFIG_SYS_CS5_CS_NEG_LGTH		10 /* max 30ns CS HIGH to LOW*/
#define CONFIG_SYS_CS5_XTRA_DEAD_CYC		0 /* ns CS HIGH to tristate */

#define CONFIG_SYS_CS5_WRITE_XTRA_LGTH		0	/* ns */
#define CONFIG_SYS_CS5_EB_ASSERT_DLY		10	/* ns */
#define CONFIG_SYS_CS5_EB_NEG_DLY		10	/* ns */
#define CONFIG_SYS_CS5_CS_ASSERT_NEG_DLY	0	/* ns */

#define CONFIG_SYS_CS5_SYNC_ENABLE		0 /* enable sync burst mode */
#define CONFIG_SYS_CS5_SYNC_PAGE_MODE_EMUL	0
#define CONFIG_SYS_CS5_SYNC_PAGE_SIZE		0 /* 4 words page size */
#define CONFIG_SYS_CS5_SYNC_BURST_CLK_START	0 /* 0 ns burst clock delay */
#define CONFIG_SYS_CS5_SYNC_BURST_CLK_DIV	0 /* 0 : divider is 1 */
#define CONFIG_SYS_CS5_SYNC_DAT_OUT_LGTH	0	/* ns */

/*
 * Default configuration for GPIOs and peripherals
 */
#define CONFIG_SYS_DDIR_A_VAL		0x00000000
#define CONFIG_SYS_OCR1_A_VAL		0x00000000
#define CONFIG_SYS_OCR2_A_VAL		0x00000000
#define CONFIG_SYS_ICFA1_A_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_A_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_A_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_A_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_A_VAL		0x00000000

/* Setup for PA23 which is Reset Default PA23 but has to become CS5 */
#define CONFIG_SYS_GIUS_A_VAL		0x00007FF8
#define CONFIG_SYS_ICR1_A_VAL		0x00000000
#define CONFIG_SYS_ICR2_A_VAL		0x00000000
#define CONFIG_SYS_IMR_A_VAL		0x00000000
#define CONFIG_SYS_GPR_A_VAL		0x00000000
#define CONFIG_SYS_PUEN_A_VAL		0xFFFFFFFF

#define CONFIG_SYS_DDIR_B_VAL		0x00000000
#define CONFIG_SYS_OCR1_B_VAL		0x00000000
#define CONFIG_SYS_OCR2_B_VAL		0x00000000
#define CONFIG_SYS_ICFA1_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_B_VAL		0x00000000
#define CONFIG_SYS_GIUS_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICR1_B_VAL		0x00000000
#define CONFIG_SYS_ICR2_B_VAL		0x00000000
#define CONFIG_SYS_IMR_B_VAL		0x00000000
#define CONFIG_SYS_GPR_B_VAL		0x00000000
#define CONFIG_SYS_PUEN_B_VAL		0xFFFFFFFF

#define CONFIG_SYS_DDIR_C_VAL		0x00000000
#define CONFIG_SYS_OCR1_C_VAL		0x00000000
#define CONFIG_SYS_OCR2_C_VAL		0x00000000
#define CONFIG_SYS_ICFA1_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_C_VAL		0x00000000
#define CONFIG_SYS_GIUS_C_VAL		0x0007FFFF
#define CONFIG_SYS_ICR1_C_VAL		0x00000000
#define CONFIG_SYS_ICR2_C_VAL		0x00000000
#define CONFIG_SYS_IMR_C_VAL		0x00000000
#define CONFIG_SYS_GPR_C_VAL		0x00000000
#define CONFIG_SYS_PUEN_C_VAL		0xF913FFFF

#define CONFIG_SYS_DDIR_D_VAL		0x00000000
#define CONFIG_SYS_OCR1_D_VAL		0x00000000
#define CONFIG_SYS_OCR2_D_VAL		0x00000000
#define CONFIG_SYS_ICFA1_D_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_D_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_D_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_D_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_D_VAL		0x00000000
#define CONFIG_SYS_GIUS_D_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICR1_D_VAL		0x00000000
#define CONFIG_SYS_ICR2_D_VAL		0x00000000
#define CONFIG_SYS_IMR_D_VAL		0x00000000
#define CONFIG_SYS_GPR_D_VAL		0x00000000
#define CONFIG_SYS_PUEN_D_VAL		0xFFFFFFFF

#define CONFIG_SYS_GPCR_VAL		0x000003AB

/* FMCR Bit 1 becomes 0 to make CS3 if we have only one sdram bank*/
#if (CONFIG_NR_DRAM_BANKS == 1)
#define CONFIG_SYS_FMCR_VAL 0x00000001
#elif (CONFIG_NR_DRAM_BANKS == 2)
#define CONFIG_SYS_FMCR_VAL 0x00000003
#endif

/*
 * From here, there should not be any user configuration.
 * All Equations are automatic
 */

#define CONFIG_SYS_HCLK_LGTH		(1000/CONFIG_SYS_BUS_FREQ) /* ns */

/* USB 48 MHz ; BUS 96MHz*/
#ifdef CONFIG_SYS_OSC16
#define CSCR_MASK 0x23030003
#else
#define CSCR_MASK 0x23000003
#endif
#define CONFIG_SYS_CSCR_VAL\
	(CSCR_MASK						\
	|((((CONFIG_SYS_FREQ/CONFIG_USB_FREQ)-1)&0x07)<<26)	\
	|((((CONFIG_SYS_FREQ/CONFIG_SYS_BUS_FREQ)-1)&0x0F)<<10))

/* PERCLKx 16MHz */
#define CONFIG_SYS_PCDR_VAL\
	(((((CONFIG_SYS_FREQ/CONFIG_PERIF1_FREQ)-1)&0x0F)<<0)	\
	|((((CONFIG_SYS_FREQ/CONFIG_PERIF2_FREQ)-1)&0x0F)<<4)	\
	|((((CONFIG_SYS_FREQ/CONFIG_PERIF3_FREQ)-1)&0x7F)<<16))

/* SDRAM controller programming Values */
#if ((CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1 > (3*CONFIG_SYS_HCLK_LGTH))\
	|| (CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1 < 1))
#define REG_FIELD_SCL_VAL 3
#else
#define REG_FIELD_SCL_VAL\
	((CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1+CONFIG_SYS_HCLK_LGTH-1)\
	/CONFIG_SYS_HCLK_LGTH)
#endif

#if ((CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY > (2*CONFIG_SYS_HCLK_LGTH))\
	|| (CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY < 1))
#define REG_FIELD_SRP_VAL 0
#else
#define REG_FIELD_SRP_VAL 1
#endif

#if (CONFIG_SYS_SDRAM_ROW_2_COL_DELAY > (3*CONFIG_SYS_HCLK_LGTH))
#define REG_FIELD_SRCD_VAL 0
#else
#define REG_FIELD_SRCD_VAL\
	((CONFIG_SYS_SDRAM_ROW_2_COL_DELAY+CONFIG_SYS_HCLK_LGTH-1)\
	/CONFIG_SYS_HCLK_LGTH)
#endif

#if (CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY > (7*CONFIG_SYS_HCLK_LGTH))
#define REG_FIELD_SRC_VAL 0
#else
#define REG_FIELD_SRC_VAL\
	((CONFIG_SYS_SDRAM_ROW_CYCLE_DELAY+CONFIG_SYS_HCLK_LGTH-1)\
	/CONFIG_SYS_HCLK_LGTH)
#endif

#define REG_SDCTL_BASE_CONFIG (0x800a1000\
				| (((CONFIG_SYS_SDRAM_NUM_ROW-11)&0x3)<<24)\
				| (((CONFIG_SYS_SDRAM_NUM_COL-8)&0x3)<<20)\
				| (((CONFIG_SYS_SDRAM_REFRESH)&0x3)<<14)\
				| (((REG_FIELD_SCL_VAL)&0x3)<<8)\
				| (((REG_FIELD_SRP_VAL)&0x1)<<6)\
				| (((REG_FIELD_SRCD_VAL)&0x3)<<4)\
				| (((REG_FIELD_SRC_VAL)&0x7)<<0))

#define CONFIG_SYS_NORMAL_RW_CMD	((0x0<<28)+REG_SDCTL_BASE_CONFIG)
#define CONFIG_SYS_PRECHARGE_CMD	((0x1<<28)+REG_SDCTL_BASE_CONFIG)
#define CONFIG_SYS_AUTOREFRESH_CMD	((0x2<<28)+REG_SDCTL_BASE_CONFIG)
#define CONFIG_SYS_SET_MODE_REG_CMD	((0x3<<28)+REG_SDCTL_BASE_CONFIG)

/* Issue Mode register Command to SDRAM*/
#define CONFIG_SYS_SDRAM_MODE_REGISTER_VAL\
	((((CONFIG_SYS_SDRAM_BURST_LENGTH)&0x7)<<(CONFIG_SYS_SDRAM_NUM_COL+4))\
	| (((REG_FIELD_SCL_VAL)&0x3)<<(CONFIG_SYS_SDRAM_NUM_COL+4+4))\
	| (((CONFIG_SYS_SDRAM_SINGLE_ACCESS)&0x1)\
		<<(CONFIG_SYS_SDRAM_NUM_COL+4+9)))

/* Issue Precharge all Command to SDRAM*/
#define CONFIG_SYS_SDRAM_PRECHARGE_ALL_VAL\
	(((CONFIG_SYS_SDRAM_SINGLE_ACCESS)&0x1)<<(CONFIG_SYS_SDRAM_NUM_COL+13))

/*
 * CSxU_VAL:
 * 63|    x    |x|x x|x x x x|x x| x | x  |x x x x|48
 *   |DTACK_SEL|0|BCD|  BCS  |PSZ|PME|SYNC|  DOL  |
 *
 * 47| x x  | x x x x x x | x | x x x x | x x x x |32
 *   | CNC  |     WSC     | 0 |   WWS   |   EDC   |
 *
 * CSxL_VAL:
 * 31|  x x x x  | x x x x  | x x x x  | x x x x  |24
 *   |    OEA    |   OEN    |   WEA    |   WEN    |
 * 23|x x x x| x | x x x | x x  x x |x x| x |  x  | 0
 *   |  CSA  |EBC|  DSZ  | 0|SP|0|WP|0 0|PA |CSEN |
 */
#define CONFIG_SYS_CS0U_VAL\
	((((CONFIG_SYS_CS0_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS0_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS0_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS0_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|((((CONFIG_SYS_CS0_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|((CONFIG_SYS_CS0_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS0_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS0_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS0_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS0_SYNC_BURST_CLK_DIV&0x03)<<28))

#define CONFIG_SYS_CS0L_VAL\
	((CONFIG_SYS_CS0_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS0_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS0_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS0_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS0_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS0_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS0_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS0_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS0_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS0_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#define CONFIG_SYS_CS1U_VAL\
	((((CONFIG_SYS_CS1_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS1_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS1_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS1_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|((((CONFIG_SYS_CS1_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|((CONFIG_SYS_CS1_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS1_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS1_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS1_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS1_SYNC_BURST_CLK_DIV&0x03)<<28))

#define CONFIG_SYS_CS1L_VAL\
	((CONFIG_SYS_CS1_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS1_PIN_ASSERT&0x01)<<1)\
	|((CONFIG_SYS_CS1_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS1_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS1_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS1_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS1_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS1_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS1_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS1_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS1_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#define CONFIG_SYS_CS2U_VAL\
	(((((CONFIG_SYS_CS2_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS2_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS2_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS2_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|(((CONFIG_SYS_CS2_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|((CONFIG_SYS_CS2_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS2_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS2_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS2_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS2_SYNC_BURST_CLK_DIV&0x03)<<28))

#define CONFIG_SYS_CS2L_VAL\
	((CONFIG_SYS_CS2_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS2_PIN_ASSERT&0x01)<<1)\
	|((CONFIG_SYS_CS2_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS2_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS2_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS2_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS2_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS2_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS2_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS2_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS2_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#define CONFIG_SYS_CS3U_VAL\
	((((CONFIG_SYS_CS3_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS3_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS3_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS3_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|((((CONFIG_SYS_CS3_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|((CONFIG_SYS_CS3_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS3_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS3_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS3_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS3_SYNC_BURST_CLK_DIV&0x03)<<28))

#define CONFIG_SYS_CS3L_VAL\
	((CONFIG_SYS_CS3_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS3_PIN_ASSERT&0x01)<<1)\
	|((CONFIG_SYS_CS3_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS3_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS3_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS3_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS3_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS3_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS3_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS3_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS3_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#define CONFIG_SYS_CS4U_VAL\
	(((((CONFIG_SYS_CS4_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS4_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS4_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS4_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|((((CONFIG_SYS_CS4_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16))\
	|((CONFIG_SYS_CS4_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS4_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS4_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS4_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS4_SYNC_BURST_CLK_DIV&0x03)<<28))

#define CONFIG_SYS_CS4L_VAL\
	((CONFIG_SYS_CS4_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS4_PIN_ASSERT&0x01)<<1)\
	|((CONFIG_SYS_CS4_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS4_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS4_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS4_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS4_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS4_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS4_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS4_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS4_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#define CONFIG_SYS_CS5U_VAL\
	((((CONFIG_SYS_CS5_XTRA_DEAD_CYC+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)\
	|((((CONFIG_SYS_CS5_WRITE_XTRA_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<4)\
	|(((((CONFIG_SYS_CS5_READ_CYC_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)-1)&0x3F)<<8)\
	|((((CONFIG_SYS_CS5_CS_NEG_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x03)<<14)\
	|((((CONFIG_SYS_CS5_SYNC_DAT_OUT_LGTH+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|((CONFIG_SYS_CS5_SYNC_ENABLE&0x01)<<20)\
	|((CONFIG_SYS_CS5_SYNC_PAGE_MODE_EMUL&0x01)<<21)\
	|((CONFIG_SYS_CS5_SYNC_PAGE_SIZE&0x03)<<22)\
	|((((CONFIG_SYS_CS5_SYNC_BURST_CLK_START+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|((CONFIG_SYS_CS5_SYNC_BURST_CLK_DIV&0x03)<<28)\
	|((CONFIG_SYS_CS5_DTACK_SELECT&0x01)<<31))

#define CONFIG_SYS_CS5L_VAL\
	((CONFIG_SYS_CS5_CHIP_SELECT_ENABLE&0x01)\
	|((CONFIG_SYS_CS5_PIN_ASSERT&0x01)<<1)\
	|((CONFIG_SYS_CS5_WRITE_PROTECT&0x01)<<4)\
	|((CONFIG_SYS_CS5_SUPERVISOR_PROTECT&0x01)<<6)\
	|((CONFIG_SYS_CS5_DATA_PORT_SIZE&0x07)<<8)\
	|((CONFIG_SYS_CS5_EB_SIGNAL_CONTROL_WRITE&0x01)<<11)\
	|((((CONFIG_SYS_CS5_CS_ASSERT_NEG_DLY+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<12)\
	|(((((CONFIG_SYS_CS5_EB_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<16)\
	|(((((CONFIG_SYS_CS5_EB_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<20)\
	|(((((CONFIG_SYS_CS5_OE_NEG_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<24)\
	|(((((CONFIG_SYS_CS5_OE_ASSERT_DLY*2)+CONFIG_SYS_HCLK_LGTH-1)\
		/CONFIG_SYS_HCLK_LGTH)&0x0F)<<28))

#endif /* __CONFIG_H */

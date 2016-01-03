/*
 *
 * Configuration settings for the Armadeus Project motherboard PPS27
 *
 * Copyright (C) 2008-2012 ej / Armadeus Project <eric.jarrige@armadeus.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION 	"3.3"
#define CONFIG_IDENT_STRING	" pps patch 3.9"
#define CONFIG_BOARD_NAME	pps

/*
 * SoC configurations
 */
#define CONFIG_ARM926EJS		/* this is an ARM926EJS CPU */
#define CONFIG_MX27			/* in a Freescale i.MX27 Chip */
#define CONFIG_MACH_TYPE 	1698	/* APF27 */

/*
 * Enable the call to miscellaneous platform dependent initialization.
 */
#define CONFIG_SYS_NO_FLASH	/* to be define before <config_cmd_default.h> */
#define CONFIG_SYS_DCACHE_OFF
#define CONFIG_MISC_INIT_R

/*
 * Board display option
 */
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_DISPLAY_CPUINFO

/*
 * SPL
 */
#define CONFIG_NAND_U_BOOT
/* Copy SPL+U-Boot here	     */
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_LOAD_ADDR
/* Size is the partion size  */
#define CONFIG_SYS_NAND_U_BOOT_SIZE	CONFIG_SYS_MONITOR_LEN 

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
#define CONFIG_ROOTPATH		"/tftpboot/" MK_STR(CONFIG_BOARD_NAME) "-root"

/*
 * U-Boot Commands
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_ASKENV	/* ask for env variable		*/
#define CONFIG_CMD_BSP		/* Board Specific functions	*/
#define CONFIG_CMD_CACHE	/* icache, dcache		*/
#define CONFIG_CMD_DATE		/* support for RTC, date/time...*/
#define CONFIG_CMD_DHCP		/* DHCP Support			*/
#define CONFIG_CMD_DNS
#define CONFIG_CMD_EEPROM	/* EEPROM read/write support	*/
/*#define CONFIG_CMD_EXT2*/
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_IMX_FUSE	/* imx iim fuse                 */
#define CONFIG_CMD_MII		/* MII support			*/
/*#define CONFIG_CMD_MMC*/
#define CONFIG_CMD_MTDPARTS	/* MTD partition support 	*/
#define CONFIG_CMD_NAND		/* NAND support			*/
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_CMD_NAND_TRIMFFS
#define CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_PING		/* ping support			*/
#define CONFIG_CMD_SETEXPR	/* setexpr support		*/
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

/*
 * Memory configurations
 */
#define CONFIG_NR_DRAM_POPULATED	1
#define CONFIG_NR_DRAM_BANKS	2

#define CONFIG_SYS_SDRAM_MBYTE_SYZE 64

#define PHYS_SDRAM_1 			0xA0000000
#define PHYS_SDRAM_2 			0xB0000000
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (512<<10) )
#define CONFIG_SYS_GBL_DATA_SIZE	256		/* Initial data */
#define CONFIG_SYS_MEMTEST_START	0xA0000000	/* memtest test area  */
#define CONFIG_SYS_MEMTEST_END		0xA0300000	/* 3 MiB RAM test */

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE	\
		+ PHYS_SDRAM_1_SIZE - 0x0100000)

#define CONFIG_SYS_TEXT_BASE	0xA1000000

/*
 * FLASH organization
 */
/*#define	CONFIG_SYS_MONITOR_BASE	0xAFF00000*/
#define	CONFIG_SYS_MONITOR_OFFSET	0x00000000
#define	CONFIG_SYS_MONITOR_LEN		0x00100000	/* 1MiB */
#define CONFIG_ENV_IS_IN_NAND
#define	CONFIG_ENV_OVERWRITE
#define	CONFIG_ENV_OFFSET		0x00100000	/* NAND offset */
#define	CONFIG_ENV_SIZE			0x00020000	/* 128kB  */
#define CONFIG_ENV_RANGE		0X00080000	/* 512kB */
#define	CONFIG_ENV_OFFSET_REDUND	\
		(CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)	/* +512kB */
#define	CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE	/* 512kB */
#define	CONFIG_FIRMWARE_OFFSET		0x00200000
#define	CONFIG_KERNEL_OFFSET		0x00300000
#define	CONFIG_ROOTFS_OFFSET		0x00800000

#define CONFIG_MTDMAP			"mxc_nand.0"
#define MTDIDS_DEFAULT			"nand0=" CONFIG_MTDMAP
#define MTDPARTS_DEFAULT	"mtdparts=" CONFIG_MTDMAP \
				":1M(u-boot)ro," \
				"512K(env)," \
				"512K(env2)," \
				"512K(firmware)," \
				"512K(dtb)," \
				"5M(kernel)," \
				"-(rootfs)"

/*
 * U-Boot general configurations
 */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT		"BIOS> "	/* prompt string */
#define CONFIG_SYS_CBSIZE		2048		/* console I/O buffer */
#define CONFIG_SYS_PBSIZE 		\
				(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
						/* Print buffer size */
#define CONFIG_SYS_MAXARGS		16		/* max command args */
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
						/* Boot argument buffer size */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_HUSH_PARSER			/* enable the "hush" shell */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "	/* secondary prompt string */

/*
 * Boot Linux
 */
#define CONFIG_CMDLINE_TAG		/* send commandline to Kernel	*/
#define CONFIG_SETUP_MEMORY_TAGS 	/* send memory definition to kernel */
#define CONFIG_INITRD_TAG		/* send initrd params	*/

/* #define CONFIG_REVISION_TAG */
#define CONFIG_OF_LIBFDT

#define CONFIG_BOOTDELAY	1
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		MK_STR(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS 	"console=" MK_STR(ACFG_CONSOLE_DEV) "," \
				MK_STR(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
			" ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs "

#define ACFG_CONSOLE_DEV	ttySMX0
#define CONFIG_BOOTCOMMAND	"run ubifsboot"
#define CONFIG_SYS_AUTOLOAD	"no"
/*
 * Default load address for user programs and kernel
 */
#define CONFIG_LOADADDR			0xA0000000
#define	CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_TFTP_LOADADDR	CONFIG_LOADADDR

/*
 * Extra Environments
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"env_version="		CONFIG_ENV_VERSION			"\0" \
	"consoledev="		MK_STR(ACFG_CONSOLE_DEV)		"\0" \
	"mtdparts="		MTDPARTS_DEFAULT			"\0" \
	"partition=nand0,6\0"						\
	"u-boot_addr="		MK_STR(CONFIG_SYS_MONITOR_OFFSET)	"\0" \
	"env_addr="		MK_STR(CONFIG_ENV_OFFSET)		"\0" \
	"firmware_addr="	MK_STR(CONFIG_FIRMWARE_OFFSET)		"\0" \
	"kernel_addr="		MK_STR(CONFIG_KERNEL_OFFSET)		"\0" \
	"rootfs_addr="		MK_STR(CONFIG_ROOTFS_OFFSET)		"\0" \
	"board_name="		MK_STR(CONFIG_BOARD_NAME)		"\0" \
	"kernel_addr_r=A0000000\0" \
	"extrabootargs=otg_mode=device\0" 				\
	"addnfsargs=setenv bootargs ${bootargs} "			\
		"root=/dev/nfs rw nfsroot=${serverip}:${rootpath}\0"    \
	"addubifsargs=setenv bootargs ${bootargs} "			\
		"ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs\0"    \
	"addmmcargs=setenv bootargs ${bootargs} "			\
		"root=/dev/mmcblk0p1 rootfstype=ext2\0"   		\
	"addipargs=setenv bootargs ${bootargs} "			\
		"ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:"	\
		"${hostname}:eth0:off \0"				\
	"nfsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addnfsargs addipargs;"\
		"nfs ${kernel_addr_r} "					\
		"${serverip}:${rootpath}/boot/${board_name}-linux.bin;"	\
		"if test -n ${fdt_addr_r}  ; then "			\
			"nand read ${fdt_addr_r} dtb; fi;"		\
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0"		\
	"ubifsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs};run addubifsargs addipargs;"\
		"if test -n ${fdt_addr_r}  ; then "			\
			"nand read ${fdt_addr_r} dtb; fi;"		\
		"nboot ${kernel_addr_r} kernel;"			\
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0"		\
	"mmcboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addmmcargs addipargs;"\
		"if test -n ${fdt_addr_r}  ; then "			\
			"nand read ${fdt_addr_r} dtb; fi;"		\
		"mmc dev 0; ext2load mmc 0 ${kernel_addr_r}"		\
			" /boot/${board_name}-linux.bin;"		\
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0"		\
	"boot_nfs_fallback_flash=if ping ${serverip};"			\
			"then echo Boot over NFS; run nfsboot;"		\
			"else echo Boot from the board; run ubifsboot;"	\
		"fi\0"							\
	"firmware_autoload=0\0"						\
	"flash_uboot=nand unlock ${u-boot_addr} ;"			\
		"nand erase.part u-boot;"		\
		"if nand write.trimffs ${fileaddr} ${u-boot_addr} ${filesize};"\
			"then nand lock; nand unlock ${env_addr};"	\
				"echo Flashing of uboot succeed;"	\
			"else echo Flashing of uboot failed;"		\
		"fi; \0"						\
	"flash_firmware=nand erase.part firmware;"			\
		"if nand write.trimffs ${fileaddr} ${firmware_addr} ${filesize};"\
			"then echo Flashing of Firmware succeed;"	\
			"else echo Flashing of Firmware failed;"	\
		"fi\0"							\
	"flash_kernel=nand erase.part kernel;"				\
		"if nand write.trimffs ${fileaddr} ${kernel_addr} ${filesize};"\
			"then echo Flashing of kernel succeed;"		\
			"else echo Flashing of kernel failed;"		\
		"fi\0"							\
	"flash_rootfs=nand erase.part rootfs;"	\
		"if nand write.trimffs ${fileaddr} ${rootfs_addr} ${filesize};"\
			"then echo Flashing of rootfs succeed;"		\
			"else echo Flashing of rootfs failed;"		\
		"fi\0"							\
	"flash_dtb=nand erase.part dtb;"				\
		"if nand write.trimffs ${fileaddr} dtb ${filesize};"	\
			"then echo Flashing of dtb succeed;"		\
			"else echo Flashing of dtb failed;"		\
		"fi\0"							\
	"flash_reset_env=env default -f; saveenv;"			\
		"echo Flash environment variables erased!\0"		\
	"download_uboot=tftpboot ${loadaddr} ${board_name}-u-boot-nand.bin\0" \
	"download_kernel=tftpboot ${loadaddr} ${board_name}-linux.bin\0" \
	"download_rootfs=tftpboot ${loadaddr} ${board_name}-rootfs.ubi\0" \
	"download_dtb=tftpboot ${loadaddr} imx27-${board_name}.dtb\0" \
	"update_uboot=run download_uboot flash_uboot\0"			\
	"update_kernel=run download_kernel flash_kernel\0"		\
	"update_rootfs=run download_rootfs flash_rootfs\0"		\
	"update_dtb=run download_dtb flash_dtb\0"		\
	"update_all=run download_kernel flash_kernel download_rootfs "	\
		"flash_rootfs download_uboot flash_uboot\0"		\
	"unlock_regs=mw 10000008 0; mw 10020008 0\0"			\

/*
 * Serial Driver
 */
#define CONFIG_MXC_UART
#define CONFIG_SYS_MX27_UART1
#define CONFIG_CONS_INDEX		1
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_BAUDRATE 		115200
#define CONFIG_MXC_UART_BASE		UART1_BASE

/*
 * GPIO
 */

/*
 * NOR
 */

/*
 * NAND
 */
#define CONFIG_NAND_MXC

#define CONFIG_MXC_NAND_REGS_BASE	0xD8000000
#define CONFIG_SYS_NAND_BASE		CONFIG_MXC_NAND_REGS_BASE
#define CONFIG_SYS_MAX_NAND_DEVICE	1

#define CONFIG_MXC_NAND_HWECC
#define CONFIG_SYS_NAND_LARGEPAGE
#define IMX27_NAND_16BITS
#define NAND_MAX_CHIPS			1

#define CONFIG_SYS_DIRECT_FLASH_TFTP
#define CONFIG_FLASH_SHOW_PROGRESS	45
#define CONFIG_SYS_NAND_QUIET		1

#define CONFIG_BOOT_TRACE_REG	0xAFFFFFF8 /* Addr to store traces of SPL boot*/

/*
 * Partitions & Filsystems
 */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_DOS_PARTITION
#define CONFIG_SUPPORT_VFAT

/*
 * UBIFS
 */
#define CONFIG_RBTREE
#define CONFIG_LZO

/*
 * Ethernet (on SOC imx FEC)
 */
#define CONFIG_FEC_MXC

#define CONFIG_NET_MULTI
#define CONFIG_FEC_MXC_PHYADDR		0x1f
#define CONFIG_MII			1	/* MII PHY management	*/

/*
 * FPGA
 */
#define CONFIG_FPGA			CONFIG_SYS_SPARTAN3
#define CONFIG_FPGA_COUNT		1
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_SPARTAN3
#define CONFIG_SYS_FPGA_WAIT		20000 /* 20 ms */
#define CONFIG_SYS_FPGA_PROG_FEEDBACK
#define CONFIG_SYS_FPGA_CHECK_CTRLC
#define CONFIG_SYS_FPGA_CHECK_ERROR
//#define CONFIG_SYS_FPGA_IS_PROTO /* to be defined with apf27 board prototype*/

/* FPGA program pin configuration */
#define CONFIG_SYS_FPGA_PWR	(GPIO_PORTF | 19)	/* FPGA prog pin  */
#define CONFIG_SYS_FPGA_PRG	(GPIO_PORTF | 11)	/* FPGA prog pin  */
#define CONFIG_SYS_FPGA_CLK	(GPIO_PORTF | 15)	/* FPGA clk pin   */
#define CONFIG_SYS_FPGA_RDATA	0xD6000000		/* FPGA data addr  */
#define CONFIG_SYS_FPGA_WDATA	0xD6000000		/* FPGA data addr  */
#define CONFIG_SYS_FPGA_INIT	(GPIO_PORTF | 12)	/* FPGA init pin  */
#define CONFIG_SYS_FPGA_DONE	(GPIO_PORTF | 9)	/* FPGA done pin  */
#define CONFIG_SYS_FPGA_RW	(GPIO_PORTF | 21)	/* FPGA done pin  */
#define CONFIG_SYS_FPGA_CS	(GPIO_PORTF | 22)	/* FPGA done pin  */
#define CONFIG_SYS_FPGA_SUSPEND (GPIO_PORTF | 10)	/* FPGA done pin  */
#define CONFIG_SYS_FPGA_RESET	(GPIO_PORTF | 7)	/* FPGA done pin  */

/*
 * Fuses - IIM
 */
#ifdef CONFIG_CMD_IMX_FUSE
#define IIM_MAC_BANK		0
#define IIM_MAC_ROW		5
#define IIM0_SCC_KEY		11
#define IIM1_SUID		1
#endif

/*
 * I2C
 */

#ifdef CONFIG_CMD_I2C
#define	CONFIG_HARD_I2C			1
#define CONFIG_I2C_MXC
#define CONFIG_I2C_MULTI_BUS
#define CONFIG_SYS_I2C_BASE		IMX_I2C1_BASE
#define CONFIG_SYS_I2C_SPEED		100000	/* 100 kHz */
#define CONFIG_SYS_I2C_SLAVE		0x7F
#define CONFIG_I2C_CMD_TREE
#define CONFIG_SYS_I2C_NOPROBES 	{ }

#ifdef CONFIG_CMD_EEPROM
# define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* EEPROM 24LC02 */
# define CONFIG_SYS_I2C_EEPROM_ADDR_LEN 1	/* bytes of address */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10	/* msec */
#endif /* CONFIG_CMD_EEPROM */
#endif /* CONFIG_CMD_I2C */

/*
 * SPI
 */

/*
 * SD/MMC
 */
#ifdef CONFIG_CMD_MMC
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_MXC_MMC
#define CONFIG_MXC_MCI_REGS_BASE	0x10014000
#endif

/*
 * RTC
 */
#ifdef CONFIG_CMD_DATE
#define CONFIG_RTC_DS1374
#define CONFIG_SYS_RTC_BUS_NUM		0
#endif /* CONFIG_CMD_DATE */

/*
 * Watchdog Config
 */

/*
 * USB
 */

/*
 * Clocks
 */
#define	CONFIG_SYS_HZ			1000	/* Ticks per second */

/*
 * PLL
 *
 *  31 | x  |x| x x x x |x x x x x x x x x x |x x|x x x x|x x x x x x x x x x| 0
 *     |CPLM|X|----PD---|--------MFD---------|XXX|--MFI--|-----MFN-----------|
 */
#define CONFIG_MX27_CLK32		32768	/* 32768 or 32000 Hz crystal */
#undef  CONFIG_MX27_CLK26		/* No external 26MHz external clock */

/* MPU CLOCK source before PLL  */
/* CONFIG_SYS_CLK_FREQ (2/3 MPLL clock or ext 266 MHZ) */
#ifndef CONFIG_MX27_CLK26
#if (CONFIG_MX27_CLK32 == 32000)
#define CONFIG_SYS_MPCTL0_VAL		0x00211803	/* 398.998 MHz */
#define CONFIG_SYS_MPCTL1_VAL		0
#define CONFIG_MPLL_FREQ	399
#else /* CONFIG_MX27_CLK32 == 32768*/
#define CONFIG_SYS_MPCTL0_VAL		0x01EF15D5	/* 399.000 MHz */
#define CONFIG_SYS_MPCTL1_VAL		0
#define CONFIG_MPLL_FREQ	399
#endif /* CONFIG_MX27_CLK32 */
#else /* CONFIG_MX27_CLK26 in use*/
#define CONFIG_SYS_MPCTL0_VAL		0x00331C23	/* 399.000 MHz */
#define CONFIG_SYS_MPCTL1_VAL		0
#define CONFIG_MPLL_FREQ	399
#endif /* CONFIG_MX27_CLK26 */

#define CONFIG_SYS_CLK_FREQ	(CONFIG_MPLL_FREQ*2/3) /* 266 MHz */

/* Serial clock source before PLL (should be named CONFIG_SYS_SYSPLL_CLK_FREQ)*/
#ifndef CONFIG_MX27_CLK26
/*#define CONFIG_SYSPLL_CLK_FREQ	(1024*CONFIG_MX27_CLK32)*/
#if (CONFIG_MX27_CLK32 == 32000)
#define CONFIG_SYS_SPCTL0_VAL		0x04A02419	/* 300.00020 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0
#define CONFIG_SPLL_FREQ	300		/* MHz */
#else /* CONFIG_MX27_CLK32 == 32768*/
#define CONFIG_SYS_SPCTL0_VAL		0x0475206F	/* 299.99937 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0
#define CONFIG_SPLL_FREQ	300		/* MHz */
#endif /* CONFIG_MX27_CLK32 */
#else /* CONFIG_MX27_CLK26 in use*/
/*#define CONFIG_SYSPLL_CLK_FREQ	CONFIG_MX27_CLK26*/
#define CONFIG_SYS_SPCTL0_VAL		0x040C2C07	/* 300 MHz */
#define CONFIG_SYS_SPCTL1_VAL		0x0
#define CONFIG_SPLL_FREQ	300		/* MHz */
#endif /* CONFIG_MX27_CLK26 */

/* ARM bus frequency (have to be a CONFIG_MPLL_FREQ ratio) */
#define CONFIG_ARM_FREQ		399 	/* up to 400 MHz */

/* external bus frequency (have to be a CONFIG_SYS_CLK_FREQ ratio) */
#define CONFIG_HCLK_FREQ	133 	/* (CONFIG_SYS_CLK_FREQ/2) */

#define CONFIG_PERIF1_FREQ	16	/* 16.625 MHz UART, GPT, PWM*/
#define CONFIG_PERIF2_FREQ 	33	/* 33.25 MHz CSPI and SDHC*/
#define CONFIG_PERIF3_FREQ	33	/* 33.25 MHz LCD*/
#define CONFIG_PERIF4_FREQ	33	/* 33.25 MHz CSI*/
#define CONFIG_SSI1_FREQ	66	/* 66.50 MHz SSI1*/
#define CONFIG_SSI2_FREQ	66	/* 66.50 MHz SSI2*/
#define CONFIG_MSHC_FREQ	66	/* 66.50 MHz MSHC*/
#define CONFIG_H264_FREQ	66	/* 66.50 MHz H264*/
#define CONFIG_CLK0_DIV      3  /* Divide CLK0 by 4 */
#define CONFIG_CLK0_EN       1  /* CLK0 enabled */

/* external bus frequency (have to be a CONFIG_HCLK_FREQ ratio) */
#define CONFIG_NFC_FREQ		44	/* NFC Clock up to 44 MHz wh 133MHz*/

/* external serial bus frequency (have to be a CONFIG_SPLL_FREQ ratio) */
#define CONFIG_USB_FREQ		60	/* 60 MHz */

/*
 * SDRAM
 */
#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 64) /* micron MT46H16M32LF -6 */
/* micron 64MB */
#define PHYS_SDRAM_1_SIZE			0x04000000 /* 64 MB */
#define PHYS_SDRAM_2_SIZE			0x04000000 /* 64 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		9  /* 8, 9, 10 or 11 column address bits */
#define CONFIG_SYS_SDRAM_NUM_ROW		13 /* 11, 12 or 13 row address bits */
#define CONFIG_SYS_SDRAM_REFRESH		3  /* 0=OFF 1=2048 2=4096 3=8192 refresh */
#define CONFIG_SYS_SDRAM_EXIT_PWD		25 /* ns exit power down delay */
#define CONFIG_SYS_SDRAM_W2R_DELAY		1  /* write to read cycle delay > 0 */
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define CONFIG_SYS_SDRAM_TMRD_DELAY		2  /* Load mode register cycle delay 1..4 */
#define CONFIG_SYS_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck; SDRAM: 0=1ck 1=2ck*/
#define CONFIG_SYS_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define CONFIG_SYS_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define CONFIG_SYS_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define CONFIG_SYS_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC refresh to command)*/
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time estimated fo CL=1 0=force 3 for lpddr*/
#define CONFIG_SYS_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater 3=Eighth 4=Sixteenth */
#define CONFIG_SYS_SDRAM_DRIVE_STRENGH		0  /* 0=Full-strength 1=half 2=quater 3=Eighth */
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0  /* 1= single access; 0 = Burst mode */
#endif

#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 128)
/* micron 128MB */
#define PHYS_SDRAM_1_SIZE			0x08000000 /* 128 MB */
#define PHYS_SDRAM_2_SIZE			0x08000000 /* 128 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		9  /* 8, 9, 10 or 11 column address bits */
#define CONFIG_SYS_SDRAM_NUM_ROW		14 /* 11, 12 or 13 row address bits */
#define CONFIG_SYS_SDRAM_REFRESH		3  /* 0=OFF 1=2048 2=4096 3=8192 refresh */
#define CONFIG_SYS_SDRAM_EXIT_PWD		25 /* ns exit power down delay */
#define CONFIG_SYS_SDRAM_W2R_DELAY		1  /* write to read cycle delay > 0 */
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define CONFIG_SYS_SDRAM_TMRD_DELAY		2  /* Load mode register cycle delay 1..4 */
#define CONFIG_SYS_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck; SDRAM: 0=1ck 1=2ck*/
#define CONFIG_SYS_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define CONFIG_SYS_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define CONFIG_SYS_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define CONFIG_SYS_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC refresh to command)*/
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time estimated fo CL=1 0=force 3 for lpddr*/
#define CONFIG_SYS_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater 3=Eighth 4=Sixteenth */
#define CONFIG_SYS_SDRAM_DRIVE_STRENGH		0  /* 0=Full-strength 1=half 2=quater 3=Eighth */
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0  /* 1= single access; 0 = Burst mode */
#endif

#if (CONFIG_SYS_SDRAM_MBYTE_SYZE == 256)
/* micron 256MB */
#define PHYS_SDRAM_1_SIZE			0x10000000 /* 256 MB */
#define PHYS_SDRAM_2_SIZE			0x10000000 /* 256 MB */
#define CONFIG_SYS_SDRAM_NUM_COL		10  /* 8, 9, 10 or 11 column address bits */
#define CONFIG_SYS_SDRAM_NUM_ROW		14 /* 11, 12 or 13 row address bits */
#define CONFIG_SYS_SDRAM_REFRESH		3  /* 0=OFF 1=2048 2=4096 3=8192 refresh */
#define CONFIG_SYS_SDRAM_EXIT_PWD		25 /* ns exit power down delay */
#define CONFIG_SYS_SDRAM_W2R_DELAY		1  /* write to read cycle delay > 0 */
#define CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define CONFIG_SYS_SDRAM_TMRD_DELAY		2  /* Load mode register cycle delay 1..4 */
#define CONFIG_SYS_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck; SDRAM: 0=1ck 1=2ck*/
#define CONFIG_SYS_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define CONFIG_SYS_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define CONFIG_SYS_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define CONFIG_SYS_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC refresh to command)*/
#define CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time estimated fo CL=1 0=force 3 for lpddr*/
#define CONFIG_SYS_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater 3=Eighth 4=Sixteenth */
#define CONFIG_SYS_SDRAM_DRIVE_STRENGH		0  /* 0=Full-strength 1=half 2=quater 3=Eighth */
#define CONFIG_SYS_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define CONFIG_SYS_SDRAM_SINGLE_ACCESS		0  /* 1= single access; 0 = Burst mode */
#endif

/*
 * External interface
 */
/*
 * CSCRxU_VAL:
 * 31| x | x | x x |x x x x| x x | x | x  |x x x x|16
 *   |SP |WP | BCD |  BCS  | PSZ |PME|SYNC|  DOL  |
 *
 * 15| x x  | x x x x x x | x | x x x x | x x x x |0
 *   | CNC  |     WSC     |EW |   WWS   |   EDC   |
 *
 * CSCRxL_VAL:
 * 31|  x x x x  | x x x x  | x x x x  | x x x x  |16
 *   |    OEA    |   OEN    |   EBWA   |   EBWN   |
 * 15|x x x x| x |x x x |x x x x| x | x | x  | x  | 0
 *   |  CSA  |EBC| DSZ  |  CSN  |PSR|CRE|WRAP|CSEN|
 *
 * CSCRxA_VAL:
 * 31|  x x x x  | x x x x  | x x x x  | x x x x  |16
 *   |   EBRA    |   EBRN   |   RWA    |   RWN    |
 * 15| x | x x |x x x|x x|x x|x x| x | x | x  | x | 0
 *   |MUM| LAH | LBN |LBA|DWW|DCT|WWU|AGE|CNC2|FCE|
 */

/* CS0 configuration for 16 bit nor flash */
#define CONFIG_SYS_CS0U_VAL	0x0000CC03
#define CONFIG_SYS_CS0L_VAL	0xa0330D01
#define CONFIG_SYS_CS0A_VAL	0x00220800

#define CONFIG_SYS_CS1U_VAL	0x00000f00
#define CONFIG_SYS_CS1L_VAL	0x00000D01
#define CONFIG_SYS_CS1A_VAL	0

#define CONFIG_SYS_CS2U_VAL	0
#define CONFIG_SYS_CS2L_VAL	0
#define CONFIG_SYS_CS2A_VAL	0

#define CONFIG_SYS_CS3U_VAL	0
#define CONFIG_SYS_CS3L_VAL	0
#define CONFIG_SYS_CS3A_VAL	0

#define CONFIG_SYS_CS4U_VAL	0
#define CONFIG_SYS_CS4L_VAL	0
#define CONFIG_SYS_CS4A_VAL	0

/* FPGA 16 bit data bus */
#define CONFIG_SYS_CS5U_VAL	0x00000600
#define CONFIG_SYS_CS5L_VAL	0x00000D01
#define CONFIG_SYS_CS5A_VAL	0

#define CONFIG_SYS_EIM_VAL	0x00002200

/* FPGA specific settings */
/* CLKO */
#define CONFIG_SYS_CCSR_VAL 0x00000305
/* drive strength CLKO set to 2*/
#define CONFIG_SYS_DSCR10_VAL 0x00020000
/* drive strength A1..A12 set to 2*/
#define CONFIG_SYS_DSCR3_VAL 0x02AAAAA8
/* drive strength ctrl*/
#define CONFIG_SYS_DSCR7_VAL 0x00020880
/* drive strength data*/
#define CONFIG_SYS_DSCR2_VAL 0xAAAAAAAA


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
#define CONFIG_SYS_GIUS_A_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICR1_A_VAL		0x00000000
#define CONFIG_SYS_ICR2_A_VAL		0x00000000
#define CONFIG_SYS_IMR_A_VAL		0x00000000
#define CONFIG_SYS_GPR_A_VAL		0x00000000
#define CONFIG_SYS_PUEN_A_VAL		0xFFFFFFFF

#define CONFIG_SYS_DDIR_B_VAL		0x000001F0
#define CONFIG_SYS_OCR1_B_VAL		0x0003ff00
#define CONFIG_SYS_OCR2_B_VAL		0x00000000
#define CONFIG_SYS_ICFA1_B_VAL		0xFFFC00FF
#define CONFIG_SYS_ICFA2_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_B_VAL 		0xFFFC00FF
#define CONFIG_SYS_ICFB2_B_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_B_VAL		0x000001F0
#define CONFIG_SYS_GIUS_B_VAL		0xFF3FFFF0
#define CONFIG_SYS_ICR1_B_VAL		0x00000000
#define CONFIG_SYS_ICR2_B_VAL		0x00000000
#define CONFIG_SYS_IMR_B_VAL		0x00000000
#define CONFIG_SYS_GPR_B_VAL		0x00000000
#define CONFIG_SYS_PUEN_B_VAL		0xFFFFFE0F

#define CONFIG_SYS_DDIR_C_VAL		0x00000000
#define CONFIG_SYS_OCR1_C_VAL		0x00000000
#define CONFIG_SYS_OCR2_C_VAL		0x00000000
#define CONFIG_SYS_ICFA1_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_C_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_C_VAL		0x00000000
#define CONFIG_SYS_GIUS_C_VAL		0xFFFFC07F
#define CONFIG_SYS_ICR1_C_VAL		0x00000000
#define CONFIG_SYS_ICR2_C_VAL		0x00000000
#define CONFIG_SYS_IMR_C_VAL		0x00000000
#define CONFIG_SYS_GPR_C_VAL		0x00000000
#define CONFIG_SYS_PUEN_C_VAL		0xFFFFFF87

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

#define CONFIG_SYS_DDIR_E_VAL		0x00000000
#define CONFIG_SYS_OCR1_E_VAL		0x00000000
#define CONFIG_SYS_OCR2_E_VAL		0x00000000
#define CONFIG_SYS_ICFA1_E_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_E_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_E_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_E_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_E_VAL		0x00000000
#define CONFIG_SYS_GIUS_E_VAL		0xFCFFCCF8
#define CONFIG_SYS_ICR1_E_VAL		0x00000000
#define CONFIG_SYS_ICR2_E_VAL		0x00000000
#define CONFIG_SYS_IMR_E_VAL		0x00000000
#define CONFIG_SYS_GPR_E_VAL		0x00000000
#define CONFIG_SYS_PUEN_E_VAL		0xFFFFFFFF

#define CONFIG_SYS_DDIR_F_VAL		0x00000000
#define CONFIG_SYS_OCR1_F_VAL		0x00000000
#define CONFIG_SYS_OCR2_F_VAL		0x00000000
#define CONFIG_SYS_ICFA1_F_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFA2_F_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB1_F_VAL		0xFFFFFFFF
#define CONFIG_SYS_ICFB2_F_VAL		0xFFFFFFFF
#define CONFIG_SYS_DR_F_VAL		0x00000000
#define CONFIG_SYS_GIUS_F_VAL		0xFF7F8000
#define CONFIG_SYS_ICR1_F_VAL		0x00000000
#define CONFIG_SYS_ICR2_F_VAL		0x00000000
#define CONFIG_SYS_IMR_F_VAL		0x00000000
#define CONFIG_SYS_GPR_F_VAL		0x00000000
#define CONFIG_SYS_PUEN_F_VAL		0xFFFFFFFF

/* Enforce DDR signal strengh & enable USB/PP/DMA burst override bits */
#define CONFIG_SYS_GPCR_VAL		0x0003000F

#define CONFIG_SYS_ESDMISC_VAL		ESDMISC_LHD+ESDMISC_MDDREN

/* FMCR select num LPDDR RAMs and nand 16bits, 2KB pages */
#if (CONFIG_NR_DRAM_BANKS == 1)
#define CONFIG_SYS_FMCR_VAL 0xFFFFFFF9
#elif (CONFIG_NR_DRAM_BANKS == 2)
#define CONFIG_SYS_FMCR_VAL 0xFFFFFFFB
#endif

#define CONFIG_SYS_AIPI1_PSR0_VAL	0x20040304
#define CONFIG_SYS_AIPI1_PSR1_VAL	0xDFFBFCFB
#define CONFIG_SYS_AIPI2_PSR0_VAL	0x00000000
#define CONFIG_SYS_AIPI2_PSR1_VAL	0xFFFFFFFF

/* PCCR enable DMA FEC I2C1 IIM SDHC1 */
#define CONFIG_SYS_PCCR0_VAL		0x05070410
#define CONFIG_SYS_PCCR1_VAL		0xA14A0608

/*
 * From here, there should not be any user configuration.
 * All Equations are automatic
 */

/* fixme none integer value (7.5ns) => 2*hclock = 15ns */
#define CONFIG_SYS_2XHCLK_LGTH		(2000/CONFIG_HCLK_FREQ)	/* ns */

/* USB 60 MHz ; ARM up to 400; HClK up to 133MHz*/
#ifdef CONFIG_MX27_CLK26
#define CSCR_MASK 0x0303800D
#else
#define CSCR_MASK 0x0300800D
#endif
#define CONFIG_SYS_CSCR_VAL\
	(CSCR_MASK 						\
	|((((CONFIG_SPLL_FREQ/CONFIG_USB_FREQ)-1)&0x07)<<28)	\
	|((((CONFIG_MPLL_FREQ/CONFIG_ARM_FREQ)-1)&0x03)<<12)	\
	|((((CONFIG_SYS_CLK_FREQ/CONFIG_HCLK_FREQ)-1)&0x03)<<8))

/* SSIx CLKO NFC H264 MSHC */
#define CONFIG_SYS_PCDR0_VAL\
	(((((CONFIG_SYS_CLK_FREQ/CONFIG_MSHC_FREQ)-1)&0x3F)<<0)	\
	|((((CONFIG_HCLK_FREQ/CONFIG_NFC_FREQ)-1)&0x0F)<<6)	\
	|(((((CONFIG_SYS_CLK_FREQ/CONFIG_H264_FREQ)-2)*2)&0x3F)<<10)\
	|(((((CONFIG_SYS_CLK_FREQ/CONFIG_SSI1_FREQ)-2)*2)&0x3F)<<16)\
	|(((CONFIG_CLK0_DIV)&0x07)<<22)\
	|(((CONFIG_CLK0_EN)&0x01)<<25)\
	|(((((CONFIG_SYS_CLK_FREQ/CONFIG_SSI2_FREQ)-2)*2)&0x3F)<<26))

/* PERCLKx  */
#define CONFIG_SYS_PCDR1_VAL\
	(((((CONFIG_SYS_CLK_FREQ/CONFIG_PERIF1_FREQ)-1)&0x3F)<<0)	\
	|((((CONFIG_SYS_CLK_FREQ/CONFIG_PERIF2_FREQ)-1)&0x3F)<<8)	\
	|((((CONFIG_SYS_CLK_FREQ/CONFIG_PERIF3_FREQ)-1)&0x3F)<<16)	\
	|((((CONFIG_SYS_CLK_FREQ/CONFIG_PERIF4_FREQ)-1)&0x3F)<<24))

/* SDRAM controller programming Values */
#if (((2*CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1)>(3*CONFIG_SYS_2XHCLK_LGTH))\
	||(CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1<1))
#define REG_FIELD_SCL_VAL 3
#define REG_FIELD_SCLIMX_VAL 0
#else
#define REG_FIELD_SCL_VAL\
	((2*CONFIG_SYS_SDRAM_CLOCK_CYCLE_CL_1+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)
#define REG_FIELD_SCLIMX_VAL REG_FIELD_SCL_VAL
#endif

//#define REG_FIELD_SCL_VAL 3
//#define REG_FIELD_SCLIMX_VAL 3

#if ((2*CONFIG_SYS_SDRAM_RC_DELAY)>(16*CONFIG_SYS_2XHCLK_LGTH))
#define REG_FIELD_SRC_VAL 0
#else
#define REG_FIELD_SRC_VAL\
	((2*CONFIG_SYS_SDRAM_RC_DELAY+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)
#endif

/* TBD Power down timer ; PRCT Bit Field Encoding; burst length 8 ; FP = 0*/
#define REG_ESDCTL_BASE_CONFIG (0x80020485\
				| (((CONFIG_SYS_SDRAM_NUM_ROW-11)&0x7)<<24)\
				| (((CONFIG_SYS_SDRAM_NUM_COL-8)&0x3)<<20)\
				| (((CONFIG_SYS_SDRAM_REFRESH)&0x7)<<13))

#define CONFIG_SYS_NORMAL_RW_CMD	((0x0<<28)+REG_ESDCTL_BASE_CONFIG)
#define CONFIG_SYS_PRECHARGE_CMD	((0x1<<28)+REG_ESDCTL_BASE_CONFIG)
#define CONFIG_SYS_AUTOREFRESH_CMD	((0x2<<28)+REG_ESDCTL_BASE_CONFIG)
#define CONFIG_SYS_SET_MODE_REG_CMD	((0x3<<28)+REG_ESDCTL_BASE_CONFIG)

/* ESDRAMC Configuration Registers : force CL=3 to lpddr */
#define CONFIG_SYS_SDRAM_ESDCFG_REGISTER_VAL (0x0\
	| (((((2*CONFIG_SYS_SDRAM_EXIT_PWD+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)-1)&0x3)<<21)\
	| (((CONFIG_SYS_SDRAM_W2R_DELAY-1)&0x1)<<20)\
	| (((((2*CONFIG_SYS_SDRAM_ROW_PRECHARGE_DELAY+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)-1)&0x3)<<18)\
	| (((CONFIG_SYS_SDRAM_TMRD_DELAY-1)&0x3)<<16)\
	| (((CONFIG_SYS_SDRAM_TWR_DELAY)&0x1)<<15)\
	| (((((2*CONFIG_SYS_SDRAM_RAS_DELAY+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)-1)&0x7)<<12)\
	| (((((2*CONFIG_SYS_SDRAM_RRD_DELAY+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)-1)&0x3)<<10)\
	| (((REG_FIELD_SCLIMX_VAL)&0x3)<<8)\
	| (((((2*CONFIG_SYS_SDRAM_RCD_DELAY+CONFIG_SYS_2XHCLK_LGTH-1)/CONFIG_SYS_2XHCLK_LGTH)-1)&0x7)<<4)\
	| (((REG_FIELD_SRC_VAL)&0x0F)<<0))

/* Issue Mode register Command to SDRAM*/
#define CONFIG_SYS_SDRAM_MODE_REGISTER_VAL\
	((((CONFIG_SYS_SDRAM_BURST_LENGTH)&0x7)<<(0))\
	| (((REG_FIELD_SCL_VAL)&0x7)<<(4))\
	| ((0)<<(3)) /* sequentiql access */ \
	/*| (((CONFIG_SYS_SDRAM_SINGLE_ACCESS)&0x1)<<(1))*/)

/* Issue Extended Mode register Command to SDRAM*/
#define CONFIG_SYS_SDRAM_EXT_MODE_REGISTER_VAL\
	((CONFIG_SYS_SDRAM_PARTIAL_ARRAY_SR<<0)\
	| (CONFIG_SYS_SDRAM_DRIVE_STRENGH<<(5))\
	| (1<<(CONFIG_SYS_SDRAM_NUM_COL+CONFIG_SYS_SDRAM_NUM_ROW+1+2)))

/* Issue Precharge all Command to SDRAM*/
#define CONFIG_SYS_SDRAM_PRECHARGE_ALL_VAL (1<<10)

#endif /* __CONFIG_H */

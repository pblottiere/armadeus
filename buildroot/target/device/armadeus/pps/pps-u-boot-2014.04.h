/*
 *
 * Configuration settings for the Armadeus Project motherboard PPS27
 *
 * Copyright (C) 2008-2014 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION 	12
#define CONFIG_IDENT_STRING	" pps patch 3.20"
#define CONFIG_BOARD_NAME	pps
#define CONFIG_SYS_GENERIC_BOARD
#define __APF27_H	/* force overide apf27.h defines */

/*
 * SoC configurations
 */
#define CONFIG_ARM926EJS		/* this is an ARM926EJS CPU */
#define CONFIG_MX27			/* in a Freescale i.MX27 Chip */
#define CONFIG_MACH_TYPE	1698	/* APF27 */

/*
 * Enable the call to miscellaneous platform dependent initialization.
 */
#define CONFIG_SYS_NO_FLASH	/* to be define before <config_cmd_default.h> */

/*
 * Board display option
 */
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_DISPLAY_CPUINFO

/*
 * SPL
 */
/* Copy SPL+U-Boot here	     */
#define CONFIG_SPL
#define CONFIG_SPL_TARGET	"u-boot-with-spl.bin"
#define CONFIG_SPL_LDSCRIPT	"arch/$(ARCH)/cpu/u-boot-spl.lds"
#define CONFIG_SPL_MAX_SIZE	2048
#define CONFIG_SPL_TEXT_BASE    0xA0000000

/* NAND boot config */
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SYS_NAND_U_BOOT_START    CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_OFFS	0x800
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_SIZE	CONFIG_SYS_MONITOR_LEN - 0x800

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

#define CONFIG_HOSTNAME	CONFIG_BOARD_NAME
#define CONFIG_ROOTPATH	"/tftpboot/" __stringify(CONFIG_BOARD_NAME) "-root"

/*
 * U-Boot Commands
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_ASKENV	/* ask for env variable		*/
#define CONFIG_CMD_BSP		/* Board Specific functions	*/
#define CONFIG_CMD_CACHE	/* icache, dcache		*/
#define CONFIG_CMD_DATE
#define CONFIG_CMD_DHCP		/* DHCP Support			*/
#define CONFIG_CMD_DNS
#define CONFIG_CMD_EEPROM
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_IMX_FUSE	/* imx iim fuse                 */
#define CONFIG_CMD_I2C
#define CONFIG_CMD_MII		/* MII support			*/
/*#define CONFIG_CMD_MMC*/
#define CONFIG_CMD_MTDPARTS	/* MTD partition support	*/
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
#define CONFIG_NR_DRAM_POPULATED 1
#define CONFIG_NR_DRAM_BANKS	2

#define ACFG_SDRAM_MBYTE_SYZE 64

#define PHYS_SDRAM_1			0xA0000000
#define PHYS_SDRAM_2			0xB0000000
#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (2 * 1024 * 1024))
#define CONFIG_SYS_MEMTEST_START	0xA0000000	/* memtest test area  */
#define CONFIG_SYS_MEMTEST_END		0xA0300000	/* 3 MiB RAM test */

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE	\
		+ PHYS_SDRAM_1_SIZE - 0x0100000)

#define CONFIG_SYS_TEXT_BASE		0xA0000800

/*
 * FLASH organization
 */
#define	ACFG_MONITOR_OFFSET		0x00000000
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
#define	CONFIG_FIRMWARE_SIZE		0x00080000	/* 512kB  */
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
					"check_env ;"


/*
 * Boot Linux
 */
#define CONFIG_CMDLINE_TAG		/* send commandline to Kernel	*/
#define CONFIG_SETUP_MEMORY_TAGS	/* send memory definition to kernel */
#define CONFIG_INITRD_TAG		/* send initrd params	*/

#define CONFIG_OF_LIBFDT

#define CONFIG_BOOTDELAY	1
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		__stringify(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," \
			__stringify(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
			" ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs "

#define ACFG_CONSOLE_DEV	ttySMX0
#define CONFIG_BOOTCOMMAND	"run ubifsboot"
#define CONFIG_SYS_AUTOLOAD	"no"
/*
 * Default load address for user programs and kernel
 */
#define CONFIG_LOADADDR			0xA0000000
#define	CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/*
 * Extra Environments
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"env_version="		__stringify(CONFIG_ENV_VERSION)		"\0" \
	"consoledev="		__stringify(ACFG_CONSOLE_DEV)		"\0" \
	"mtdparts="		MTDPARTS_DEFAULT			"\0" \
	"partition=nand0,6\0"						\
	"u-boot_addr="		__stringify(ACFG_MONITOR_OFFSET)	"\0" \
	"env_addr="		__stringify(CONFIG_ENV_OFFSET)		"\0" \
	"firmware_addr="	__stringify(CONFIG_FIRMWARE_OFFSET)	"\0" \
	"firmware_size="	__stringify(CONFIG_FIRMWARE_SIZE)	"\0" \
	"kernel_addr="		__stringify(CONFIG_KERNEL_OFFSET)	"\0" \
	"rootfs_addr="		__stringify(CONFIG_ROOTFS_OFFSET)	"\0" \
	"board_name="		__stringify(CONFIG_BOARD_NAME)		"\0" \
	"kernel_addr_r=A0000000\0" \
	"extrabootargs=otg_mode=device\0" 				\
	"check_env=if test -n ${flash_env_version}; "			\
		"then env default env_version; "			\
		"else env set flash_env_version ${env_version}; env save; "\
		"fi; "							\
		"if itest ${flash_env_version} < ${env_version}; then " \
			"echo \"*** Warning - Environment version"	\
			" change suggests: run flash_reset_env; reset\"; "\
			"env default flash_reset_env; "			\
		"else exit; fi; \0"					\
	"check_flash=nand lock; nand unlock ${env_addr}; \0"		\
	"flash_reset_env=env default -f -a && saveenv &&"		\
		"echo Flash environment variables erased!\0"		\
	"download_uboot=tftpboot ${loadaddr} ${board_name}"		\
		"-u-boot-with-spl.bin\0"				\
	"flash_uboot=nand unlock ${u-boot_addr} ;"			\
		"nand erase.part u-boot;"		\
		"if nand write.trimffs ${fileaddr} ${u-boot_addr} ${filesize};"\
			"then nand lock; nand unlock ${env_addr};"	\
				"echo Flashing of uboot succeed;"	\
			"else echo Flashing of uboot failed;"		\
		"fi; \0"						\
	"update_uboot=run download_uboot flash_uboot\0"			\
	"download_env=tftpboot ${loadaddr} ${board_name}"		\
		"-u-boot-env.txt\0"					\
	"flash_env=env import -t ${loadaddr} && env save; \0"		\
	"update_env=run download_env flash_env\0"			\
	"download_firmware=tftpboot ${loadaddr} ${board_name}-firmware.bin\0"\
	"flash_firmware=nand erase.part firmware;"			\
		"if nand write.trimffs ${fileaddr} ${firmware_addr} "	\
			"${filesize};"					\
		"then echo Flashing of Firmware succeed;"		\
		"else echo Flashing of Firmware failed;"		\
		"fi\0"							\
	"update_firmware=run download_firmware flash_firmware\0"	\
	"load_firmware=nand read ${loadaddr} firmware && "		\
		"fpga loadb 0 ${loadaddr} ${firmware_size} || "		\
		"fpga load 0 ${loadaddr} ${firmware_size}; "		\
		"if test $? -eq 0; "					\
		"then echo Firmware successfully loaded;"		\
		"else echo Loading firmware failed!;"			\
		"fi;\0"							\
	"checknload_firmware=if test -n ${firmware_autoload};"		\
				"then run load_firmware; else exit; fi;\0"\
	"download_dtb=tftpboot ${loadaddr} ${board_name}.dtb\0"		\
	"flash_dtb=nand erase.part dtb;"				\
		"if nand write.trimffs ${fileaddr} dtb ${filesize};"	\
		"then echo Flashing dtb succeed;"			\
		"else echo Flashing dtb failed;"			\
		"fi;\0"							\
	"update_dtb=run download_dtb flash_dtb\0"			\
	"download_kernel=tftpboot ${loadaddr} ${board_name}-linux.bin\0"\
	"flash_kernel=nand erase.part kernel;"				\
		"if nand write.trimffs ${fileaddr} ${kernel_addr} "	\
			"${filesize};"					\
		"then echo Flashing of kernel succeed;"			\
		"else echo Flashing of kernel failed;"			\
		"fi\0"							\
	"update_kernel=run download_kernel flash_kernel\0"		\
	"download_rootfs=tftpboot ${loadaddr} ${board_name}-rootfs.ubi\0"\
	"flash_rootfs=nand erase.part rootfs;"				\
		"if nand write.trimffs ${fileaddr} ${rootfs_addr} "	\
			"${filesize};"					\
		"then echo Flashing of rootfs succeed;"			\
		"else echo Flashing of rootfs failed;"			\
		"fi\0"							\
	"update_rootfs=run download_rootfs flash_rootfs\0"		\
	"update_all=run update_kernel update_rootfs update_uboot &&"	\
		"if test -n ${fdt_addr_r};"				\
		"then run update_dtb; else exit; fi &&"			\
		"if test -n ${firmware_autoload};"			\
		"then run update_firmware; else exit; fi;\0"		\
	"addipargs=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:"\
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0"	\
	"addmmcargs=setenv bootargs ${bootargs} root=/dev/mmcblk0p1 "	\
		"rootfstype=ext2\0"					\
	"mmcboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addmmcargs addipargs;"\
		"mmc dev 0 &&"						\
		"if test -n ${fdt_addr_r} ;"				\
		"then ext2load mmc 0 ${fdt_addr_r} "			\
			"/boot/${board_name}.dtb;"			\
		"else echo no fdt_addr_r;"				\
		"fi &&"							\
		"ext2load mmc 0 ${kernel_addr_r} "			\
			"/boot/${board_name}-linux.bin &&"		\
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0"		\
	"addnfsargs=setenv bootargs ${bootargs} root=/dev/nfs rw "	\
		"nfsroot=${serverip}:${rootpath}\0"			\
	"nfsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs}; run addnfsargs addipargs;"\
		"nfs ${kernel_addr_r} ${serverip}:${rootpath}/boot/"	\
			"${board_name}-linux.bin &&"			\
		"if test -n ${fdt_addr_r};"				\
		"then nfs ${fdt_addr_r} ${serverip}:${rootpath}/boot/"	\
			"${board_name}.dtb;"				\
		"else echo no fdt_addr_r;"				\
		"fi &&"							\
		"bootm ${kernel_addr_r} - ${fdt_addr_r}\0"		\
	"addubifsargs=setenv bootargs ${bootargs} ubi.mtd=rootfs "	\
		"root=ubi0:rootfs rootfstype=ubifs\0"			\
	"ubifsboot=setenv bootargs console=${consoledev},${baudrate} "	\
		"${mtdparts} ${extrabootargs};run addubifsargs addipargs;"\
		"if test -n ${fdt_addr_r};"				\
		"then nand read ${fdt_addr_r} dtb;"			\
		"else echo no fdt_addr_r;"				\
		"fi &&"							\
		"nboot ${kernel_addr_r} kernel;bootm ${kernel_addr_r} - "\
			"${fdt_addr_r}\0"				\
	"unlock_regs=mw 10000008 0; mw 10020008 0\0"			\

/*
 * Serial Driver
 */
#define CONFIG_MXC_UART
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200
#define CONFIG_MXC_UART_BASE		UART1_BASE

/*
 * GPIO
 */
#define CONFIG_MXC_GPIO

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
#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define CONFIG_SYS_NAND_BUSWIDTH_16BIT
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)
#define CONFIG_SYS_NAND_PAGE_COUNT	CONFIG_SYS_NAND_BLOCK_SIZE / \
						CONFIG_SYS_NAND_PAGE_SIZE
#define CONFIG_SYS_NAND_SIZE		(256 * 1024 * 1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	11
#define NAND_MAX_CHIPS			1


#define CONFIG_SYS_DIRECT_FLASH_TFTP
#define CONFIG_FLASH_SHOW_PROGRESS	45
#define CONFIG_SYS_NAND_QUIET		1

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
/*#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC*/
#define CONFIG_FEC_MXC_PHYADDR		0x1f
#define CONFIG_MII				/* MII PHY management	*/

/*
 * FPGA
 */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_FPGA
#endif
#define CONFIG_FPGA_COUNT		1
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_SPARTAN3
#define CONFIG_SYS_FPGA_WAIT		250 /* 250 ms */
#define CONFIG_SYS_FPGA_PROG_FEEDBACK
#define CONFIG_SYS_FPGA_CHECK_CTRLC
#define CONFIG_SYS_FPGA_CHECK_ERROR

/* FPGA program pin configuration */
#define ACFG_FPGA_PWR	(GPIO_PORTF | 19)	/* FPGA prog pin  */
#define ACFG_FPGA_PRG	(GPIO_PORTF | 11)	/* FPGA prog pin  */
#define ACFG_FPGA_CLK	(GPIO_PORTF | 15)	/* FPGA clk pin   */
#define ACFG_FPGA_RDATA	0xD6000000		/* FPGA data addr */
#define ACFG_FPGA_WDATA	0xD6000000		/* FPGA data addr */
#define ACFG_FPGA_INIT	(GPIO_PORTF | 12)	/* FPGA init pin  */
#define ACFG_FPGA_DONE	(GPIO_PORTF | 9)	/* FPGA done pin  */
#define ACFG_FPGA_RW	(GPIO_PORTF | 21)	/* FPGA done pin  */
#define ACFG_FPGA_CS	(GPIO_PORTF | 22)	/* FPGA done pin  */
#define ACFG_FPGA_SUSPEND (GPIO_PORTF | 10)	/* FPGA done pin  */
#define ACFG_FPGA_RESET	(GPIO_PORTF | 7)	/* FPGA done pin  */

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
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_MXC_I2C1_SPEED	100000	/* 100 kHz */
#define CONFIG_SYS_MXC_I2C1_SLAVE	0x7F
#define CONFIG_SYS_MXC_I2C2_SPEED	100000	/* 100 kHz */
#define CONFIG_SYS_MXC_I2C2_SLAVE	0x7F
#define CONFIG_SYS_I2C_NOPROBES		{ }

#ifdef CONFIG_CMD_EEPROM
# define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* EEPROM 24LC02 */
# define CONFIG_SYS_I2C_EEPROM_ADDR_LEN 1	/* bytes of address */
#define CONFIG_SYS_EEPROM_PAGE_WRITE_BITS	3
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS	10	/* msec */
#endif /* CONFIG_CMD_EEPROM */
#endif /* CONFIG_CMD_I2C */

/*
 * SD/MMC
 */
#ifdef CONFIG_CMD_MMC
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_MXC_MMC
#define CONFIG_MXC_MCI_REGS_BASE	0x10014000
#define PC_PWRON	(GPIO_PORTF | 16)
#endif

/*
 * RTC
 */
#ifdef CONFIG_CMD_DATE
#define CONFIG_RTC_DS1374
#define CONFIG_SYS_RTC_BUS_NUM		0
#endif /* CONFIG_CMD_DATE */

/*
 * PLL
 *
 *  31 | x  |x| x x x x |x x x x x x x x x x |x x|x x x x|x x x x x x x x x x| 0
 *     |CPLM|X|----PD---|--------MFD---------|XXX|--MFI--|-----MFN-----------|
 */
#define CONFIG_MX27_CLK32		32768	/* 32768 or 32000 Hz crystal */

/*
 * MPU CLOCK source before PLL
 * ACFG_CLK_FREQ (2/3 MPLL clock or ext 266 MHZ)
 */
#define ACFG_MPCTL0_VAL		0x01EF15D5	/* 399.000 MHz */
#define ACFG_MPCTL1_VAL		0
#define CONFIG_MPLL_FREQ	399

#define ACFG_CLK_FREQ	(CONFIG_MPLL_FREQ*2/3) /* 266 MHz */

/* Serial clock source before PLL (should be named ACFG_SYSPLL_CLK_FREQ)*/
#define ACFG_SPCTL0_VAL		0x0475206F	/* 299.99937 MHz */
#define ACFG_SPCTL1_VAL		0
#define CONFIG_SPLL_FREQ	300		/* MHz */

/* ARM bus frequency (have to be a CONFIG_MPLL_FREQ ratio) */
#define CONFIG_ARM_FREQ		399	/* up to 400 MHz */

/* external bus frequency (have to be a ACFG_CLK_FREQ ratio) */
#define CONFIG_HCLK_FREQ	133	/* (ACFG_CLK_FREQ/2) */

#define CONFIG_PERIF1_FREQ	16	/* 16.625 MHz UART, GPT, PWM */
#define CONFIG_PERIF2_FREQ	33	/* 33.25 MHz CSPI and SDHC */
#define CONFIG_PERIF3_FREQ	33	/* 33.25 MHz LCD */
#define CONFIG_PERIF4_FREQ	33	/* 33.25 MHz CSI */
#define CONFIG_SSI1_FREQ	66	/* 66.50 MHz SSI1 */
#define CONFIG_SSI2_FREQ	66	/* 66.50 MHz SSI2 */
#define CONFIG_MSHC_FREQ	66	/* 66.50 MHz MSHC */
#define CONFIG_H264_FREQ	66	/* 66.50 MHz H264 */
#define CONFIG_CLK0_DIV		3	/* Divide CLK0 by 4 */
#define CONFIG_CLK0_EN		1	/* CLK0 enabled */

/* external bus frequency (have to be a CONFIG_HCLK_FREQ ratio) */
#define CONFIG_NFC_FREQ		44	/* NFC Clock up to 44 MHz wh 133MHz */

/* external serial bus frequency (have to be a CONFIG_SPLL_FREQ ratio) */
#define CONFIG_USB_FREQ		60	/* 60 MHz */

/*
 * SDRAM
 */
#if (ACFG_SDRAM_MBYTE_SYZE == 64) /* micron MT46H16M32LF -6 */
/* micron 64MB */
#define PHYS_SDRAM_1_SIZE		0x04000000 /* 64 MB */
#define PHYS_SDRAM_2_SIZE		0x04000000 /* 64 MB */
#define ACFG_SDRAM_NUM_COL		9  /* 8, 9, 10 or 11
					    * column address bits
					    */
#define ACFG_SDRAM_NUM_ROW		13 /* 11, 12 or 13
					    * row address bits
					    */
#define ACFG_SDRAM_REFRESH		3  /* 0=OFF 1=2048
					    * 2=4096 3=8192 refresh
					    */
#define ACFG_SDRAM_EXIT_PWD		25 /* ns exit power
					    * down delay
					    */
#define ACFG_SDRAM_W2R_DELAY		1  /* write to read
					    * cycle delay > 0
					    */
#define ACFG_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define ACFG_SDRAM_TMRD_DELAY		2  /* Load mode register
					    * cycle delay 1..4
					    */
#define ACFG_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck
					    * SDRAM: 0=1ck 1=2ck
					    */
#define ACFG_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define ACFG_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define ACFG_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define ACFG_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC
					    * refresh to command)
					    */
#define ACFG_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time
					   * estimated fo CL=1
					   * 0=force 3 for lpddr
					   */
#define ACFG_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater
					    * 3=Eighth 4=Sixteenth
					    */
#define ACFG_SDRAM_DRIVE_STRENGH	0  /* 0=Full-strength 1=half
					    * 2=quater 3=Eighth
					    */
#define ACFG_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define ACFG_SDRAM_SINGLE_ACCESS	0  /* 1= single access
					    * 0 = Burst mode
					    */
#endif

#if (ACFG_SDRAM_MBYTE_SYZE == 128)
/* micron 128MB */
#define PHYS_SDRAM_1_SIZE		0x08000000 /* 128 MB */
#define PHYS_SDRAM_2_SIZE		0x08000000 /* 128 MB */
#define ACFG_SDRAM_NUM_COL		9  /* 8, 9, 10 or 11
					    * column address bits
					    */
#define ACFG_SDRAM_NUM_ROW		14 /* 11, 12 or 13
					    * row address bits
					    */
#define ACFG_SDRAM_REFRESH		3  /* 0=OFF 1=2048
					    * 2=4096 3=8192 refresh
					    */
#define ACFG_SDRAM_EXIT_PWD		25 /* ns exit power
					    * down delay
					    */
#define ACFG_SDRAM_W2R_DELAY		1  /* write to read
					    * cycle delay > 0
					    */
#define ACFG_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define ACFG_SDRAM_TMRD_DELAY		2  /* Load mode register
					    * cycle delay 1..4
					    */
#define ACFG_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck
					    * SDRAM: 0=1ck 1=2ck
					    */
#define ACFG_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define ACFG_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define ACFG_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define ACFG_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC
					    * refresh to command)
					    */
#define ACFG_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time
					   * estimated fo CL=1
					   * 0=force 3 for lpddr
					   */
#define ACFG_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater
					    * 3=Eighth 4=Sixteenth
					    */
#define ACFG_SDRAM_DRIVE_STRENGH	0  /* 0=Full-strength 1=half
					    * 2=quater 3=Eighth
					    */
#define ACFG_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define ACFG_SDRAM_SINGLE_ACCESS	0  /* 1= single access
					    * 0 = Burst mode
					    */
#endif

#if (ACFG_SDRAM_MBYTE_SYZE == 256)
/* micron 256MB */
#define PHYS_SDRAM_1_SIZE		0x10000000 /* 256 MB */
#define PHYS_SDRAM_2_SIZE		0x10000000 /* 256 MB */
#define ACFG_SDRAM_NUM_COL		10  /* 8, 9, 10 or 11
					     * column address bits
					     */
#define ACFG_SDRAM_NUM_ROW		14 /* 11, 12 or 13
					    * row address bits
					    */
#define ACFG_SDRAM_REFRESH		3  /* 0=OFF 1=2048
					    * 2=4096 3=8192 refresh
					    */
#define ACFG_SDRAM_EXIT_PWD		25 /* ns exit power
					    * down delay
					    */
#define ACFG_SDRAM_W2R_DELAY		1  /* write to read cycle
					    * delay > 0
					    */
#define ACFG_SDRAM_ROW_PRECHARGE_DELAY	18 /* ns */
#define ACFG_SDRAM_TMRD_DELAY		2  /* Load mode register
					    * cycle delay 1..4
					    */
#define ACFG_SDRAM_TWR_DELAY		1  /* LPDDR: 0=2ck 1=3ck
					    * SDRAM: 0=1ck 1=2ck
					    */
#define ACFG_SDRAM_RAS_DELAY		42 /* ns ACTIVE-to-PRECHARGE delay */
#define ACFG_SDRAM_RRD_DELAY		12 /* ns ACTIVE-to-ACTIVE delay */
#define ACFG_SDRAM_RCD_DELAY		18 /* ns Row to Column delay */
#define ACFG_SDRAM_RC_DELAY		70 /* ns Row cycle delay (tRFC
					    * refresh to command)
					    */
#define ACFG_SDRAM_CLOCK_CYCLE_CL_1	0 /* ns clock cycle time
					   * estimated fo CL=1
					   * 0=force 3 for lpddr
					   */
#define ACFG_SDRAM_PARTIAL_ARRAY_SR	0  /* 0=full 1=half 2=quater
					    * 3=Eighth 4=Sixteenth
					    */
#define ACFG_SDRAM_DRIVE_STRENGH	0  /* 0=Full-strength
					    * 1=half
					    * 2=quater
					    * 3=Eighth
					    */
#define ACFG_SDRAM_BURST_LENGTH		3  /* 2^N BYTES (N=0..3) */
#define ACFG_SDRAM_SINGLE_ACCESS	0  /* 1= single access
					    * 0 = Burst mode
					    */
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
#define ACFG_CS0U_VAL	0x0000CC03
#define ACFG_CS0L_VAL	0xa0330D01
#define ACFG_CS0A_VAL	0x00220800

#define ACFG_CS1U_VAL	0x00000f00
#define ACFG_CS1L_VAL	0x00000D01
#define ACFG_CS1A_VAL	0

#define ACFG_CS2U_VAL	0
#define ACFG_CS2L_VAL	0
#define ACFG_CS2A_VAL	0

#define ACFG_CS3U_VAL	0
#define ACFG_CS3L_VAL	0
#define ACFG_CS3A_VAL	0

#define ACFG_CS4U_VAL	0
#define ACFG_CS4L_VAL	0
#define ACFG_CS4A_VAL	0

/* FPGA 16 bit data bus */
#define ACFG_CS5U_VAL	0x00000600
#define ACFG_CS5L_VAL	0x00000D01
#define ACFG_CS5A_VAL	0

#define ACFG_EIM_VAL	0x00002200


/*
 * FPGA specific settings
 */

/* CLKO */
#define ACFG_CCSR_VAL 0x00000305
/* drive strength CLKO set to 2 */
#define ACFG_DSCR10_VAL 0x00020000
/* drive strength A1..A12 set to 2 */
#define ACFG_DSCR3_VAL 0x02AAAAA8
/* drive strength ctrl */
#define ACFG_DSCR7_VAL 0x00020880
/* drive strength data */
#define ACFG_DSCR2_VAL 0xAAAAAAAA


/*
 * Default configuration for GPIOs and peripherals
 */
#define ACFG_DDIR_A_VAL		0x00000000
#define ACFG_OCR1_A_VAL		0x00000000
#define ACFG_OCR2_A_VAL		0x00000000
#define ACFG_ICFA1_A_VAL	0xFFFFFFFF
#define ACFG_ICFA2_A_VAL	0xFFFFFFFF
#define ACFG_ICFB1_A_VAL	0xFFFFFFFF
#define ACFG_ICFB2_A_VAL	0xFFFFFFFF
#define ACFG_DR_A_VAL		0x00000000
#define ACFG_GIUS_A_VAL		0xFFFFFFFF
#define ACFG_ICR1_A_VAL		0x00000000
#define ACFG_ICR2_A_VAL		0x00000000
#define ACFG_IMR_A_VAL		0x00000000
#define ACFG_GPR_A_VAL		0x00000000
#define ACFG_PUEN_A_VAL		0xFFFFFFFF

#define ACFG_DDIR_B_VAL		0x000001F0
#define ACFG_OCR1_B_VAL		0x0003ff00
#define ACFG_OCR2_B_VAL		0x00000000
#define ACFG_ICFA1_B_VAL	0xFFFC00FF
#define ACFG_ICFA2_B_VAL	0xFFFFFFFF
#define ACFG_ICFB1_B_VAL	0xFFFC00FF
#define ACFG_ICFB2_B_VAL	0xFFFFFFFF
#define ACFG_DR_B_VAL		0x000001F0
#define ACFG_GIUS_B_VAL		0xFF3FFFF0
#define ACFG_ICR1_B_VAL		0x00000000
#define ACFG_ICR2_B_VAL		0x00000000
#define ACFG_IMR_B_VAL		0x00000000
#define ACFG_GPR_B_VAL		0x00000000
#define ACFG_PUEN_B_VAL		0xFFFFFE0F

#define ACFG_DDIR_C_VAL		0x00000000
#define ACFG_OCR1_C_VAL		0x00000000
#define ACFG_OCR2_C_VAL		0x00000000
#define ACFG_ICFA1_C_VAL	0xFFFFFFFF
#define ACFG_ICFA2_C_VAL	0xFFFFFFFF
#define ACFG_ICFB1_C_VAL	0xFFFFFFFF
#define ACFG_ICFB2_C_VAL	0xFFFFFFFF
#define ACFG_DR_C_VAL		0x00000000
#define ACFG_GIUS_C_VAL		0xFFFFC07F
#define ACFG_ICR1_C_VAL		0x00000000
#define ACFG_ICR2_C_VAL		0x00000000
#define ACFG_IMR_C_VAL		0x00000000
#define ACFG_GPR_C_VAL		0x00000000
#define ACFG_PUEN_C_VAL		0xFFFFFF87

#define ACFG_DDIR_D_VAL		0x00000000
#define ACFG_OCR1_D_VAL		0x00000000
#define ACFG_OCR2_D_VAL		0x00000000
#define ACFG_ICFA1_D_VAL	0xFFFFFFFF
#define ACFG_ICFA2_D_VAL	0xFFFFFFFF
#define ACFG_ICFB1_D_VAL	0xFFFFFFFF
#define ACFG_ICFB2_D_VAL	0xFFFFFFFF
#define ACFG_DR_D_VAL		0x00000000
#define ACFG_GIUS_D_VAL		0xFFFFFFFF
#define ACFG_ICR1_D_VAL		0x00000000
#define ACFG_ICR2_D_VAL		0x00000000
#define ACFG_IMR_D_VAL		0x00000000
#define ACFG_GPR_D_VAL		0x00000000
#define ACFG_PUEN_D_VAL		0xFFFFFFFF

#define ACFG_DDIR_E_VAL		0x00000000
#define ACFG_OCR1_E_VAL		0x00000000
#define ACFG_OCR2_E_VAL		0x00000000
#define ACFG_ICFA1_E_VAL	0xFFFFFFFF
#define ACFG_ICFA2_E_VAL	0xFFFFFFFF
#define ACFG_ICFB1_E_VAL	0xFFFFFFFF
#define ACFG_ICFB2_E_VAL	0xFFFFFFFF
#define ACFG_DR_E_VAL		0x00000000
#define ACFG_GIUS_E_VAL		0xFCFFCCF8
#define ACFG_ICR1_E_VAL		0x00000000
#define ACFG_ICR2_E_VAL		0x00000000
#define ACFG_IMR_E_VAL		0x00000000
#define ACFG_GPR_E_VAL		0x00000000
#define ACFG_PUEN_E_VAL		0xFFFFFFFF

#define ACFG_DDIR_F_VAL		0x00000000
#define ACFG_OCR1_F_VAL		0x00000000
#define ACFG_OCR2_F_VAL		0x00000000
#define ACFG_ICFA1_F_VAL	0xFFFFFFFF
#define ACFG_ICFA2_F_VAL	0xFFFFFFFF
#define ACFG_ICFB1_F_VAL	0xFFFFFFFF
#define ACFG_ICFB2_F_VAL	0xFFFFFFFF
#define ACFG_DR_F_VAL		0x00000000
#define ACFG_GIUS_F_VAL		0xFF7F8000
#define ACFG_ICR1_F_VAL		0x00000000
#define ACFG_ICR2_F_VAL		0x00000000
#define ACFG_IMR_F_VAL		0x00000000
#define ACFG_GPR_F_VAL		0x00000000
#define ACFG_PUEN_F_VAL		0xFFFFFFFF

/* Enforce DDR signal strengh & enable USB/PP/DMA burst override bits */
#define ACFG_GPCR_VAL		0x0003000F

#define ACFG_ESDMISC_VAL	ESDMISC_LHD+ESDMISC_MDDREN

/* FMCR select num LPDDR RAMs and nand 16bits, 2KB pages */
#if (CONFIG_NR_DRAM_BANKS == 1)
#define ACFG_FMCR_VAL 0xFFFFFFF9
#elif (CONFIG_NR_DRAM_BANKS == 2)
#define ACFG_FMCR_VAL 0xFFFFFFFB
#endif

#define ACFG_AIPI1_PSR0_VAL	0x20040304
#define ACFG_AIPI1_PSR1_VAL	0xDFFBFCFB
#define ACFG_AIPI2_PSR0_VAL	0x00000000
#define ACFG_AIPI2_PSR1_VAL	0xFFFFFFFF

/* PCCR enable DMA FEC I2C1 IIM SDHC1 */
#define ACFG_PCCR0_VAL		0x05070410
#define ACFG_PCCR1_VAL		0xA14A0608

/*
 * From here, there should not be any user configuration.
 * All Equations are automatic
 */

/* fixme none integer value (7.5ns) => 2*hclock = 15ns */
#define ACFG_2XHCLK_LGTH	(2000/CONFIG_HCLK_FREQ)	/* ns */

/* USB 60 MHz ; ARM up to 400; HClK up to 133MHz*/
#define CSCR_MASK 0x0300800D

#define ACFG_CSCR_VAL						\
	(CSCR_MASK						\
	|((((CONFIG_SPLL_FREQ/CONFIG_USB_FREQ)-1)&0x07) << 28)	\
	|((((CONFIG_MPLL_FREQ/CONFIG_ARM_FREQ)-1)&0x03) << 12)	\
	|((((ACFG_CLK_FREQ/CONFIG_HCLK_FREQ)-1)&0x03) << 8))

/* SSIx CLKO NFC H264 MSHC */
#define ACFG_PCDR0_VAL\
	(((((ACFG_CLK_FREQ/CONFIG_MSHC_FREQ)-1)&0x3F)<<0)	\
	|((((CONFIG_HCLK_FREQ/CONFIG_NFC_FREQ)-1)&0x0F)<<6)	\
	|(((((ACFG_CLK_FREQ/CONFIG_H264_FREQ)-2)*2)&0x3F)<<10)\
	|(((((ACFG_CLK_FREQ/CONFIG_SSI1_FREQ)-2)*2)&0x3F)<<16)\
	|(((CONFIG_CLK0_DIV)&0x07)<<22)\
	|(((CONFIG_CLK0_EN)&0x01)<<25)\
	|(((((ACFG_CLK_FREQ/CONFIG_SSI2_FREQ)-2)*2)&0x3F)<<26))

/* PERCLKx  */
#define ACFG_PCDR1_VAL\
	(((((ACFG_CLK_FREQ/CONFIG_PERIF1_FREQ)-1)&0x3F)<<0)	\
	|((((ACFG_CLK_FREQ/CONFIG_PERIF2_FREQ)-1)&0x3F)<<8)	\
	|((((ACFG_CLK_FREQ/CONFIG_PERIF3_FREQ)-1)&0x3F)<<16)	\
	|((((ACFG_CLK_FREQ/CONFIG_PERIF4_FREQ)-1)&0x3F)<<24))

/* SDRAM controller programming Values */
#if (((2*ACFG_SDRAM_CLOCK_CYCLE_CL_1) > (3*ACFG_2XHCLK_LGTH)) || \
	(ACFG_SDRAM_CLOCK_CYCLE_CL_1 < 1))
#define REG_FIELD_SCL_VAL 3
#define REG_FIELD_SCLIMX_VAL 0
#else
#define REG_FIELD_SCL_VAL\
	((2*ACFG_SDRAM_CLOCK_CYCLE_CL_1+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)
#define REG_FIELD_SCLIMX_VAL REG_FIELD_SCL_VAL
#endif

#if ((2*ACFG_SDRAM_RC_DELAY) > (16*ACFG_2XHCLK_LGTH))
#define REG_FIELD_SRC_VAL 0
#else
#define REG_FIELD_SRC_VAL\
	((2*ACFG_SDRAM_RC_DELAY+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)
#endif

/* TBD Power down timer ; PRCT Bit Field Encoding; burst length 8 ; FP = 0*/
#define REG_ESDCTL_BASE_CONFIG (0x80020485\
				| (((ACFG_SDRAM_NUM_ROW-11)&0x7)<<24)\
				| (((ACFG_SDRAM_NUM_COL-8)&0x3)<<20)\
				| (((ACFG_SDRAM_REFRESH)&0x7)<<13))

#define ACFG_NORMAL_RW_CMD	((0x0<<28)+REG_ESDCTL_BASE_CONFIG)
#define ACFG_PRECHARGE_CMD	((0x1<<28)+REG_ESDCTL_BASE_CONFIG)
#define ACFG_AUTOREFRESH_CMD	((0x2<<28)+REG_ESDCTL_BASE_CONFIG)
#define ACFG_SET_MODE_REG_CMD	((0x3<<28)+REG_ESDCTL_BASE_CONFIG)

/* ESDRAMC Configuration Registers : force CL=3 to lpddr */
#define ACFG_SDRAM_ESDCFG_REGISTER_VAL (0x0\
	| (((((2*ACFG_SDRAM_EXIT_PWD+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)-1)&0x3)<<21)\
	| (((ACFG_SDRAM_W2R_DELAY-1)&0x1)<<20)\
	| (((((2*ACFG_SDRAM_ROW_PRECHARGE_DELAY+ \
		ACFG_2XHCLK_LGTH-1)/ACFG_2XHCLK_LGTH)-1)&0x3)<<18) \
	| (((ACFG_SDRAM_TMRD_DELAY-1)&0x3)<<16)\
	| (((ACFG_SDRAM_TWR_DELAY)&0x1)<<15)\
	| (((((2*ACFG_SDRAM_RAS_DELAY+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)-1)&0x7)<<12) \
	| (((((2*ACFG_SDRAM_RRD_DELAY+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)-1)&0x3)<<10) \
	| (((REG_FIELD_SCLIMX_VAL)&0x3)<<8)\
	| (((((2*ACFG_SDRAM_RCD_DELAY+ACFG_2XHCLK_LGTH-1)/ \
		ACFG_2XHCLK_LGTH)-1)&0x7)<<4) \
	| (((REG_FIELD_SRC_VAL)&0x0F)<<0))

/* Issue Mode register Command to SDRAM */
#define ACFG_SDRAM_MODE_REGISTER_VAL\
	((((ACFG_SDRAM_BURST_LENGTH)&0x7)<<(0))\
	| (((REG_FIELD_SCL_VAL)&0x7)<<(4))\
	| ((0)<<(3)) /* sequentiql access */ \
	/*| (((ACFG_SDRAM_SINGLE_ACCESS)&0x1)<<(1))*/)

/* Issue Extended Mode register Command to SDRAM */
#define ACFG_SDRAM_EXT_MODE_REGISTER_VAL\
	((ACFG_SDRAM_PARTIAL_ARRAY_SR<<0)\
	| (ACFG_SDRAM_DRIVE_STRENGH<<(5))\
	| (1<<(ACFG_SDRAM_NUM_COL+ACFG_SDRAM_NUM_ROW+1+2)))

/* Issue Precharge all Command to SDRAM */
#define ACFG_SDRAM_PRECHARGE_ALL_VAL (1<<10)

#endif /* __CONFIG_H */

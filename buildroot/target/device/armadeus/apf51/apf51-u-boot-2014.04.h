/*
 * Copyright (C) 2010-2013 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION	11
#define CONFIG_IDENT_STRING	" apf51 patch 2.3"
#define CONFIG_BOARD_NAME apf51

/*
 * SoC configurations
 */
#define CONFIG_ARMV7			/* This is armv7 Cortex-A8 CPU  */
#define CONFIG_MX51			/* in a Freescale i.MX51 Chip */
#define CONFIG_MACH_TYPE	2795	/* APF51 */

/*
 * Enable the call to miscellaneous platform dependent initialization.
 */
#define CONFIG_SYS_NO_FLASH	/* to be define before <config_cmd_default.h> */
#define CONFIG_BOARD_EARLY_INIT_F

/*
 * Board display option
 */
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_DISPLAY_CPUINFO

/*
 * SPL
 */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_TARGET		"u-boot-with-nand-spl.imx"
#define CONFIG_SPL_MAX_SIZE		0x4000 /* can be extended to 128kiB */
/* CONFIG_SPL_TEXT_BASE -> IRAM start address available for SPL :
 * - 0x1FFE0000	IRAM base address
 * - 0x00002000	IRAM space reserved for IPL
 * - 0x00002000	reserve space SPL DCD & header
 */
#define CONFIG_SPL_TEXT_BASE		0x1FFE4000 
#define CONFIG_SPL_STACK		CONFIG_SYS_INIT_RAM_ADDR \
					+ CONFIG_SYS_INIT_RAM_SIZE \
					- 0xC000 /* MMU and IPL stack */
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_NAND_SUPPORT
#define CONFIG_SPL_RAM_DEVICE


/* NAND boot config */
#define CONFIG_SYS_NAND_U_BOOT_OFFS	CONFIG_SPL_PAD_TO
#define CONFIG_SYS_NAND_U_BOOT_SIZE	CONFIG_SYS_MONITOR_LEN
#define CONFIG_SYS_NAND_U_BOOT_DST	CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START	CONFIG_SYS_TEXT_BASE

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

#define CONFIG_CMD_ASKENV	/* ask for env variable         */
#define CONFIG_CMD_BSP		/* Board Specific functions     */
#define CONFIG_CMD_CACHE	/* icache, dcache               */
/*#define CONFIG_CMD_DATE*/
#define CONFIG_CMD_DHCP		/* DHCP Support                 */
#define CONFIG_CMD_DNS
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_I2C		/* I2C serial bus support       */
#define CONFIG_CMD_IIM		/* imx iim fuse                 */
#define CONFIG_CMD_MII		/* MII support			*/
#define CONFIG_CMD_MMC
#define CONFIG_CMD_MTDPARTS	/* MTD partition support        */
#define CONFIG_CMD_NAND		/* NAND support                 */
#define CONFIG_CMD_NAND_LOCK_UNLOCK
#define CONFIG_CMD_NAND_TRIMFFS
#define CONFIG_CMD_NFS		/* NFS support			*/
#define CONFIG_CMD_PING		/* ping support                 */
#define CONFIG_CMD_SETEXPR	/* setexpr support		*/
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

/*
 * Memory configurations
 */
#define CONFIG_NR_DRAM_POPULATED 1
#define CONFIG_NR_DRAM_BANKS		2
#define CONFIG_SYS_SDRAM_BASE		CSD0_BASE_ADDR	/* SDRAM bank #1 */
#define PHYS_SDRAM_SIZE			256
#define PHYS_SDRAM_1_SIZE		(PHYS_SDRAM_SIZE) * 1024 * 1024
#define PHYS_SDRAM_2_SIZE		(PHYS_SDRAM_SIZE) * 1024 * 1024
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (2 * 1024 * 1024))
#define CONFIG_SYS_MEMTEST_START	0x90000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x90100000	/* 1st MiB in DRAM */

#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_SYS_TEXT_BASE		0x90004040

/*
 * FLASH organization
 */
#define	CONFIG_SYS_MONITOR_OFFSET	0x00000000	/* offset in NAND */
#define	CONFIG_SYS_MONITOR_LEN		0x00100000	/* 1MB */
#define CONFIG_ENV_IS_IN_NAND
#define	CONFIG_ENV_OVERWRITE
#define	CONFIG_ENV_OFFSET		0x00100000	/* NAND offset */
#define	CONFIG_ENV_SIZE			0x00020000	/* 128kB  */
#define CONFIG_ENV_RANGE		0x00080000	/* 512kB */
#define	CONFIG_ENV_OFFSET_REDUND	\
		(CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)	/* +512kB */
#define	CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE	/* 128kB */
#define	CONFIG_FIRMWARE_OFFSET		0x00200000
#define	CONFIG_FIRMWARE_SIZE		0x00100000
#define	CONFIG_KERNEL_OFFSET		0x00400000
#define	CONFIG_ROOTFS_OFFSET		0x00C00000

#define CONFIG_MTDMAP			"mxc_nand"
#define MTDIDS_DEFAULT			"nand0=" CONFIG_MTDMAP
#define MTDPARTS_DEFAULT	"mtdparts=" CONFIG_MTDMAP \
				":1M(u-boot)ro,"\
				"512K(env),"	\
				"512K(env2),"	\
				"1M(firmware),"	\
				"512K(dtb),"	\
				"512K(splash),"	\
				"8M(kernel),"	\
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

#define CONFIG_REVISION_TAG
#define CONFIG_OF_LIBFDT

#define CONFIG_BOOTDELAY	5
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		__stringify(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," \
				__stringify(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
			" ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs "

#define ACFG_CONSOLE_DEV	ttymxc2
#define CONFIG_BOOTCOMMAND	"run ubifsboot"
#define CONFIG_SYS_AUTOLOAD	"no"
/*
 * Default load address for user programs and kernel
 */
#define CONFIG_LOADADDR			0x90000000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/*
 * Extra Environments
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"env_version="		__stringify(CONFIG_ENV_VERSION)		"\0" \
	"consoledev="		__stringify(ACFG_CONSOLE_DEV)		"\0" \
	"mtdparts="		MTDPARTS_DEFAULT			"\0" \
	"partition=nand0,7\0"						\
	"uboot_addr="		__stringify(CONFIG_SYS_MONITOR_OFFSET)	"\0" \
	"env_addr="		__stringify(CONFIG_ENV_OFFSET)		"\0" \
	"firmware_addr="	__stringify(CONFIG_FIRMWARE_OFFSET)	"\0" \
	"firmware_size="	__stringify(CONFIG_FIRMWARE_SIZE)	"\0" \
	"kernel_addr="		__stringify(CONFIG_KERNEL_OFFSET)	"\0" \
	"rootfs_addr="		__stringify(CONFIG_ROOTFS_OFFSET)	"\0" \
	"board_name="		__stringify(CONFIG_BOARD_NAME)		"\0" \
	"kernel_addr_r=90800000\0"					\
	"check_env=if test -n ${flash_env_version}; "			\
		"then env default env_version; "			\
		"else env set flash_env_version ${env_version}; env save; "\
		"fi; "							\
		"if itest ${flash_env_version} != ${env_version}; then " \
			"echo \"*** Warning - Environment version"	\
			" change suggests: run flash_reset_env; reset\"; "\
			"env default flash_reset_env; "			\
		"else exit; fi; \0"					\
	"check_flash=nand lock; nand unlock ${env_addr}; \0"		\
	"flash_reset_env=env default -f -a && saveenv &&"		\
		"echo Flash environment variables erased!\0"		\
	"download_uboot=tftpboot ${loadaddr} ${board_name}"		\
		"-u-boot-with-nand-spl.imx\0"				\
	"flash_uboot=nand unlock ${uboot_addr} ;"			\
		"nand erase.part u-boot;"				\
		"if nand write.trimffs ${fileaddr} ${uboot_addr} ${filesize};"\
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
	"firmware_autoload=0\0"						\
	"checknload_firmware=if test ${firmware_autoload} -eq 1;"	\
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
		"if test ${firmware_autoload} -eq 1;"			\
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
	"unlock_regs=echo unlock_reg not needed anymore\0"

/*
 * Serial Driver
 */
#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART3_BASE
#define CONFIG_BAUDRATE			115200

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

#define CONFIG_MXC_NAND_REGS_BASE	NFC_BASE_ADDR_AXI
#define CONFIG_MXC_NAND_IP_REGS_BASE	NFC_BASE_ADDR
#define CONFIG_SYS_NAND_BASE		CONFIG_MXC_NAND_REGS_BASE
#define CONFIG_SYS_NAND_SIZE		(256 * 1024 * 1024)
#define CONFIG_SYS_MAX_NAND_DEVICE	1

#define CONFIG_MXC_NAND_HWECC
#define CONFIG_SYS_NAND_LARGEPAGE
#define CONFIG_SYS_NAND_USE_FLASH_BBT
#define NAND_MAX_CHIPS			1

#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_PAGE_COUNT	64
#define CONFIG_SYS_NAND_PAGE_SIZE	2048
#define CONFIG_SYS_NAND_BLOCK_SIZE	(128*1024)
#define CONFIG_SYS_NAND_BAD_BLOCK_POS	0
#define CONFIG_SYS_NAND_OOBSIZE		64

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
 * Ethernet (on SOC mx51 FEC)
 */
#define CONFIG_FEC_MXC
#define IMX_FEC_BASE			FEC_BASE_ADDR
#define CONFIG_SYS_PHY_RESET_GPIO	IMX_GPIO_NR(3, 0)

#define CONFIG_FEC_MXC_PHYADDR		0x02
#define CONFIG_MII
#define CONFIG_DISCOVER_PHY

/*
 * FPGA
 */
#define CONFIG_FPGA
#define CONFIG_FPGA_COUNT		1
#define CONFIG_FPGA_XILINX
#define CONFIG_FPGA_SPARTAN3
#define CONFIG_SYS_FPGA_WAIT		250	/* 250ms */
#define CONFIG_SYS_FPGA_PROG_FEEDBACK
#define CONFIG_SYS_FPGA_CHECK_CTRLC
#define CONFIG_SYS_FPGA_CHECK_ERROR

/* FPGA program pin configuration */
#define ACFG_FPGA_PWR	IMX_GPIO_NR(3, 3)
#define ACFG_FPGA_PRG	IMX_GPIO_NR(4, 9)
#define ACFG_FPGA_RDATA	CS1_BASE_ADDR
#define ACFG_FPGA_WDATA	CS1_BASE_ADDR
#define ACFG_FPGA_INIT	IMX_GPIO_NR(4, 11)
#define ACFG_FPGA_DONE	IMX_GPIO_NR(4, 10)
#define ACFG_FPGA_SUSPEND IMX_GPIO_NR(3, 6)

#define	ACFG_FPGA_GCR1_VALUE	0x019100bf
#define	ACFG_FPGA_GCR2_VALUE	0x00000000

#define	ACFG_FPGA_RCR1_VALUE	0x04000010
#define	ACFG_FPGA_RCR2_VALUE	0x00000000

#define	ACFG_FPGA_WCR1_VALUE	0x04000008
#define	ACFG_FPGA_WCR2_VALUE	0x00000000

/*
 * Fuses - IIM
 */

#define CONFIG_IMX_IIM
#define CONFIG_IIM_MAC_BANK     1
#define CONFIG_IIM_MAC_ROW      9

/*
 * I2C
 */
#ifdef	CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_MXC_I2C2_SPEED	400000
#define CONFIG_SYS_MXC_I2C2_SLAVE	0xfe
#define CONFIG_SYS_SPD_BUS_NUM		1 /* hack to force bus 1 by default */
#endif /* CONFIG_CMD_I2C */

/*
 * SPI
 */

/*
 * SD/MMC
 */
#ifdef CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	MMC_SDHC1_BASE_ADDR
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#endif

/*
 * RTC
 */

/*
 * Watchdog Config
 */
/*#define CONFIG_HW_WATCHDOG
#define CONFIG_IMX_WATCHDOG*/
/*
 * USB
 */

/*
 * Clocks
 */
#define CONFIG_SYS_MX5_CLK32		32768
#define CONFIG_SYS_MX5_HCLK		1024*CONFIG_SYS_MX5_CLK32
#define CONFIG_MX51_PLL_ERRATA

/* external bus frequencies */
#define APF_DDR_CLK	200	/* MHZ DDR frequency from PLL1 */
#define APF_ARM_CLK	800	/* MHZ CPU frequency from PLL1 */
#define APF_AXIA_CLK	166	/* MHZ AXI A frequency from PLL2 */
#define APF_AXIB_CLK	110	/* 110.8 133 MHZ AXI B frequency from PLL2 */
#define APF_EMI_CLK	95	/* MHZ AHB frequency from PLL2 */
#define APF_AHB_CLK	133	/* MHZ AHB frequency from PLL2 */
#define APF_IPG_CLK	66	/* 66.5 MHZ IPG frequency from PLL2 */
#define APF_PERCLK	55	/* 55.4 MHZ PERCLK frequency */
#define APF_NFC_CLK	23	/* MHZ NFC Clock from EMI clk */
#define APF_USB_CLK	54	/* external USB serial bus frequency PLL3 */
#define APF_ESDHC_CLK	54	/* MHz ESDHC controller frequency / PLL3 */
#define APF_UART_CLK	66	/* 66.5 MHz UART frequency from PLL2 */

/*#define APF_SSI_CLK	54*/	/* MHz SSI controller frequency */
/*#define APF_TVE_CLK	216*/	/* MHz TVE controller frequency */
/*#define APF_DI_CLK	0*/	/* MHz DISPLAY controller frequency*/
/*#define APF_VPU_CLK	0*/	/* MHz VPU controller frequency*/
/*#define APF_SPDIF_CLK	10*/	/* MHz SPDIF controller frequency 10.3 MHz*/
/*#define APF_SIM_CLK	54*/	/* MHz SIM controller frequency */
/*#define APF_FIRI_CLK	54*/	/* MHz FIRI controller frequency */
/*#define APF_HSI2C_CLK	54*/	/* MHz HS I2C controller frequency */
/*#define APF_CSI_CLK	54*/	/* MHz HS CSI controller frequency */
/*#define APF_ECSPI_CLK	54*/	/* MHz HECSPI controller frequency */
/*#define APF_WRCK_CLK	13*/	/* MHz WRCK 13.3 MHz */
/*#define APF_LPSR_CLK	0*/	/* MHz  */
#define APF_PGC_CLK	66	/* MHz 66.5 MHz */

/* PLL parameters assuming 32.768kHz *1024 input clock with doubler ON */
/*				  MFI         PDF */
#define DP_OP_864		((11 << 4) + ((2 - 1)  << 0))
#define DP_MFD_864		(65536 - 1)
#define DP_MFN_864		122854
#define DP_MFN_800_DIT		60354

#define DP_OP_850		((6 << 4) + ((1 - 1)  << 0))
#define DP_MFD_850		(1048576 - 1)
#define DP_MFN_850		349169

#define ACFG_PLL1_CLK		800	/* MHz */
#define DP_OP_800		((11 << 4) + ((2 - 1)  << 0))
#define DP_MFD_800		(65536 - 1)
#define DP_MFN_800		60354

/* 455MHz only needed to define references to compile U-Boot */
#define DP_OP_455		((9 << 4) + ((2 - 1)  << 0))
#define DP_MFD_455		(48 - 1)
#define DP_MFN_455		23

#define DP_OP_400		((11 << 4) + ((3 - 1)  << 0))
#define DP_MFD_400		(65536 - 1)
#define DP_MFN_400		60354

#define ACFG_PLL2_CLK		665	/* MHz */
#define DP_OP_665		((14 << 4) + ((3 - 1)  << 0))
#define DP_MFD_665		(2097152 - 1)
#define DP_MFN_665		1811747

#define ACFG_PLL3_CLK		216	/* MHz */
#define DP_OP_216			((8 << 4) + ((5 - 1)  << 0))
#define DP_MFD_216		(262144 - 1)
#define DP_MFN_216		12223

#define CONFIG_SYS_DDR_CLKSEL	0

/*
 * SDRAM
 */
/*
 * ACFG_SDRAM_NUM_COL	8, 9, 10 or 11 column address bits
 * ACFG_SDRAM_NUM_ROW	11, 12 or 13 row address bits
 * ACFG_SDRAM_REFRESH	0=OFF 1=2048 2=4096 3=8192.. refresh
 * ACFG_SDRAM_XSR_DELAY	ns Exit self refresh to any command (>= 25 ddr_clk)
 * ACFG_SDRAM_EXIT_PWD	ns exit power down delay
 * ACFG_SDRAM_W2R_DELAY	WRITE to READ Command Delay 0=1ck 1=2ck
 * ACFG_SDRAM_ROW_PRECHARGE_DELAY	ns (>2 x ddr_clk)
 * ACFG_SDRAM_TMRD_DELAY	Load mode register cycle delay 1..4
 * ACFG_SDRAM_TWR_DELAY	DDR: 0=2ck 1=3ck
 * ACFG_SDRAM_RAS_DELAY	ns ACTIVE-to-PRECHARGE delay
 * ACFG_SDRAM_RRD_DELAY	ns ACTIVE-to-ACTIVE delay
 * ACFG_SDRAM_RCD_DELAY	ns Row to Column delay
 * ACFG_SDRAM_RC_DELAY	ns Row cycle delay (tRFC refresh to command)
 * ACFG_SDRAM_RFC_DELAY	ns (> 10xddr_clk) auto refresh to any command
 * ACFG_SDRAM_PARTIAL_ARRAY_SR	0=full 1=half 2=quater 3=Eighth 4=Sixteenth
 * ACFG_SDRAM_DRIVE_STRENGH	0=Full-strength 1=half 2=quater 3=Eighth
 * ACFG_SDRAM_BURST_LENGTH	2^N BYTES (N=0..3)
 * ACFG_SDRAM_SINGLE_ACCESS	1= single access; 0 = Burst mode
 *
 */
/* micron 256MB */
#define ACFG_SDRAM_NUM_COL		10	/* 8..11 column address bits */
#define ACFG_SDRAM_NUM_ROW		14	/* 11..13 row address bits */
#define ACFG_SDRAM_REFRESH		3	/* 0=OFF 1=2048 2=4096 3=8192.*/
#define ACFG_SDRAM_XSR_DELAY		125	/* ns Exit self refresh */
#define ACFG_SDRAM_EXIT_PWD		10	/* ns exit power down delay */
#define ACFG_SDRAM_W2R_DELAY		1	/* 0=1ck 1=2ck */
#define ACFG_SDRAM_ROW_PRECHARGE_DELAY	15	/* ns (>2 x ddr_clk) */
#define ACFG_SDRAM_TMRD_DELAY		2	/* 1..4 */
#define ACFG_SDRAM_TWR_DELAY		1	/* DDR: 0=2ck 1=3ck */
#define ACFG_SDRAM_RAS_DELAY		40	/* ns ACTIVE-to-PRECHARGE */
#define ACFG_SDRAM_RRD_DELAY		10	/* ns ACTIVE-to-ACTIVE delay */
#define ACFG_SDRAM_RCD_DELAY		15	/* ns Row to Column delay */
#define ACFG_SDRAM_RC_DELAY		55	/* ns Row cycle delay */
#define ACFG_SDRAM_RFC_DELAY		72	/* ns (> 10xddr_clk) */
#define ACFG_SDRAM_PARTIAL_ARRAY_SR	0	/* 0..4 */
#define ACFG_SDRAM_DRIVE_STRENGH	0	/* 0..3 */
#define ACFG_SDRAM_BURST_LENGTH		3	/* 2^N BYTES (N=0..3) */
#define ACFG_SDRAM_SINGLE_ACCESS	0	/* 0..1 */

/*
 * External interface
 */
  /* force BCLK free run */
#define ACFG_WCR_VALUE		0x00000021
#define ACFG_WIAR_VALUE		0x00000010

/* CS1 */
#define ACFG_CS1GCR1_VALUE	ACFG_FPGA_GCR1_VALUE
#define ACFG_CS1GCR2_VALUE	ACFG_FPGA_GCR2_VALUE

#define ACFG_CS1RCR1_VALUE	ACFG_FPGA_RCR1_VALUE
#define ACFG_CS1RCR2_VALUE	ACFG_FPGA_RCR2_VALUE

#define ACFG_CS1WCR1_VALUE	ACFG_FPGA_WCR1_VALUE
#define ACFG_CS1WCR2_VALUE	ACFG_FPGA_WCR2_VALUE

/* CS2 */
#define ACFG_CS2GCR1_VALUE	0x00010080
#define ACFG_CS2GCR2_VALUE	0x00000000

#define ACFG_CS2RCR1_VALUE	0x00000000
#define ACFG_CS2RCR2_VALUE	0x00000000

#define ACFG_CS2WCR1_VALUE	0x00000000
#define ACFG_CS2WCR2_VALUE	0x00000000

/* CS3 */
#define ACFG_CS3GCR1_VALUE	0x00010080
#define ACFG_CS3GCR2_VALUE	0x00000000

#define ACFG_CS3RCR1_VALUE	0x00000000
#define ACFG_CS3RCR2_VALUE	0x00000000

#define ACFG_CS3WCR1_VALUE	0x00000000
#define ACFG_CS3WCR2_VALUE	0x00000000

/* CS4 */
#define ACFG_CS4GCR1_VALUE	0x00010080
#define ACFG_CS4GCR2_VALUE	0x00000000

#define ACFG_CS4RCR1_VALUE	0x00000000
#define ACFG_CS4RCR2_VALUE	0x00000000

#define ACFG_CS4WCR1_VALUE	0x00000000
#define ACFG_CS4WCR2_VALUE	0x00000000

/* CS5 */
#define ACFG_CS5GCR1_VALUE	0x00010080
#define ACFG_CS5GCR2_VALUE	0x00000000

#define ACFG_CS5RCR1_VALUE	0x00000000
#define ACFG_CS5RCR2_VALUE	0x00000000

#define ACFG_CS5WCR1_VALUE	0x00000000
#define ACFG_CS5WCR2_VALUE	0x00000000

/*
 * From here, there should not be any user configuration.
 * All Equations are automatic
 */
#include <asm/arch/imx-regs.h>

/* Issue Mode register Command to SDRAM*/
#define ACFG_DRR_MR_VAL (0x0\
	| (((ACFG_SDRAM_BURST_LENGTH)&0x7)<<(0))\
	| ((3)<<(4)) \
	| (((ACFG_SDRAM_SINGLE_ACCESS)&0x1)<<(3)))

/* Issue Extended Mode register Command to SDRAM*/
#define ACFG_DRR_EMR_VAL\
	(((ACFG_SDRAM_PARTIAL_ARRAY_SR&0x7)<<0)\
	| ((ACFG_SDRAM_DRIVE_STRENGH&0x7)<<(5)))

#define ACFG_SDRAM_ESDGPR_REGISTER_VAL		0x20020000
#define ACFG_SDRAM_ESDCDLY1_REGISTER_VAL	0x00F48000
#define ACFG_SDRAM_ESDCDLY2_REGISTER_VAL	0x00F48000
#define ACFG_SDRAM_ESDCDLY3_REGISTER_VAL	0x00F48000
#define ACFG_SDRAM_ESDCDLY4_REGISTER_VAL	0x00F48000
#define ACFG_SDRAM_ESDCDLY5_REGISTER_VAL	0x00F48000

/* SDRAM IO Muxing */
#define ACFG_IOMUX_PAD_DRAM_RAS	PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_CAS	PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDWE \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDCKE0 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDCKE1 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDCLK \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDQS0 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_DOWN \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDQS1 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_DOWN \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDQS2 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_DOWN \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_SDQS3 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_DOWN \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_CS0 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_CS1 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_DQM0 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_DQM1 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_DQM2 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_PAD_DRAM_DQM3 \
	PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP \
	| PAD_CTL_DSE_MAX | PAD_CTL_SRE_SLOW

#define ACFG_IOMUX_PAD_EIM_SDBA2 \
	PAD_CTL_DVS | PAD_CTL_PKE | PAD_CTL_PUE \
	| PAD_CTL_PUS_100K_UP
#define ACFG_IOMUX_PAD_EIM_SDODT1 \
	PAD_CTL_DVS | PAD_CTL_PKE | PAD_CTL_PUE \
	| PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_LOW | PAD_CTL_SRE_FAST
#define ACFG_IOMUX_PAD_EIM_SDODT0 \
	PAD_CTL_DVS | PAD_CTL_PKE | PAD_CTL_PUE \
	| PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_LOW | PAD_CTL_SRE_FAST

#define ACFG_IOMUX_GRP_DDRPKS		PAD_CTL_PUE /* imx default */
#define ACFG_IOMUX_GRP_DRAM_B4		PAD_CTL_DSE_MAX
#define ACFG_IOMUX_GRP_INDDR		0 /* imx default */
#define ACFG_IOMUX_GRP_PKEDDR		PAD_CTL_PKE /* imx default */
#define ACFG_IOMUX_GRP_DDR_A0		PAD_CTL_DSE_MAX
#define ACFG_IOMUX_GRP_DDR_A1		PAD_CTL_DSE_MAX
#define ACFG_IOMUX_GRP_DDRAPUS		PAD_CTL_PUS_100K_UP /* imx default */
#define ACFG_IOMUX_GRP_HYSDDR0		0 /* imx default */
#define ACFG_IOMUX_GRP_HYSDDR1		0 /* imx default */
#define ACFG_IOMUX_GRP_HYSDDR2		0 /* imx default */
#define ACFG_IOMUX_GRP_HVDDR		PAD_CTL_DVS
#define ACFG_IOMUX_GRP_HYSDDR3		0 /* imx default */
#define ACFG_IOMUX_GRP_DDR_SR_B0	PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_GRP_DDRAPKS		PAD_CTL_PUE	/* imx default */

#define ACFG_IOMUX_GRP_DDR_SR_B1	PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_GRP_DDRPUS		PAD_CTL_PUS_100K_UP /* imx default */
#define ACFG_IOMUX_GRP_DDR_SR_B2	PAD_CTL_SRE_SLOW
#define ACFG_IOMUX_GRP_PKEADDR		PAD_CTL_PKE /* imx default */
#define ACFG_IOMUX_GRP_DDR_SR_B4	PAD_CTL_SRE_SLOW

#define ACFG_IOMUX_GRP_INMODE1		0 /* imx default */
#define ACFG_IOMUX_GRP_DRAM_B0		PAD_CTL_DSE_MAX
#define ACFG_IOMUX_GRP_DRAM_B1		PAD_CTL_DSE_MAX

#define ACFG_IOMUX_GRP_DDR_SR_A0	PAD_CTL_SRE_SLOW

#define ACFG_IOMUX_GRP_DRAM_B2		PAD_CTL_DSE_MAX

#define ACFG_IOMUX_GRP_DDR_SR_A1	PAD_CTL_SRE_SLOW
/* end of SDRAM IO Muxing */

/* CCM Configuration */

#define CONFIG_SYS_PLL_DP_CTL		0x00001332	/* use fpm_clk */
#define CONFIG_SYS_CLKCTL_CCMR		0x00001FFF
#define CONFIG_SYS_CLKCTL_CSR		0x0000003E
#define CONFIG_SYS_CLKCTL_CCSR		0x00000200

#define CCBDR_MASK 0x40000000

#define CONFIG_SYS_CLKTL_CBCDR\
	(CCBDR_MASK					\
	|((((ACFG_PLL1_CLK/APF_DDR_CLK)-1)&0x07)<<27)	\
	|((((ACFG_PLL2_CLK/APF_EMI_CLK)-1)&0x07)<<22)	\
	|((((ACFG_PLL2_CLK/APF_AXIB_CLK)-1)&0x07)<<19)	\
	|((((ACFG_PLL2_CLK/APF_AXIA_CLK)-1)&0x07)<<16)	\
	|((((APF_EMI_CLK/APF_NFC_CLK)-1)&0x07)<<13)	\
	|((((ACFG_PLL2_CLK/APF_AHB_CLK)-1)&0x07)<<10)	\
	|((((APF_AHB_CLK/APF_IPG_CLK)-1)&0x03)<<8)	\
	|(((((ACFG_PLL2_CLK/APF_PERCLK)/8))&0x03)<<6)	\
	|(((((ACFG_PLL2_CLK/APF_PERCLK)			\
	 /((ACFG_PLL2_CLK/APF_PERCLK)/8+1))-1)&0x07)<<0)\
	)

#define CONFIG_SYS_CLKTL_CBCMR 0x000020C0

#define CONFIG_SYS_CLKTL_CBCMR1 0xA5A2A020

#define CONFIG_SYS_CLKTL_CSCDR1\
	(0x0							\
	|((((ACFG_PLL3_CLK/APF_ESDHC_CLK)/8)&0x07)<<19)		\
	|(((((ACFG_PLL3_CLK/APF_ESDHC_CLK)			\
	 /((ACFG_PLL3_CLK/APF_ESDHC_CLK)/8+1))-1)&0x07)<<22)	\
	|((((ACFG_PLL3_CLK/APF_ESDHC_CLK)/8)&0x07)<<11)		\
	|((((APF_IPG_CLK/APF_PGC_CLK)-1)&0x07)<<14)		\
	|(((((ACFG_PLL3_CLK/APF_ESDHC_CLK)			\
	 /((ACFG_PLL3_CLK/APF_ESDHC_CLK)/8+1))-1)&0x07)<<16)	\
	|((((ACFG_PLL3_CLK/APF_USB_CLK)/4)&0x07)<<6)		\
	|(((((ACFG_PLL3_CLK/APF_USB_CLK)			\
	 /((ACFG_PLL3_CLK/APF_USB_CLK)/4+1))-1)&0x03)<<8)	\
	|((((ACFG_PLL2_CLK/APF_UART_CLK)/8)&0x07)<<0)		\
	|(((((ACFG_PLL2_CLK/APF_UART_CLK)			\
	 /((ACFG_PLL2_CLK/APF_UART_CLK)/8+1))-1)&0x07)<<3)	\
	)

/* clock cycle time with x2 for a 0.5ns accuracy */
#define CONFIG_SYS_2XDDRCLK_LGTH	(2000/APF_DDR_CLK)	/* ns */

#define ACFG_SDRAM_ESDMISC_REGISTER_VAL 0x000a5080

#if ((2*ACFG_SDRAM_RC_DELAY) > (16*CONFIG_SYS_2XDDRCLK_LGTH))
#define REG_FIELD_RC_VAL 0
#else
#define REG_FIELD_RC_VAL \
	(((2*ACFG_SDRAM_RC_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)	\
	/CONFIG_SYS_2XDDRCLK_LGTH)-1)
#endif

#if ((2*ACFG_SDRAM_RFC_DELAY) > (25*CONFIG_SYS_2XDDRCLK_LGTH))
#define REG_FIELD_RFC_VAL \
	(((2*ACFG_SDRAM_RFC_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)	\
	/CONFIG_SYS_2XDDRCLK_LGTH/2)-10)
#define REG_FIELD_ESDCTL_PAT 0x80820000
#else
#define REG_FIELD_RFC_VAL \
	(((2*ACFG_SDRAM_RFC_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)	\
	/CONFIG_SYS_2XDDRCLK_LGTH)-10)
#define REG_FIELD_ESDCTL_PAT 0x80020000
#endif

/* ESDCTLx Configuration Registers: 32bits */
#define ACFG_SDRAM_ESDCTL_REGISTER_VAL (REG_FIELD_ESDCTL_PAT\
	| (((ACFG_SDRAM_REFRESH)&0x0F)<<28)\
	| (((ACFG_SDRAM_NUM_ROW-11)&0x7)<<24)\
	| (((ACFG_SDRAM_NUM_COL-8)&0x3)<<20))

#define ACFG_SDRAM_ESDCFG_REGISTER_VAL (0x00000000\
	| ((REG_FIELD_RFC_VAL&0x0F)<<28)\
	| (((((2*ACFG_SDRAM_XSR_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-25)&0x0F)<<24)\
	| (((((2*ACFG_SDRAM_EXIT_PWD+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-1)&0x7)<<20)\
	| ((ACFG_SDRAM_W2R_DELAY)<<20)\
	| (((((2*ACFG_SDRAM_ROW_PRECHARGE_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-2)&0x3)<<18)\
	| (((ACFG_SDRAM_TMRD_DELAY-1)&0x3)<<16)\
	| (((((2*ACFG_SDRAM_RAS_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-1)&0x0F)<<12)\
	| (((((2*ACFG_SDRAM_RRD_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-1)&0x3)<<10)\
	| (((ACFG_SDRAM_TWR_DELAY)&0x1)<<7)\
	| (((((2*ACFG_SDRAM_RCD_DELAY+CONFIG_SYS_2XDDRCLK_LGTH-1)\
		/CONFIG_SYS_2XDDRCLK_LGTH)-1)&0x7)<<4)\
	| (((REG_FIELD_RC_VAL)&0x0F)<<0))

#endif /* __CONFIG_H */

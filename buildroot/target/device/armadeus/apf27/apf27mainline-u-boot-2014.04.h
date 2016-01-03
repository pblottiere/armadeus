/*
 *
 * Configuration settings for the Armadeus Project motherboard APF27
 *
 * Copyright (C) 2008-2014 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION	12
#define CONFIG_IDENT_STRING	" apf27 patch 3.20"
#define CONFIG_BOARD_NAME	apf27

/*
 * SoC configurations
 */
#define CONFIG_ARM926EJS		/* this is an ARM926EJS CPU */
#define CONFIG_MX27			/* in a Freescale i.MX27 Chip */
#define CONFIG_MACH_TYPE	1698	/* APF27 */
#define CONFIG_SYS_GENERIC_BOARD

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
#define CONFIG_CMD_MMC
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

#define CONFIG_BOOTDELAY	5
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		__stringify(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," \
			__stringify(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
			" ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs "

#define ACFG_CONSOLE_DEV	ttymxc0
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
	"fdt_addr_r=A1000000\0" \
	"kernel_addr_r=A0000000\0" \
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

#if (ACFG_SDRAM_MBYTE_SYZE == 64) /* micron MT46H16M32LF -6 */
/* micron 64MB */
#define PHYS_SDRAM_1_SIZE			0x04000000 /* 64 MB */
#define PHYS_SDRAM_2_SIZE			0x04000000 /* 64 MB */
#endif

#if (ACFG_SDRAM_MBYTE_SYZE == 128)
/* micron 128MB */
#define PHYS_SDRAM_1_SIZE			0x08000000 /* 128 MB */
#define PHYS_SDRAM_2_SIZE			0x08000000 /* 128 MB */
#endif

#if (ACFG_SDRAM_MBYTE_SYZE == 256)
/* micron 256MB */
#define PHYS_SDRAM_1_SIZE			0x10000000 /* 256 MB */
#define PHYS_SDRAM_2_SIZE			0x10000000 /* 256 MB */
#endif

#endif /* __CONFIG_H */

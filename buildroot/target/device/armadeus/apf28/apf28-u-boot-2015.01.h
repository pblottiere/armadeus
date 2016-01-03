/*
 * Copyright (C) 2011-2014 Eric Jarrige <eric.jarrige@armadeus.org>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_ENV_VERSION	11
#define CONFIG_IDENT_STRING	" apf28 patch 1.5"
#define CONFIG_BOARD_NAME	apf28
#define CONFIG_SYS_GENERIC_BOARD

/*
 * SoC configurations
 */
#define	CONFIG_MX28			/* i.MX28 SoC */
#define	CONFIG_MACH_TYPE	3845	/* APF28 */

/*
 * Enable the call to miscellaneous platform dependent initialization.
 */
#define CONFIG_SYS_NO_FLASH	/* to be define before <config_cmd_default.h>*/

/*
 * Board display option
 */
#define	CONFIG_DISPLAY_CPUINFO
#define	CONFIG_DOS_PARTITION

/*
 * SPL inherited from <configs/mxs.h>
 */

/* Enable CONFIG_SYS_NO_DCDC_BATT_SOURCE the following if you only have a
 * VDD5V source only and no DCDC_BATT source. This includes configurations with
 * and without a supercap on the BATTERY pin as shown in the i.MX28 reference
 * schematic.
 *
 * NOTE: If both a DCDC_BATT and VDD5V source are present, this is not meant to
 * select which one to use. This is only for hardware configurations without
 * DCDC_BATT power source.
 */
/* #define CONFIG_SYS_NO_DCDC_BATT_SOURCE */

/* Enable CONFIG_SYS_NO_VDD5V_SOURCE the following if you only have a DCDC_BATT
 * source only and no VDD5V. This includes configurations with and without a
 * supercap on the BATTERY pin as shown in the i.MX28 reference schematic.
 *
 * NOTE: If both a DCDC_BATT and VDD5V source are present, this is not meant to
 * select which one to use.  This is only for hardware configurations without
 * VDD5V power source.
*/
/* #define CONFIG_SYS_NO_VDD5V_SOURCE */

/* Enable CONFIG_SYS_VBUS_CURRENT_DRAW the following if VDD5V is sourced from
 * VBUS. Limit the current to 100mA
 */
/* #define CONFIG_SYS_VBUS_CURRENT_DRAW */

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
#define	CONFIG_CMD_CACHE
#define	CONFIG_CMD_DATE
#define	CONFIG_CMD_DHCP
#define CONFIG_CMD_DNS
#define	CONFIG_CMD_EXT2
#define	CONFIG_CMD_FAT
#define	CONFIG_CMD_GPIO
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_I2C
#define	CONFIG_CMD_MII
#define	CONFIG_CMD_MMC
#define	CONFIG_CMD_MTDPARTS
#define	CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS
#define	CONFIG_CMD_NFS
#define	CONFIG_CMD_PING
#define	CONFIG_CMD_SETEXPR
#define CONFIG_CMD_SPI
#define	CONFIG_CMD_USB
#define	CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS

#define CONFIG_REGEX			/* Enable regular expression support */

/*
 * Memory configurations
 */
#define	CONFIG_NR_DRAM_BANKS		1		/* 1 bank of DRAM */
#define	PHYS_SDRAM_1			0x40000000	/* Base address */
#define	PHYS_SDRAM_1_SIZE		0x20000000	/* Max 512 MiB RAM */
#define	CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM_1
#define	CONFIG_SYS_TEXT_BASE		0x40000000

/* Environment */
#define CONFIG_ENV_SIZE			0x00020000	/* 128kB  */

/* Environment is in NAND */
#define	CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE	/* 128kB */
#define CONFIG_ENV_SECT_SIZE		(128 * 1024)
#define CONFIG_ENV_RANGE		(512 * 1024)
#define CONFIG_ENV_OFFSET		0x300000
#define CONFIG_ENV_OFFSET_REDUND	\
		(CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE)	/* +512kB */
#define	CONFIG_ENV_OVERWRITE

/*
 * FLASH organization
 */
/* First 1 MiB reservered for mx28 NAND BCB tables */
/* last 2 MiB reservered for 2 copies of U-Boot */
#define	CONFIG_SYS_MONITOR_OFFSET	0x00100000	/* offset in NAND */
#define	CONFIG_SYS_MONITOR_LEN		0x00100000	/* 2 x 1MB */
#define	CONFIG_KERNEL_OFFSET		0x00500000	/* 8MB */
#define	CONFIG_ROOTFS_OFFSET		0x00D00000

#define CONFIG_MTDMAP			"gpmi-nfc-main"
#define	MTDIDS_DEFAULT			"nand0=" CONFIG_MTDMAP
#define	MTDPARTS_DEFAULT			\
	"mtdparts=" CONFIG_MTDMAP			\
		":3M(u-boot)ro,"		\
		"512K(env),"		\
		"512K(env2),"	\
		"512K(dtb),"			\
		"512K(splash),"			\
		"8M(kernel),"			\
		"-(rootfs)"

/*
 * U-Boot general configurations
 */
#define	CONFIG_SYS_PROMPT		"BIOS> "
#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_PREBOOT			"run check_flash check_env;"

/*
 * Boot Linux
 */
#define CONFIG_INITRD_TAG		/* send initrd params	*/

/* #define CONFIG_REVISION_TAG */

#define	CONFIG_BOOTDELAY	5
#define CONFIG_ZERO_BOOTDELAY_CHECK
#define	CONFIG_BOOTFILE		__stringify(CONFIG_BOARD_NAME) "-linux.bin"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," \
				__stringify(CONFIG_BAUDRATE) " " MTDPARTS_DEFAULT \
			" ubi.mtd=rootfs root=ubi0:rootfs rootfstype=ubifs "

#define ACFG_CONSOLE_DEV	ttyAM0
#define	CONFIG_BOOTCOMMAND	"run ubifsboot"
#define CONFIG_SYS_AUTOLOAD	"no"
/*
 * Default load address for user programs and kernel
 */
#define	CONFIG_LOADADDR			0x40000000
#define	CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/*
 * Extra Environments
 */
#define	CONFIG_EXTRA_ENV_SETTINGS					\
	"env_version="		__stringify(CONFIG_ENV_VERSION)		"\0"\
	"consoledev="		__stringify(ACFG_CONSOLE_DEV)		"\0"\
	"mtdparts="		MTDPARTS_DEFAULT			"\0"\
	"partition=nand0,6\0"						\
	"u-boot_addr="		__stringify(CONFIG_SYS_MONITOR_OFFSET)	"\0"\
	"u-boot_len="		__stringify(CONFIG_SYS_MONITOR_LEN)	"\0"\
	"env_addr="		__stringify(CONFIG_ENV_OFFSET)		"\0"\
	"kernel_addr="		__stringify(CONFIG_KERNEL_OFFSET)	"\0"\
	"rootfs_addr="		__stringify(CONFIG_ROOTFS_OFFSET)	"\0"\
	"board_name="		__stringify(CONFIG_BOARD_NAME)		"\0"\
	"kernel_addr_r=40000000\0"					\
	"check_env=if test -n ${flash_env_version}; "			\
		"then env default env_version; "			\
		"else env set flash_env_version ${env_version}; env save; "\
		"fi; "							\
		"if itest ${flash_env_version} != ${env_version}; then " \
			"echo \"*** Warning - Environment version"	\
			" change suggests: run flash_reset_env; reset\"; "\
			"env default flash_reset_env; "			\
		"else exit; fi; \0"					\
	"check_flash= exit\0"						\
	"flash_reset_env=env default -f -a && saveenv &&"		\
		"echo Flash environment variables erased!\0"		\
	"download_uboot=tftpboot ${loadaddr} ${board_name}-u-boot.sb\0"	\
	"flash_uboot=if bcb nand.write ; then "				\
		"nand erase ${u-boot_addr} ${u-boot_len} ; "		\
		"nand write.trimffs ${fileaddr} ${u-boot_addr} ${filesize};" \
		"nand erase ${u-boot2_addr} ${u-boot_len};"		\
		"if nand write.trimffs ${fileaddr} ${u-boot2_addr}"	\
			" ${filesize};"					\
			"then echo Flashing of uboot succeed;"		\
			"else echo Flashing of uboot failed;"		\
		"fi; "							\
		"else echo bcb update failed;;"				\
		"fi; \0"						\
	"update_uboot=run download_uboot flash_uboot\0"			\
	"download_env=tftpboot ${loadaddr} ${board_name}"		\
		"-u-boot-env.txt\0"					\
	"flash_env=env import -t ${loadaddr} && env save; \0"		\
	"update_env=run download_env flash_env\0"			\
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
		"then run update_dtb; else exit; fi;\0"			\
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
 * Serial Driver inherited from <configs/mxs.h>
 */

/*
 * GPIO inherited from <configs/mxs.h>
 */

/*
 * NOR
 */

/*
 * NAND
 */
#define	NAND_MAX_CHIPS			1

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
 * Ethernet (on SOC mx28 FEC)
 */
#ifdef	CONFIG_CMD_NET
#define	CONFIG_FEC_MXC
#define CONFIG_NET_MULTI
#define CONFIG_MX28_FEC_MAC_IN_OCOTP
#define CONFIG_NETCONSOLE		1
#endif
#define ACFG_OUI	0x001EAC

/*
 * Fuses - OCOTP
 */
/* #define CONFIG_IMX_OCOTP */

/*
 * I2C inherited from <configs/mxs.h>
 */

/*
 * SPI
 */
#ifdef CONFIG_CMD_SPI
#define CONFIG_DEFAULT_SPI_BUS		2
#define CONFIG_DEFAULT_SPI_CS		0
#define CONFIG_DEFAULT_SPI_MODE		SPI_MODE_0
#endif

/*
 * SD/MMC inherited from <configs/mxs.h>
 */

/*
 * RTC
 */
#ifdef	CONFIG_CMD_DATE
/* Use the internal RTC in the MXS chip */
#define	CONFIG_RTC_INTERNAL
#define	CONFIG_RTC_MXS
#endif

/*
 * Watchdog Config
 */

/*
 * USB
 */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_MXS_PORT1
#define CONFIG_USB_STORAGE
#endif

/* The rest of the configuration is shared */
#include <configs/mxs.h>

#endif /* __CONFIG_H */

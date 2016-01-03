/*
 * Copyright (C) 2013-2015 Armadeus Systems
 *
 * Configuration settings for the APF6.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
#define DEBUG
*/

#define CONFIG_SYS_GENERIC_BOARD

#define CONFIG_VERSION_VARIABLE
#define CONFIG_ENV_VERSION	21
#define CONFIG_BOARD_NAME	apf6

/* SPL */
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT

#include "imx6_spl.h"
#include "mx6_common.h"

#define CONFIG_MX6
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_MACH_TYPE		4412 /* has to be changed !! */

#include <asm/arch/imx-regs.h>
#include <asm/imx-common/gpio.h>
#include <linux/sizes.h>

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(10 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART4_BASE

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX		1
#define CONFIG_BAUDRATE			115200

/* U-boot commands */
#include <config_cmd_default.h>
#undef CONFIG_CMD_IMLS
#define CONFIG_CMD_BMODE
#define CONFIG_CMD_BOOTZ
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_DNS
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_HDMIDETECT
#define CONFIG_CMD_MII
#define CONFIG_CMD_MMC
#define CONFIG_CMD_NET
#define CONFIG_CMD_PCI
#define CONFIG_CMD_PING
#define CONFIG_CMD_SETEXPR
#define CONFIG_CMD_USB
#define CONFIG_CMD_USB_MASS_STORAGE

#define CONFIG_BOOTDELAY		5
#define CONFIG_ZERO_BOOTDELAY_CHECK

#define CONFIG_SYS_MEMTEST_START	0x10000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 500 * SZ_1M)
#define CONFIG_LOADADDR			0x12000000

/* MMC Configuration */
#define CONFIG_SUPPORT_EMMC_BOOT
#define CONFIG_FSL_ESDHC
#define CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR

#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_BOUNCE_BUFFER
#define CONFIG_DOS_PARTITION

/* Ethernet Configuration */
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RGMII
#define CONFIG_ETHPRIME			"FEC"
#define CONFIG_FEC_MXC_PHYADDR		1
#define CONFIG_NETCONSOLE
#define CONFIG_PHYLIB
#define CONFIG_PHY_ATHEROS

/* Framebuffer */
#define CONFIG_BMP_16BPP
#define CONFIG_CFB_CONSOLE
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP
#define CONFIG_IPUV3_CLK 260000000
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_VIDEO
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_IPUV3
#define CONFIG_VIDEO_LOGO

/* USB Configs */
#define CONFIG_USB_DEVICE
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_SYS_USB_EVENT_POLL_VIA_CONTROL_EP
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_KEYBOARD
/* Gadget part */
#define CONFIG_CI_UDC
#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USB_GADGET_VBUS_DRAW	2
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_G_DNL_VENDOR_NUM		0x0525
#define CONFIG_G_DNL_PRODUCT_NUM	0xa4a5
#define CONFIG_G_DNL_MANUFACTURER	"Armadeus Systems"

/* PCIE */
#define CONFIG_PCI
#define CONFIG_PCI_PNP
#define CONFIG_PCIE_IMX

/* FPGA */
#define CONFIG_FPGA_COUNT	1
#define CONFIG_FPGA
#define CONFIG_FPGA_ALTERA
#define CONFIG_FPGA_CYCLON2

/*
 *  BOOTP options
 */
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2

#define CONFIG_HOSTNAME         CONFIG_BOARD_NAME
#define CONFIG_ROOTPATH         "/tftpboot/" __stringify(CONFIG_BOARD_NAME) "-root"

#define CONFIG_PREBOOT		"run checknload_firmware check_env;"

#define ACFG_CONSOLE_DEV        ttymxc3
#define CONFIG_SYS_AUTOLOAD	"no"
#define CONFIG_BOOTARGS		"console=" __stringify(ACFG_CONSOLE_DEV) "," __stringify(CONFIG_BAUDRATE)
#define CONFIG_BOOTCOMMAND 	"run emmcboot"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"env_version="          __stringify(CONFIG_ENV_VERSION)         "\0"			\
	"consoledev="           __stringify(ACFG_CONSOLE_DEV)           "\0" 			\
	"board_name="           __stringify(CONFIG_BOARD_NAME)          "\0" 			\
	"fdt_addr=0x18000000\0" 								\
	"fdt_high=0xffffffff\0" 								\
	"fdt_name="           __stringify(CONFIG_BOARD_NAME)          "dev\0" 			\
	"initrd_high=0xffffffff\0" 								\
	"ip_dyn=yes\0" 										\
	"stdin=serial\0"									\
	"stout=serial\0"									\
	"sterr=serial\0"									\
	"mmcdev=0\0" 										\
	"mmcpart=1\0" 										\
	"mmcroot=/dev/mmcblk2p2 ro\0" 								\
	"mmcrootfstype=ext4 rootwait\0" 							\
	"firmware_autoload=0\0"									\
	"firmware_path=/lib/firmware/fpga/firmware.periph.rbf\0"				\
	"kernelimg="           __stringify(CONFIG_BOARD_NAME)          "-linux.bin\0" 		\
	"check_env=if test -n ${flash_env_version}; "						\
		"then env default env_version; "						\
		"else env set flash_env_version ${env_version}; env save; "			\
		"fi; "										\
		"if itest ${flash_env_version} != ${env_version}; then "			\
			"echo \"*** Warning - Environment version"				\
			" change suggests: run flash_reset_env; reset\"; "			\
			"env default flash_reset_env; "						\
		"else exit; fi; \0"								\
	"flash_reset_env=env default -f -a && saveenv && "					\
		"echo Flash environment variables erased!\0"					\
	"download_uboot_spl=tftpboot ${loadaddr} ${board_name}-u-boot.spl\0" 			\
	"flash_uboot_spl=" 									\
		"if mmc dev 0 1; then "								\
			"setexpr sz ${filesize} / 0x200; " 					\
			"setexpr sz ${sz} + 1; " 						\
			"if mmc write ${loadaddr} 0x2 ${sz}; then " 				\
				"echo Flashing of U-boot SPL succeed; " 			\
			"else echo Flashing of U-boot SPL failed; "				\
			"fi; "									\
		"fi;\0" 									\
	"download_uboot_img=tftpboot ${loadaddr} ${board_name}-u-boot.img\0" 			\
	"flash_uboot_img=" 									\
		"if mmc dev 0 1; then "								\
			"setexpr sz ${filesize} / 0x200; " 					\
			"setexpr sz ${sz} + 1; " 						\
			"if mmc write ${loadaddr} 0x8a ${sz}; then " 				\
				"echo Flashing of U-boot image succeed; " 			\
			"else echo Flashing of U-boot image failed; " 				\
			"fi; "									\
		"fi;\0" 									\
	"update_uboot=run download_uboot_spl flash_uboot_spl " 					\
		"download_uboot_img flash_uboot_img\0" 						\
	"download_kernel=tftpboot ${loadaddr} ${kernelimg}\0"		 			\
	"flash_kernel=" 									\
		"if ext4write mmc ${mmcdev}:${mmcpart} ${loadaddr} /${kernelimg} ${filesize}; then "	\
		      "echo kernel update in Boot partition succeed; "                      			\
		      "else echo kernel update in Boot partition failed; "                  			\
		"fi;\0"						               			\
	"update_kernel=run download_kernel flash_kernel\0" 					\
	"download_dtb=tftpboot ${fdt_addr} ${cpu_type}-${fdt_name}.dtb\0"			\
	"flash_dtb=" 										\
		"if ext4write mmc ${mmcdev}:${mmcpart} ${fdt_addr} /${cpu_type}-${fdt_name}.dtb ${filesize}; then " \
		      "echo dtb update in Boot partition succeed; "                      	\
		      "else echo dtb update in Boot partition failed; "                  	\
		"fi;\0" 									\
	"update_dtb=run download_dtb flash_dtb\0" 						\
	"download_boot=tftpboot ${loadaddr} ${board_name}-boot.ext4\0"				\
	"flash_boot=" 									\
		"if mmc dev 0 0; then "								\
			"setexpr nbblocks ${filesize} / 0x200; " 				\
			"setexpr nbblocks ${nbblocks} + 1; " 					\
			"if mmc write ${loadaddr} 0x800 ${nbblocks}; then " 			\
				"echo Flashing of boot image succeed; " 			\
			"else echo Flashing of boot image failed; " 				\
			"fi; "									\
		"fi;\0" 									\
	"update_boot=run download_boot flash_boot\0" 					\
	"erase_rootfs=echo Erasing 10 first MB of eMMC rootfs partition: ; "			\
		"mw ${loadaddr} 0 0xa00000 && mmc dev 0 0 && "					\
		"mmc write ${loadaddr} 0x18800 0x5000\0"						\
	"download_rootfs=tftpboot ${loadaddr} ${board_name}-rootfs.ext4\0" 			\
	"flash_rootfs=" 									\
		"if mmc dev 0 0; then "								\
			"setexpr nbblocks ${filesize} / 0x200; " 				\
			"setexpr nbblocks ${nbblocks} + 1; " 					\
			"if mmc write ${loadaddr} 0x18800 ${nbblocks}; then " 			\
				"echo Flashing of rootfs image succeed; " 			\
			"else echo Flashing of rootfs image failed; " 				\
			"fi; "									\
		"fi;\0" 									\
	"update_rootfs=run download_rootfs flash_rootfs\0" 					\
	"download_userdata=tftpboot ${loadaddr} ${board_name}-user_data.ext4\0" 		\
	"flash_userdata=" 									\
		"if mmc dev 0 0; then "								\
			"setexpr nbblocks ${filesize} / 0x200; " 				\
			"setexpr nbblocks ${nbblocks} + 1; " 					\
			"if mmc write ${loadaddr} 0 ${nbblocks}; then " 			\
				"echo Flashing of user_data image succeed; " 			\
			"else echo Flashing of user_data image failed; " 			\
			"fi; "									\
		"fi;\0" 									\
	"update_userdata=run download_userdata flash_userdata; mmc rescan\0"			\
	"erase_userdata="									\
		"if mmc dev 0 0; then "								\
			"echo Erasing eMMC User Data partition, no way out...; "		\
			"mw ${loadaddr} 0 0x200000; "						\
			"mmc write ${loadaddr} 0 0x1000; "					\
			"mmc write ${loadaddr} 0x800 0x1000; "					\
			"mmc write ${loadaddr} 0x18800 0x1000; "					\
		"fi;"										\
		"mmc rescan\0"									\
	"update_all=run update_kernel update_rootfs update_dtb update_uboot\0"			\
	"load_firmware=ext2load mmc 0:2 ${loadaddr} ${firmware_path} && "			\
		"fpga load 0 ${loadaddr} ${filesize}\0"						\
	"checknload_firmware=if test ${firmware_autoload} = 1;"					\
		"then run load_firmware; else exit; fi;\0"					\
	"mmcloadzimage=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${mmcbootdir}/${kernelimg}\0" 	\
	"mmcloadfdt=load mmc ${mmcdev}:${mmcpart} ${fdt_addr} "					\
		"${mmcbootdir}/${mmcdtbdir}/${cpu_type}-${fdt_name}.dtb || "			\
		"echo Please update your eMMC Boot partition\0" 				\
	"initargs=setenv bootargs console=${consoledev},${baudrate} ${extrabootargs}\0"		\
	"addipargs=setenv bootargs ${bootargs} ip=${ipaddr}:${serverip}:"			\
		"${gatewayip}:${netmask}:${hostname}:eth0:off\0"				\
	"addmmcargs=setenv bootargs ${bootargs} root=${mmcroot} " 				\
		"rootfstype=${mmcrootfstype}\0" 						\
	"genmmcboot=run initargs; run addmmcargs; "						\
		"run mmcloadzimage && run mmcloadfdt && bootz ${loadaddr} - ${fdt_addr};\0"	\
	"mmcboot=setenv mmcdev 1; setenv mmcpart 1; setenv mmcbootdir /boot; "			\
		"setenv mmcroot /dev/mmcblk1p1 ro; "						\
		"run genmmcboot;\0"								\
	"emmcboot=setenv mmcdev 0; setenv mmcpart 1; setenv mmcbootdir; "			\
		"setenv mmcroot /dev/mmcblk2p2 ro; "						\
		"run genmmcboot;\0"								\
	"addramargs=setenv bootargs ${bootargs} root=/dev/ram rw\0"				\
	"ramboot=run initargs; run addramargs; "						\
		"run download_kernel && run download_dtb && "					\
		"bootz ${loadaddr} - ${fdt_addr};\0"						\
	"addnfsargs=setenv bootargs ${bootargs} root=/dev/nfs rw "				\
		"nfsroot=${serverip}:${rootpath}\0"						\
	"nfsboot=run initargs; run addnfsargs addipargs; "					\
		"nfs ${loadaddr} ${serverip}:${rootpath}/boot/${kernelimg} && "			\
		"nfs ${fdt_addr} ${serverip}:${rootpath}/boot/${cpu_type}-${fdt_name}.dtb && "	\
		"bootz ${loadaddr} - ${fdt_addr};\0"

/* Miscellaneous configurable options */
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_CBSIZE		384	/* Console I/O Buffer Size */
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT		"BIOS> "

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	       16
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		1
#define CONFIG_ENV_SIZE			(10 * 1024)
#define CONFIG_ENV_OFFSET		(1024 * 1024) /* 1 MB */
#define CONFIG_ENV_OFFSET_REDUND	(1536 * 1024) /* 512KB from CONFIG_ENV_OFFSET */

#define CONFIG_OF_LIBFDT
#define CONFIG_SUPPORT_RAW_INITRD

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

#endif			       /* __CONFIG_H * */

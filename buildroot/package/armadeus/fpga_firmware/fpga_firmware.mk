#############################################################
#
# FPGA firmware
#
#############################################################

FPGA_FIRMWARE_VERSION = 6.0
FPGA_FIRMWARE_SITE = $(TOPDIR)/../firmware
FPGA_FIRMWARE_SITE_METHOD = local

ifeq ($(strip $(BR2_PACKAGE_FPGA_FIRMWARE_APF51_DUAL_UART)),y)
FPGA_FIRMWARE = $(TOPDIR)/../firmware/pod_scripts/apf51_gsm_gps_firmware.bin
endif
ifeq ($(strip $(BR2_PACKAGE_FPGA_FIRMWARE_CUSTOM)),y)
FPGA_FIRMWARE = $(call qstrip,$(BR2_PACKAGE_FPGA_FIRMWARE_CUSTOM_FILE))
endif

FIRMWARES += $(FPGA_FIRMWARE)
FPGA_FIRMWARE_TARGET_DIR = $(TARGET_DIR)/lib/firmware/fpga/

ifeq ($(BR2_PACKAGE_FPGA_FIRMWARE_EXPORT),y)
define FPGA_FIRMWARE_EXPORT
	# Only last file in previous list will remain as symlink
	for firmware in $(FIRMWARES); do \
		ln -sf $$firmware $(BINARIES_DIR)/$(BOARD_NAME)-firmware.bin ; \
	done
endef
endif

define FPGA_FIRMWARE_INSTALL_TARGET_CMDS
	@$(call MESSAGE,"Installing some FPGA firmwares")
	mkdir -p $(FPGA_FIRMWARE_TARGET_DIR)
	for firmware in $(FIRMWARES); do \
		$(INSTALL) -m 0755 $$firmware $(FPGA_FIRMWARE_TARGET_DIR) ; \
	done
	$(FPGA_FIRMWARE_EXPORT)
endef

$(eval $(generic-package))

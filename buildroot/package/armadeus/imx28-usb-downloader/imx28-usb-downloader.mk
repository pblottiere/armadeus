################################################################################
#
# gnet-imx28-usb-downloader
#
################################################################################

IMX28_USB_DOWNLOADER_VERSION = 0.1
IMX28_USB_DOWNLOADER_SITE = $(TOPDIR)/../target/packages/gnet-imx28-usb-downloader/
IMX28_USB_DOWNLOADER_SITE_METHOD = local
IMX28_USB_DOWNLOADER_DEPENDENCIES = libusb-compat

IMX28_USB_DOWNLOADER_BIN_NAME = gnet-imx28-usb-downloader

define IMX28_USB_DOWNLOADER_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D) all
endef

define HOST_IMX28_USB_DOWNLOADER_BUILD_CMDS
	$(MAKE) CFLAGS="$(HOST_CFLAGS)" LDFLAGS="$(HOST_LDFLAGS)" -C $(@D) all
endef

define IMX28_USB_DOWNLOADER_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/$(IMX28_USB_DOWNLOADER_BIN_NAME) \
		$(TARGET_DIR)/usr/bin/$(IMX28_USB_DOWNLOADER_BIN_NAME)
endef

define HOST_IMX28_USB_DOWNLOADER_INSTALL_CMDS
	$(INSTALL) -D -m 0755 $(@D)/$(IMX28_USB_DOWNLOADER_BIN_NAME) \
		$(HOST_DIR)/usr/bin/$(IMX28_USB_DOWNLOADER_BIN_NAME)
endef

$(eval $(generic-package))
$(eval $(host-generic-package))

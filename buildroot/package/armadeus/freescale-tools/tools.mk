#############################################################
#
# Miscellaneous tools from Freescale BSP
#
#############################################################

FREESCALE_TOOLS_VERSION = 20071002
FREESCALE_TOOLS_SOURCE = misc-$(FREESCALE_TOOLS_VERSION).tar.gz
FREESCALE_TOOLS_PATCH = misc-vpu_lib-2.2.2-mx27-2.patch
FREESCALE_TOOLS_SITE = http://www.bitshrine.org/gpp/
FREESCALE_TOOLS_PATCH_SITE = ftp://ftp2.armadeus.com/armadeusw/download/
FREESCALE_TOOLS_INSTALL_STAGING = YES

FREESCALE_TOOLS_TARGET_DIR = $(TARGET_DIR)/usr/bin/vpu

define FREESCALE_TOOLS_BUILD_CMDS
	$(MAKE1) CROSS_COMPILE="$(TARGET_CROSS)" PLATFORM=IMX27ADS -C $(@D)/lib/vpu/
	$(MAKE1) CROSS_COMPILE="$(TARGET_CROSS)" PLATFORM=IMX27ADS \
		TOPDIR=$(@D) \
		OBJDIR="$(FREESCALE_TOOLS_TARGET_DIR)" \
		-C $(@D)/test/mxc_vpu_test/
endef

define FREESCALE_TOOLS_INSTALL_STAGING_CMDS
	cp $(@D)/lib/vpu/*.h $(STAGING_DIR)/usr/include/
	cp $(@D)/lib/vpu/libvpu.a $(STAGING_DIR)/usr/lib/
endef

define FREESCALE_TOOLS_CLEAN_CMDS
	-$(MAKE) -C $(@D)/lib/vpu/ clean
	-$(MAKE) OBJDIR="$(FREESCALE_TOOLS_TARGET_DIR)" \
		 -C $(@D)/test/mxc_vpu_test/ clean
endef

$(eval $(generic-package))

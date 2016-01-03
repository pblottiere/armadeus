#############################################################
#
# fpgaregs debug tool
#
#############################################################

FPGAREGS_VERSION = 2014.06
FPGAREGS_SITE = $(TOPDIR)/../target/linux/debug/fpgaregs/
FPGAREGS_SITE_METHOD = local
FPGAREGS_LICENSE = GPLv2+

ifeq ($(BR2_CPU_NAME),"imx27")
FPGAREGS_BINARY = fpga27regs
else
 ifeq ($(BR2_CPU_NAME),"imx51")
 FPGAREGS_BINARY = fpga51regs
 else
 FPGAREGS_BINARY = fpgaregs
 endif
endif
FPGAREGS_TARGET_BINARY:=usr/bin/fpgaregs

define FPGAREGS_BUILD_CMDS
	$(TARGET_CONFIGURE_OPTS) $(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define FPGAREGS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 $(@D)/$(FPGAREGS_BINARY) $(TARGET_DIR)/usr/bin/fpgaregs
endef

$(eval $(generic-package))

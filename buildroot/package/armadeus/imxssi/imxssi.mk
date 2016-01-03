################################################################################
#
# imxssi
#
################################################################################

IMXSSI_VERSION = 1.01
IMXSSI_SITE = $(TOPDIR)/../target/linux/debug/imxssi/
IMXSSI_SITE_METHOD = local
IMXSSI_LICENSE = GPLv2+
IMXSSI_LICENSE_FILES = COPYING

IMXSSI_DEFINES = -D$(shell echo $(BR2_CPU_NAME) | tr '[:lower:]' '[:upper:]')
IMXSSI_CONF_ENV = CFLAGS="$(TARGET_CFLAGS) $(IMXSSI_DEFINES)"

define IMXSSI_COPY_IMXREGS_SYMLINKS
	rm -f $(@D)/imx*reg* $(@D)/definitions.h
	cp -Lr $(IMXSSI_SITE)/imx*reg* $(IMXSSI_SITE)/definitions.h $(@D)/
endef

IMXSSI_PRE_CONFIGURE_HOOKS += IMXSSI_COPY_IMXREGS_SYMLINKS

$(eval $(autotools-package))

################################################################################
#
# as_devices
#
################################################################################

AS_DEVICES_VERSION = 0.17
AS_DEVICES_SITE = $(TOPDIR)/../target/packages/as_devices
ifeq ($(generic-package), )
AS_DEVICES_SITE_METHOD = copy
else
AS_DEVICES_SITE_METHOD = local
endif
AS_DEVICES_LICENSE = LGPLv2.1+
AS_DEVICES_LICENSE_FILES = COPYING
AS_DEVICES_INSTALL_STAGING = YES

AS_DEVICES_BUILD_TARGET = c_target

ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_CPP)),y)
AS_DEVICES_BUILD_TARGET += cpp_target
endif

ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_PYTHON)),y)
AS_DEVICES_DEPENDENCIES += python
AS_DEVICES_BUILD_TARGET += python_target
endif

AS_DEVICES_MAKE_ENV = CC="$(TARGET_CC)" CXX="$(TARGET_CXX)"
ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_PYTHON)),y)
AS_DEVICES_MAKE_ENV += PYINC="$(STAGING_DIR)/usr/include/python$(PYTHON_VERSION_MAJOR)"
AS_DEVICES_MAKE_ENV += PYLIB="$(STAGING_DIR)/usr/lib/python$(PYTHON_VERSION_MAJOR)"
endif
#XXX: BR2_TARGET_UBOOT_BOARDNAME is wrong, to be fixed
AS_DEVICES_MAKE_ENV += ARMADEUS_BOARD_NAME=$(BR2_TARGET_UBOOT_BOARDNAME)

# Build
define AS_DEVICES_BUILD_CMDS
	$(MAKE) $(AS_DEVICES_MAKE_ENV) -C $(@D) $(AS_DEVICES_BUILD_TARGET)
endef

# Install staging/
ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_CPP)),y)
define AS_DEVICES_INSTALL_STAGING_CPP_LIB
	$(MAKE) INSTALL_DIR=$(STAGING_DIR) -C $(@D)/cpp install
endef
endif

define AS_DEVICES_INSTALL_STAGING_CMDS
	$(MAKE) INSTALL_DIR=$(STAGING_DIR) -C $(@D) install
	$(AS_DEVICES_INSTALL_STAGING_CPP_LIB)
endef

# Install target/
ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_TESTS)),y)
define AS_DEVICES_INSTALL_TEST
	cp -rf $(@D)/test_cdevices $(TARGET_DIR)/usr/bin/
endef
endif

ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_CPP)),y)
define AS_DEVICES_INSTALL_CPP_LIB
	$(MAKE) INSTALL_DIR=$(TARGET_DIR) -C $(@D)/cpp install-exe
endef
endif

ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_TESTS_CPP)),y)
define AS_DEVICES_INSTALL_CPP_TEST
	cp -rf $(@D)/test_cppdevices $(TARGET_DIR)/usr/bin/
endef
endif

ifeq ($(strip $(BR2_PACKAGE_AS_DEVICES_PYTHON)),y)
define AS_DEVICES_INSTALL_PYTHON
	cp -rf $(@D)/python/AsDevices $(TARGET_DIR)/usr/lib/python$(PYTHON_VERSION_MAJOR)/
endef
endif

define AS_DEVICES_INSTALL_TARGET_CMDS
	$(MAKE) INSTALL_DIR=$(TARGET_DIR) \
		-C $(@D) install-exe
	$(AS_DEVICES_INSTALL_TEST)
	$(AS_DEVICES_INSTALL_CPP_LIB)
	$(AS_DEVICES_INSTALL_CPP_TEST)
	$(AS_DEVICES_INSTALL_PYTHON)
endef

ifeq ($(generic-package), )
$(eval $(call GENTARGETS))
else
$(eval $(generic-package))
endif


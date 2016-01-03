#############################################################
#
# Armadeus Demos
#
#############################################################

DEMOS_VERSION = 1.0
DEMOS_SITE = $(TOPDIR)/../target/demos
DEMOS_SITE_METHOD = local

ARMADEUS-DEMOS_TARGET_DIR = $(TARGET_DIR)/usr/local/bin
ARMADEUS-DEMOS_TARGET_DATA_DIR = $(TARGET_DIR)/usr/share
ARMADEUS-DEMOS_TARGET_INIT_DIR = $(TARGET_DIR)/etc/init.d

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_ARMANOID),y)
ARMADEUS-DEMOS_SUBDIRS += armanoid
DEMOS_DEPENDENCIES += sdl
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_BACKLIGHT),y)
ARMADEUS-DEMOS_SUBDIRS += backlight_control
DEMOS_DEPENDENCIES += sdl sdl_image
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_GPIO),y)
ARMADEUS-DEMOS_SUBDIRS += gpio
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_CAPTURE),y)
ARMADEUS-DEMOS_SUBDIRS += camera/capture
DEMOS_DEPENDENCIES += sdl
  ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_CAPTURE_VPU),y)
  ARMADEUS-DEMOS_PARAMS = "VPU=yes"
    ifeq ($(BR2_CPU_NAME),"imx51")
    DEMOS_DEPENDENCIES += imx-lib
    endif
    ifeq ($(BR2_CPU_NAME),"imx27")
    DEMOS_DEPENDENCIES += freescale-tools
    endif
  endif
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_GPS),y)
ARMADEUS-DEMOS_SUBDIRS += gps
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_INPUT),y)
ARMADEUS-DEMOS_SUBDIRS += input_test
DEMOS_DEPENDENCIES += sdl
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_MUSIC_PLAYER),y)
ARMADEUS-DEMOS_SUBDIRS += music_player
DEMOS_DEPENDENCIES += sdl sdl_mixer sdl_ttf
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_OSCILLO),y)
ARMADEUS-DEMOS_SUBDIRS += oscillo
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_SHOW_IMAGE),y)
ARMADEUS-DEMOS_SUBDIRS += show_image
DEMOS_DEPENDENCIES += sdl sdl_image
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_TEST_LCD),y)
ARMADEUS-DEMOS_SUBDIRS += test_lcd
DEMOS_DEPENDENCIES += sdl
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_MCP25020),y)
ARMADEUS-DEMOS_SUBDIRS += mcp25020_ctrl
endif

ifeq ($(BR2_PACKAGE_ARMADEUS_DEMOS_SHM),y)
ARMADEUS-DEMOS_SUBDIRS += shm
endif

define DEMOS_BUILD_CMDS
	@for dir in $(ARMADEUS-DEMOS_SUBDIRS) ; do \
		echo -e "\n   >>> Compiling $$dir demo <<<" ; \
		dir=$(@D)/$$dir ; \
                if [ -d "$$dir" ]; then \
                        (cd $$dir && $(MAKE) CC="$(TARGET_CC)" STAGING_DIR=$(STAGING_DIR) ARMADEUS_BOARD_NAME=$(BR2_BOARD_NAME) $(ARMADEUS-DEMOS_PARAMS)) || exit 1 ; \
                fi \
        done
endef

define DEMOS_INSTALL_TARGET_CMDS
	@for dir in $(ARMADEUS-DEMOS_SUBDIRS) ; do \
		echo -e "\n   >>> Installing $$dir demo <<<" ; \
		dir=$(@D)/$$dir ; \
                if [ -d $$dir ] ; then \
                        (cd $$dir && $(MAKE) install INSTALL_DIR=$(ARMADEUS-DEMOS_TARGET_DIR) \
                               INSTALL_DATA_DIR=$(ARMADEUS-DEMOS_TARGET_DATA_DIR) INIT_DIR=$(ARMADEUS-DEMOS_TARGET_INIT_DIR) STRIP=$(TARGET_STRIP)) || exit 1 ; \
                fi \
        done
endef

$(eval $(generic-package))

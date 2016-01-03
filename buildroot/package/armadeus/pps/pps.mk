#############################################################
#
# PPS Utilities & Test scripts
#
#############################################################

PPS_VERSION = 1.1
PPS_SITE = $(TOPDIR)/../target/pps
ifeq ($(generic-package), )
PPS_SITE_METHOD = copy
else
PPS_SITE_METHOD = local
endif

PPS_CONFIGS_DIR:=$(TOPDIR)/../target/pps/configs
PPS_CONFIGS_TARGET_DIR:=$(TARGET_DIR)/etc/
PPS_INITS_DIR:=$(TOPDIR)/../target/pps/init
PPS_INITS_TARGET_DIR:=$(PPS_CONFIGS_TARGET_DIR)/init.d
PPS_HOME_DIR:=$(TOPDIR)/../target/pps/home

PPS_TESTSCRIPTS_SOURCE:=$(TOPDIR)/../target/pps/scripts
PPS_TESTSCRIPTS_TARGET_DIR:=$(TARGET_DIR)/usr/local/pps/scripts
PPS_TESTSCRIPTS_WWW_DIR:=$(PPS_TESTSCRIPTS_SOURCE)/www
PPS_TESTSCRIPTS_WWW_TARGET_DIR:=$(TARGET_DIR)/var
PPS_TESTSCRIPTS_CONFIGS_DIR:=$(PPS_TESTSCRIPTS_SOURCE)/configs

ifeq ($(BR2_PACKAGE_PPS_TEST_SCRIPTS),y)
define PPS_INSTALL_TEST_SCRIPTS
	$(PPS_TESTSCRIPTS_SOURCE)/install.sh $(PPS_TESTSCRIPTS_TARGET_DIR)
	cp -Rf $(PPS_TESTSCRIPTS_WWW_DIR) $(PPS_TESTSCRIPTS_WWW_TARGET_DIR)
	cp -Rf $(PPS_TESTSCRIPTS_CONFIGS_DIR)/* $(PPS_CONFIGS_TARGET_DIR)
endef
endif

define PPS_INSTALL_TARGET_CMDS
	cp $(PPS_INITS_DIR)/S80gpios_$(BR2_BOARD_NAME) $(PPS_INITS_TARGET_DIR)/S80gpios
	cp -Rf $(PPS_INITS_DIR)/S90* $(PPS_INITS_TARGET_DIR)
	mkdir -p $(TARGET_DIR)/home
	cp -Rf $(PPS_HOME_DIR)/* $(TARGET_DIR)/home
	mkdir -p $(PPS_CONFIGS_TARGET_DIR)
	cp -Rf $(PPS_CONFIGS_DIR)/* $(PPS_CONFIGS_TARGET_DIR)
	$(PPS_INSTALL_TEST_SCRIPTS)
endef

define PPS_UNINSTALL_TARGET_CMDS
	rm -rf $(PPS_TESTSCRIPTS_TARGET_DIR)
	rm -rf $(PPS_WWW_TARGET_DIR)/www
endef

ifeq ($(generic-package), )
$(eval $(call GENTARGETS))
else
$(eval $(generic-package))
endif


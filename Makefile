# Makefile for Armadeus
#
# Copyright (C) 2005-2014 by the Armadeus Team
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#

#--------------------------------------------------------------
# Just run 'make menuconfig', configure stuff, then run 'make'.
# You shouldn't need to mess with anything beyond this point...
#--------------------------------------------------------------

ARMADEUS_TOPDIR:=$(shell pwd)
export ARMADEUS_TOPDIR

#--- User configurable stuff:
#BUILDROOT_VERSION=2013.11
BUILDROOT_VERSION=2015.02
include ./Makefile.in
BUILDROOT_SITE:=http://buildroot.uclibc.org/downloads
BUILDROOT_PATCH_DIR:=$(ARMADEUS_TOPDIR)/patches/buildroot/$(BUILDROOT_VERSION)
ARMADEUS_CONFIG_DIR:=$(BUILDROOT_DIR)/configs
#--- End of user conf (don't touch anything below unless you know what you're doing !! ;-) )

BUILDROOT_SOURCE:=buildroot-$(BUILDROOT_VERSION).tar.bz2
#BUILDROOT_DIR is defined in ./Makefile.in
PATCH_DIR = patches
ifeq ($(ARMADEUS_DL_DIR),)
BUILDROOT_FILE_PATH = $(BUILDROOT_DIR)/downloads
else
BUILDROOT_FILE_PATH = $(ARMADEUS_DL_DIR)
endif
TAR_OPTIONS = --exclude=.svn --exclude=.git --exclude=.gitignore --strip-component=1 -xf
BUILDROOT_PATCH_SCRIPT = $(BUILDROOT_DIR)/support/scripts/apply-patches.sh
ARMADEUS_ENV_FILE:=$(ARMADEUS_TOPDIR)/armadeus_env.sh
WGET = wget --passive-ftp --tries=3
ARMADEUS_FTP_SITE = ftp://ftp2.armadeus.com/armadeusw/download/

ECHO_CONFIGURATION_NOT_DEFINED:= echo -en "\033[1m"; \
                echo "                                                   "; \
                echo " System not configured. Use make <board>_defconfig " >&2; \
                echo " armadeus valid configurations are:                " >&2; \
                echo "     "$(shell find $(ARMADEUS_CONFIG_DIR) -name "[ap][fp][fs]*_defconfig" | sed 's/.*\///');\
                echo "                                                   "; \
		echo -en "\033[0m";

ARMADEUS_BSP_VERSION=$(shell cat $(BUILDROOT_DIR)/target/device/armadeus/rootfs/target_skeleton/etc/armadeus-version)


default: all

help:
	@echo
	@echo 'First, choose your platform and adjust configuration:'
	@echo '  apf9328_defconfig    - get default config for an APF9328+Dev kit'
	@echo '  apf27_defconfig      - get default config for an APF27+Dev kit'
	@echo '  pps_defconfig        - get default config for an APF27+PPS system'
	@echo '  apf51_defconfig      - get default config for an APF51+Dev kit'
	@echo '  pps51_defconfig      - get default config for an APF51+PPS51 system'
	@echo '  apf28_defconfig      - get default config for an APF28+Dev kit'
	@echo '  apf6_defconfig       - get default config for an APF6+Dev kit (3.19+ mainline/vanilla kernel)'
	@echo '  apf6legacy_defconfig - get default config for an APF6+Dev kit (3.10 Wandboard kernel)'
	@echo ''
	@echo 'Build:'
	@echo '  all                - make world '
	@echo '  <package>          - a single package (ex: uboot, linux, sdl, etc...)'
	@echo
	@echo 'Cleaning:'
	@echo '  clean              - delete files created by build'
	@echo '  distclean          - delete all non-source files (including .config)'
	@echo
	@echo 'Configuration:'
	@echo ' Global:'
	@echo '  menuconfig           - interactive curses-based configurator'
	@echo ' Specific (after first build was successful):'
	@echo '  linux-menuconfig     - configure Linux parameters'
	@echo '  uclibc-menuconfig    - configure uClibc library parameters'
	@echo '  busybox-menuconfig   - configure Busybox parameters'
	@echo
	@echo 'Miscellaneous:'
	@echo '  source                 - download all sources needed for offline-build'
	@echo '  source-check           - check all packages for valid download URLs'
	@echo
	@echo 'See http://www.armadeus.org for further details'
	@echo 'See http://www.buildroot.net/downloads/buildroot.html for'
	@echo 'further Buildroot details'
	@echo


# configuration
# ---------------------------------------------------------------------------

$(BUILDROOT_FILE_PATH)/$(BUILDROOT_SOURCE):
	mkdir -p $(BUILDROOT_FILE_PATH)
	$(WGET) -P $(BUILDROOT_FILE_PATH) $(BUILDROOT_SITE)/$(BUILDROOT_SOURCE) \
		|| $(WGET) -P $(BUILDROOT_FILE_PATH) $(ARMADEUS_FTP_SITE)/$(BUILDROOT_SOURCE)

buildroot-sources: $(BUILDROOT_FILE_PATH)/$(BUILDROOT_SOURCE)

$(BUILDROOT_DIR)/.unpacked: $(BUILDROOT_FILE_PATH)/$(BUILDROOT_SOURCE) $(BUILDROOT_PATCH_DIR)/*.patch
	$(BUILDROOT_PATCH_DIR)/cleanup_buildroot.sh
	bzcat $(BUILDROOT_FILE_PATH)/$(BUILDROOT_SOURCE) | \
		tar -C $(BUILDROOT_DIR) $(TAR_OPTIONS) -
	touch $@

buildroot-unpacked: $(BUILDROOT_DIR)/.unpacked

$(BUILDROOT_DIR)/.patched: $(BUILDROOT_DIR)/.unpacked
	$(BUILDROOT_PATCH_SCRIPT) $(BUILDROOT_DIR) $(BUILDROOT_PATCH_DIR) \*.patch
	touch $@

buildroot-patched: $(BUILDROOT_DIR)/.patched

$(BUILDROOT_DIR)/.configured: $(BUILDROOT_DIR)/.patched
	@if [ ! -f $@ ]; then \
		$(ECHO_CONFIGURATION_NOT_DEFINED) \
		exit 1; \
	fi

# To be called only one time if one wants to make an automatic build
buildauto: $(BUILDROOT_DIR)/.patched
	# ! Be sure that /local/downloads exists if you want to use automated build !
	BUILDROOT_DL_DIR=/local/downloads BR2_DL_DIR=/local/downloads $(MAKE) -s -C $(BUILDROOT_DIR)

linux: $(BUILDROOT_DIR)/.configured
	@$(MAKE) -C $(BUILDROOT_DIR) linux-rebuild

linux-clean: $(BUILDROOT_DIR)/.configured
	@$(MAKE) -C $(BUILDROOT_DIR) linux-clean

%_defconfig: $(BUILDROOT_DIR)/.patched
	@if [ -e "$(ARMADEUS_CONFIG_DIR)/$@" ] || [ -e "$(ARMADEUS_CONFIG_DIR)/$(patsubst %_defconfig,%,$@)/$@" ]; then \
		rm -f $(BUILDROOT_DIR)/.config ; \
		$(MAKE) -C $(BUILDROOT_DIR) $@ ; \
		$(MAKE) -C $(BUILDROOT_DIR) menuconfig ; \
		touch $(BUILDROOT_DIR)/.configured ; \
	else \
		echo -e "\033[1m\nThis configuration does not exist !!\n\033[0m" ; \
	fi;

# for the one who doesn't want config menu:
%_autoconf: $(BUILDROOT_DIR)/.patched
	@if [ -e "$(BUILDROOT_DIR)/configs/$(patsubst %_autoconf,%,$@)_defconfig" ]; then \
		rm -f $(BUILDROOT_DIR)/.config ; \
		$(MAKE) -C $(BUILDROOT_DIR) $(patsubst %_autoconf,%,$@)_defconfig ; \
		touch $(BUILDROOT_DIR)/.configured ; \
	else \
		echo -e "\033[1m\nThis configuration does not exist !!\n\033[0m" ; \
	fi;

all: $(BUILDROOT_DIR)/.configured
	@echo -n `date +%s` > before.txt
	@$(MAKE) -C $(BUILDROOT_DIR) $@
	@./scripts/show_build_infos.sh
	@echo -n `date +%s` > after.txt
	@BEFORE=`cat before.txt`; AFTER=`cat after.txt`; BUILD_TIME_IN_SEC=`expr $$AFTER - $$BEFORE`; echo -e "\033[1mBuild time: $$BUILD_TIME_IN_SEC seconds\033[0m"
	@rm before.txt after.txt

menuconfig: $(BUILDROOT_DIR)/.configured
	@$(MAKE) -C $(BUILDROOT_DIR) $@

# !! .DEFAULT do NOT handle dependencies !!
.DEFAULT:
	@if [ ! -e "$(BUILDROOT_DIR)/.configured" ]; then \
		$(ECHO_CONFIGURATION_NOT_DEFINED) \
		exit 1; \
	fi
	@$(MAKE) -C $(BUILDROOT_DIR) $@

clean:
	@echo "Are you sure you want to delete all generated files (toolchain, kernel & Co) ?"
	@echo " Ctrl+c to stop now"
	@read answer
	rm -rf $(BUILDROOT_DIR)/output
	rm -f $(ARMADEUS_ENV_FILE)

buildroot-dirclean:
	rm -rf $(BUILDROOT_DIR)	

# Generate shell's most useful variables:
shell_env:
	@echo "# Automatically generated file !" > $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BUILDROOT_DIR=$(BUILDROOT_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BUILDROOT_VERSION=$(BUILDROOT_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BUILDROOT_PATCH_DIR=$(BUILDROOT_PATCH_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LINUX_DIR=$(ARMADEUS_LINUX_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LINUX_PATCH_DIR=$(ARMADEUS_LINUX_PATCH_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LINUX_CONFIG=$(ARMADEUS_LINUX_CONFIG) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LINUX_VERSION=$(ARMADEUS_LINUX_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LINUX_MAIN_VERSION=$(ARMADEUS_LINUX_MAIN_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_U_BOOT_DIR=$(ARMADEUS_U_BOOT_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_U_BOOT_VERSION=$(ARMADEUS_U_BOOT_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_U_BOOT_CUSTOM_VERSION=$(ARMADEUS_U_BOOT_CUSTOM_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_U_BOOT_CONFIG=$(ARMADEUS_U_BOOT_CONFIG) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_ROOTFS_DIR=$(ARMADEUS_ROOTFS_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BINARIES=$(ARMADEUS_BINARIES) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_ROOTFS_TAR=$(ARMADEUS_ROOTFS_TAR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_STAGING_DIR=$(ARMADEUS_STAGING_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_TOOLCHAIN_PATH=$(ARMADEUS_TOOLCHAIN_PATH) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_QT_DIR=$(ARMADEUS_QT_DIR) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_GCC_VERSION=$(ARMADEUS_GCC_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LIBC_NAME=$(ARMADEUS_LIBC_NAME) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_LIBC_VERSION=$(ARMADEUS_LIBC_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BUSYBOX_VERSION=$(ARMADEUS_BUSYBOX_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BSP_VERSION=$(ARMADEUS_BSP_VERSION) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_BOARD_NAME=$(ARMADEUS_BOARD_NAME) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_U_BOOT_BOARD_NAME=$(ARMADEUS_U_BOOT_BOARD_NAME) >> $(ARMADEUS_ENV_FILE)
	@echo ARMADEUS_DL_DIR=$(ARMADEUS_DL_DIR) >> $(ARMADEUS_ENV_FILE)

PHONY_TARGETS+=dummy all linux linux-clean buildroot-clean buildroot-dirclean
PHONY_TARGETS+=menuconfig $(BUILDROOT_DIR)/.config shell_env buildroot-sources buildroot-unpacked
PHONY_TARGETS+=buildroot-patched 
.PHONY: $(PHONY_TARGETS)


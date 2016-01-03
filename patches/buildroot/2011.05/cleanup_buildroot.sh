#!/bin/bash
#
# Things to do before patching Buildroot
#

# Removes packages/files added through patches:
rm -rf buildroot/package/games/quake-pak/
rm -rf buildroot/package/games/sdlquake/
rm -rf buildroot/package/gpm/
rm -rf buildroot/package/python-pygame/
rm -rf buildroot/package/uucp/
rm -f  buildroot/package/multimedia/libsndfile/libsndfile-1.0.*-srconly.patch
rm -rf buildroot/package/multimedia/flite/
rm -rf buildroot/package/libcurl/
rm -f  buildroot/package/multimedia/mplayer/mplayer-1.0rc2-add_pld_asm_inst_for_non_armv5_arch.patch
rm -f  buildroot/package/multimedia/madplay/madplay-audio-alsa.patch
rm -rf buildroot/package/firmware/
rm -f  buildroot/toolchain/gcc/4.2.1/999-4.2.1-armeabi-aapcs-linux.patch
rm -f  buildroot/toolchain/gcc/4.2.4/999-4.2.4-armeabi-aapcs-linux.patch
rm -rf buildroot/package/games/wolf4sdl/
rm -rf buildroot/package/ussp-push/
rm -rf buildroot/package/openobex/
rm -rf buildroot/package/lmbench/
rm -rf buildroot/package/tslib/
rm -rf buildroot/package/openssh/
rm -rf buildroot/package/sdl/
rm -rf buildroot/package/sdlwater/
rm -rf buildroot/package/slideshow/
rm -rf buildroot/package/pymysql/
rm -rf buildroot/package/socketcan/
rm -rf buildroot/package/lcd4linux/
rm -rf buildroot/package/mesa/
rm -rf buildroot/package/games/snes9x/
rm -rf buildroot/package/allegro/
rm -rf buildroot/package/fbgrab/
rm -rf buildroot/package/fbtest/
rm -rf buildroot/package/xenomai/
rm -rf buildroot/package/cwiid/
rm -rf buildroot/package/urg/
rm -rf buildroot/package/pywebradio/
rm -rf buildroot/package/e-uae/
rm -rf buildroot/package/opentyrian/
rm -rf buildroot/package/ipsec-tools/
rm -rf buildroot/package/lbreakout2/
rm -rf buildroot/package/ltris/
rm -rf buildroot/package/qwt/
rm -rf buildroot/fs/ubi/
rm -f buildroot/package/flex/flex-2.5.35-undefine_rpl_malloc_and_realloc.patch
rm -f buildroot/package/ncurses/ncurses-5.7-allows_build_with_old_ncurses_host_installation.patch
rm -rf buildroot/package/mysql_client/
rm -rf buildroot/package/portmap/
rm -rf buildroot/package/bluez3/
rm -rf buildroot/package/ngircd/
rm -rf buildroot/package/can-utils/
rm -f buildroot/toolchain/uClibc/*.patch
rm -f buildroot/package/ntp/ntp-nano.patch
rm -f buildroot/package/busybox/busybox-1.16.2/*.patch
rm -f buildroot/target/generic/Config.in
rm -rf buildroot/package/libmodbus
rm -rf buildroot/package/nanocom
rm -rf buildroot/package/elftosb
rm -rf buildroot/package/rt-tests
rm -rf buildroot/package/libcanfestival
rm -rf buildroot/package/python
rm -rf buildroot/package/gsmmux
rm -rf buildroot/package/imx-lib/
rm -rf buildroot/package/imx-test/

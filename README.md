# ARMADEUS-6.0

## Debian Jessie

After some updates about downloading links (in the Buildroot part), the
compilation is straightforward and images are successfully generated thanks to:

    make apf28_defconfig

I have tested my armadeus APF28 card with :

  * the uboot image **apf28-u-boot.sb**
  * the kernel image **apf28-linux.bin**
  * the UBIFS **apf28-rootfs.ubi**

## Debian Stretch

Due to gcc 5.x, several errors raised during the compilation. I did not have
time to dig up at the moment...

For example:

    In file included from ../../gcc/cp/except.c:990:0:
    cfns.gperf: At top level:
    cfns.gperf:101:1: error: ‘gnu_inline’ attribute present on ‘libc_name_p’
    cfns.gperf:26:14: error: but not here
    ../../gcc/cp/except.c: In function ‘nothrow_spec_p’:
    ../../gcc/cp/except.c:1254:224: warning: ISO C does not support ‘__FUNCTION__’ predefined identifier [-Wpedantic]
    ../../gcc/cp/except.c:1262:5: warning: ISO C does not support ‘__FUNCTION__’ predefined identifier [-Wpedantic]
    gcc_assert (processing_template_decl)

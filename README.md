# ARMADEUS-6.0

## Debian Jessie

After some updates about downloading links (in the Buildroot part), the
compilation is straightforward and images are successfully generated thanks to:

    > make apf28_defconfig

I have tested my armadeus APF28 card with :

  * the uboot image **apf28-u-boot.sb**
  * the kernel image **apf28-linux.bin**
  * the UBIFS **apf28-rootfs.ubi**

## Debian Stretch

Due to gcc 5.x, several errors raised during the compilation. I did not have
time to dig up at the moment...

After a patch to the cross compiler, the toolchain is well generated :

    > make toolchain

However, some errors remains to fully generate all images :

    > make
    ...
    ...
    ...
    _11918.c:835:22: error: expected ‘)’ before ‘int’
    Makefile:795: recipe for target '../obj_s/lib_gen.o' failed
    make[3]: *** [../obj_s/lib_gen.o] Error 1
    make[3]: *** Waiting for unfinished jobs....
    make[3]: Leaving directory '/home/tergeist/devel/packages/armadeus/buildroot/output/build/host-ncurses-5.9/ncurses'
    Makefile:307: recipe for target '../lib/libncurses.so' failed
    make[2]: *** [../lib/libncurses.so] Error 2
    make[2]: Leaving directory '/home/tergeist/devel/packages/armadeus/buildroot/output/build/host-ncurses-5.9/progs'
    package/pkg-generic.mk:182: recipe for target '/home/tergeist/devel/packages/armadeus/buildroot/output/build/host-ncurses-5.9/.stamp_built' failed
    make[1]: *** [/home/tergeist/devel/packages/armadeus/buildroot/output/build/host-ncurses-5.9/.stamp_built] Error 2
    make[1]: Leaving directory '/home/tergeist/devel/packages/armadeus/buildroot'
    Makefile:164: recipe for target 'all' failed
    make: *** [all] Error 2

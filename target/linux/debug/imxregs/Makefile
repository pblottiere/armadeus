ARMADEUS_BASE_DIR=../../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in


CC = $(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
EXECS = imxlregs imx27regs imx28regs imx51regs imx53regs imx6regs

all: $(EXECS)

imxlregs: imxregs.c imxregs.h definitions.h
	$(CC) imxregs.c -o $@

imx27regs: imxregs.c imx27regs.h definitions.h
	$(CC) -DIMX27 imxregs.c -o $@

imx28regs: imxregs.c imx28regs.h definitions.h
	$(CC) -DIMX28 imxregs.c -o $@

imx51regs: imxregs.c imx5regs.h definitions.h
	$(CC) -DIMX51 imxregs.c -o $@

imx53regs: imxregs.c imx5regs.h definitions.h
	$(CC) -DIMX53 imxregs.c -o $@

imx6regs: imxregs.c imx6regs.h definitions.h
	$(CC) -DIMX6 imxregs.c -o $@

clean:
	rm -f *.o
	rm -f $(EXECS)

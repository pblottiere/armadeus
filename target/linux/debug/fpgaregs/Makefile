ARMADEUS_BASE_DIR=../../../..
-include $(ARMADEUS_BASE_DIR)/Makefile.in

ifeq ($(CC),cc)
        CC=$(ARMADEUS_TOOLCHAIN_PATH)/$(ARMADEUS_TOOLCHAIN_PREFIX)gcc
endif

all: fpgaregs fpga27regs fpga51regs

fpgaregs: fpgaregs.c
	$(CC) -Wall fpgaregs.c -o $@

fpga27regs: fpgaregs.c
	$(CC) -DIMX27 -Wall fpgaregs.c -o $@
    
fpga51regs: fpgaregs.c
	$(CC) -DIMX51 -Wall fpgaregs.c -o $@

clean:
	rm -f *.o
	rm -f fpgaregs
	rm -f fpga27regs
	rm -f fpga51regs

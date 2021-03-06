CROSS_COMPILE      ?= riscv64-unknown-elf-gcc

CFLAGS             =  -Os -march=rv32imac -mabi=ilp32  -mcmodel=medany -ffunction-sections -fdata-sections 
LDFLAGS            = -nostartfiles -nostdlib -nostdinc -static -lgcc \
                     -Wl,--nmagic -Wl,--gc-sections -T default.lds

hello: crtm.o startup.o port.o timer.s
	$(CROSS_COMPILE) $(CFLAGS) $(LDFLAGS) $^ -o $@

crtm.o:crtm.s
	$(CROSS_COMPILE) $(CFLAGS) -c $^ -o $@

startup.o:startup.c
	$(CROSS_COMPILE) $(CFLAGS) -c $^ -o $@

port.o:port.s
	$(CROSS_COMPILE) $(CFLAGS) -c $^ -o $@

timer.o:timer.s
	$(CROSS_COMPILE) $(CFLAGS) -c $^ -o $@

clean:
	rm *.o
	rm hello

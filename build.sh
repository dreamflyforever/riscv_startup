riscv64-unknown-elf-gcc -Os -march=rv64imac -mabi=lp64  -mcmodel=medany -ffunction-sections -fdata-sections  -I. -c crtm.s -o  crtm.o
riscv64-unknown-elf-gcc -Os -march=rv64imac -mabi=lp64  -mcmodel=medany -ffunction-sections -fdata-sections  -I. -c startup.c -o startup.o

riscv64-unknown-elf-gcc -Os -march=rv64imac -mabi=lp64  -I. -mcmodel=medany -ffunction-sections -fdata-sections  -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections -T default.lds crtm.o startup.o -o hello

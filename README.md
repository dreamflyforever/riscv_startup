## intro
This project is about port RTOS to risc-v(rv32i), including thread switch, interrupt handle,
setion of bss, text & data initialize, CPU initializes & uart driver, make sure you can output
printf.

## quick start
Play `./go.sh` to compile the project, and play `./t.sh` to trace the it. When you enter debug mode
interface, `disassemble` is to see where PC is, `si` is step go, so you can debug very well. Use `c`
is continue to run from the point of interrupt.

## author
MIT by Jim

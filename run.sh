DEBUG="-gdb tcp::1235 -S"
qemu-system-arm -M vexpress-a9 -m 256 -kernel kernel.elf -nographic ${DEBUG}

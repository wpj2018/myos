DEBUG="-gdb tcp::1235 -S"
if [ $# == 0 ]; then
	DEBUG=""
fi
qemu-system-arm -M vexpress-a9 -m 256 -kernel kernel.elf -nographic ${DEBUG}

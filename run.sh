DEBUG="-gdb tcp::1235 -S"
if [ $# == 0 ]; then
	DEBUG=""
fi
qemu-system-arm -machine vexpress-a9 -m 1024M -kernel loader/bootimage -serial stdio ${DEBUG}

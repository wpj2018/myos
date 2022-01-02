all:
	gcc gen_verify_file.c -o gen_verify_file
	./gen_verify_file
	dd if=/dev/zero of=ramdisk bs=1M count=40
	mkfs.ext4 -b 4096 -F ramdisk
	sudo mkdir rootfs
	sudo mount -t ext4 ramdisk rootfs
	sudo cp verify.bin rootfs
	sudo cp ../uapps/init.elf rootfs
	sudo umount rootfs
	rm -rf rootfs
clean:
	rm -rf ramdisk gen_verify_file verify.bin

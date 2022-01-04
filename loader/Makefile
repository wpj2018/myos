TARGET := bootimage
LDFLAGS += -T loader.ld.s
SEC_FLAG := --add-section .archive=kernel --set-section-flags .archive=alloc
SEC_FLAG += --add-section .rootfs=rootfs --set-section-flags .rootfs=alloc

SOURCE = $(wildcard *.S *.c)

PREREQUISITE = kernel rootfs
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : ${PREREQUISITE} ${OBJS}
	${OBJCOPY} ${SEC_FLAG} main.o
	${LD} ${OBJS} -o $@ ${LDFLAGS}

kernel:
	${OBJCOPY} -O binary ../kernel/kernel.elf $@
rootfs:
	cp ../rootfs/ramdisk $@

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${PREREQUISITE} ${TARGET} ${OBJS}

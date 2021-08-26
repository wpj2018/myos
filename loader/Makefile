include ../toolchain.mk

TARGET := bootimage
CFLAGS := -g -I../include
LDFLAGS := -T loader.ld.s -lc -L../lib
SEC_FLAG := --add-section .archive=kernel.bin --set-section-flags .archive=alloc

SOURCE = $(wildcard *.S *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : kernel.bin ${OBJS}
	${OBJCOPY} ${SEC_FLAG} main.o
	${LD} ${OBJS} -o $@ ${LDFLAGS}

kernel.bin:
	${OBJCOPY} -O binary ../kernel/kernel.elf $@

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf kernel.bin ${TARGET} ${OBJS}

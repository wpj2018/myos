TARGET := kernel.elf
CROSS_COMPILER := arm-linux-gnueabi-
CC := ${CROSS_COMPILER}gcc
LD := ${CROSS_COMPILER}ld
CFLAGS := -g -Iinclude
LDFLAGS := -T myos.ld.s

SOURCE = $(wildcard kernel/*.* lib/*.c)
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : ${OBJS}
	${LD} ${LDFLAGS} $^ -o $@
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

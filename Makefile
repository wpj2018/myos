TARGET := kernel.elf
CROSS_COMPILER := arm-linux-gnueabi-
CC := ${CROSS_COMPILER}gcc
LD := ${CROSS_COMPILER}ld
OBJS := start.o main.o
${TARGET} : ${OBJS}
	${LD} -T myos.ld.s -Ttext 0x60000000 $^ -o $@
%.o : %.c
	${CC} -g -c $< -o $@
%.o : %.S
	${CC} -g -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

include ../toolchain.mk

TARGET := kernel.elf
CFLAGS := -g -I../include -fno-stack-protector
LDFLAGS := -T kernel.ld.s -ltest -L../tests -lc -L../lib/

SOURCE = $(wildcard *.c *.S ../driver/*.c)
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : ${OBJS}
	${LD} $^ -o $@ ${LDFLAGS}
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

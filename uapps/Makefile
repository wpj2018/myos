include ../toolchain.mk

TARGET := init.elf
CFLAGS := -g -I../include -fno-stack-protector
LDFLAGS := -T uapps.ld.s -lc -L../lib/

SOURCE = $(wildcard *.c *.S)
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : ${OBJS}
	${LD} $^ -o $@ ${LDFLAGS}
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

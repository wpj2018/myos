include ../toolchain.mk

TARGET := libc.a
CFLAGS := -g -I../include

SOURCE = $(wildcard *.c *.S)
OBJS = $(patsubst %.c,%.o,$(SOURCE:.S=.o))

${TARGET} : ${OBJS}
	${AR} -rcs $@ $^
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

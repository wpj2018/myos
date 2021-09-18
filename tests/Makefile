include ../toolchain.mk

TARGET := libtest.a
CFLAGS := -g -I../include -fno-stack-protector

SOURCE = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SOURCE))

${TARGET} : ${OBJS}
	${AR} -rcs $@ $^
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

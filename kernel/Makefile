include ../toolchain.mk

TARGET := kernel.elf
CFLAGS := -g -I../include -fno-stack-protector
LDFLAGS := -T kernel.ld.s -ltest -L../tests -lc -L../lib/

DIRS = $(shell find . -type d)
DIRS += ../driver

SRCS_C += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))
SRCS_S += $(foreach dir, $(DIRS), $(wildcard $(dir)/*.S))

OBJS_C = $(patsubst %.c, %.o, $(SRCS_C))
OBJS_S = $(patsubst %.S, %.o, $(SRCS_S))

OBJS := ${OBJS_C} ${OBJS_S}

${TARGET} : ${OBJS}
	${LD} $^ -o $@ ${LDFLAGS}
%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@
%.o : %.S
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -rf ${TARGET} ${OBJS}

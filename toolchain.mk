DIR=${CURDIR}
export CROSS_COMPILER := arm-linux-gnueabi-
export CC := ${CROSS_COMPILER}gcc
export LD := ${CROSS_COMPILER}ld
export OBJCOPY := ${CROSS_COMPILER}objcopy

export CFLAGS := -g -I${DIR}/include -fno-stack-protector -fno-builtin
export LDFLAGS := -lc -L${DIR}/lib/

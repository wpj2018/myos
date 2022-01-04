DIRS := init tests

all:
	for dir in ${DIRS}; do				\
		cp Makefile.base $$dir/Makefile;	\
		make -C $$dir;				\
	done

clean:
	for dir in ${DIRS}; do		\
		make -C $$dir clean;	\
		rm -rf $$dir/Makefile;	\
	done

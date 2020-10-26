BINARIES=libtest.a

OBJS=ctest.o

all: $(BINARIES)

libtest.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	cp -RL include/. $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(BINARIES)
	cp -RL $(BINARIES) $(SYSROOT)$(USR_LIB_DIR)


clean: 
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
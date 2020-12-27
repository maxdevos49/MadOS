BINARIES=libtest.a

OBJS=ctest.o

all: $(BINARIES)

libtest.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.s: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -S -masm=intel -fverbose-asm $< -o $@

.PHONY: install install-headers install-bin clean
install: install-headers install-bin

install-headers:
	rsync -avm --include='*.h' -f 'hide,! */' .  $(SYSROOT)$(USR_INCLUDE_DIR)

install-bin: $(BINARIES)
	cp -RL $(BINARIES) $(SYSROOT)$(USR_LIB_DIR)

debug: $(OBJS:.o=.s)

clean: 
	@find . -type f -name "*.o" -delete -print
	@find . -type f -name "*.d" -delete -print
	@find . -type f -name "*.a" -delete -print
	@find . -type f -name "*.s" -delete -print
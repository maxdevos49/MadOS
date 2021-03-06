
TOOLS=\
	build_initrd\

all: $(TOOLS)

%.bin: %.c
	gcc -Wextra -Wall -c $< -o $@

.PHONY: clean all

clean:
	rm -f $(TOOLS)
# ###################################################################################
# #-----------------------------Kernel Makefile options-----------------------------#
# ###################################################################################

# # Do not edit below
# ###################################################################################
# KERNEL_LINKER_LIST=
# include kernel/arch/$(ARCH_DIR)/$(ARCH_NAME).Makefile
# ###################################################################################
# # Do not edit above


# KERNEL_OBJS= \
# 	kernel/kernel.o \
# 	kernel/heap/init_heap.o\
# 	kernel/heap/k_malloc.o\
# 	kernel/heap/k_calloc.o\
# 	kernel/heap/k_realloc.o\
# 	kernel/heap/k_free.o\

# KERNEL_CFLAGS:= -Ttext 0x8000 -mno-red-zone $(CFLAGS)  
# KERNEL_CPPFLAGS:=$(CPPFLAGS) -D__$(ARCH_NAME) -D__is_kernel -Iinclude  
# KERNEL_LIBS:=$(LIBS) -nostdlib -lk -lgcc

# # Do not edit below
# ###################################################################################

# KERNEL_SRC_DIR=kernel
# KERNEL_LINKER_LIST+=$(addprefix $(KERNEL_SRC_DIR)/, $(KERNEL_OBJS))

# KERNEL_LINKER_LIST:=$(KERNEL_LINKER_LIST:.o=.k.o)
# KERNEL_DEPS:=$(KERNEL_LINKER_LIST:.o=.d)

# -include $(KERNEL_DEPS)
# -include echo $(KERNEL_DEPS)

# # Links MadOS.kernel
# $(KERNEL): $(KERNEL_LINKER_LIST)
# 	$(CC) $(KERNEL_CFLAGS)  $(KERNEL_CPPFLAGS) -T"kernel/arch/$(ARCH_DIR)/linker.ld" $(KERNEL_LINKER_LIST) $(KERNEL_LIBS) -o kernel/$(KERNEL)

# # Build a c file into a object file
# %.k.o: %.c
# 	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) $(KERNEL_LIBS) -c $< -o $@ 
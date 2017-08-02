# Makefile dependencies directory
MK_COMMON_DIR=	./../mk/

# Definitions for Makefile
NAME=		$(ROOT_DIR)/build/boot/kernel.bin

ISO=		$(ROOT_DIR)/build/kernel.iso

DEBUG=						yes

LOCAL_DEBUG_CFLAGS= #-fsanitize=undefined

LOCAL_ASFLAGS=			-f elf32

LOCAL_COMP_CFLAGS=	-m32											\
										-ffreestanding						\
										-fno-builtin							\
										-fno-stack-protector			\
										-fno-omit-frame-pointer 	\
										-nodefaultlibs						\
										-nostdlib									\
										-MMD 											\
										-mno-sse 									\
										-mno-sse2 								\
										-mno-sse3 								\
										-mno-sse4.1 							\
										-mno-sse4.2 							\
										-mno-sse4									\
										-isystem ./include				\
										-isystem ../libc/include

LOCAL_COMP_FLAGS=

LOCAL_LINK_FLAGS=		-T ./src/link.ld			\
										-melf_i386						\
										-L$(ROOT_DIR)/build/	\
										-lbordos_c

LOCAL_DEBUG_FLAGS=

include $(MK_COMMON_DIR)colors.mk $(MK_COMMON_DIR)defs.mk

LD=									i386-unknown-linux-gnu-ld

# Project's directories

SRC_DIR=	./src/

INCLUDE_DIR=

DIR_LIST=	$(SRC_DIR)									\
					$(SRC_DIR)arch							\
					$(SRC_DIR)arch/x86					\
					$(SRC_DIR)drivers

HEADER=

# Project's files

SRC_FILES=	$(shell find -E . -regex '.*\.(asm|s|c|cpp)' | cut -c7-)

SRC=				$(addprefix $(SRC_DIR), $(SRC_FILES))

INC_DIR=		$(DIR_LIST:$(SRC_DIR)%=$(INCLUDE_DIR)%)

PRECOMPILED_HEADER= $(addprefix $(INCLUDE_DIR),$(HEADER))

# Rules
ifeq ($(shell uname -s),Darwin)
include $(MK_COMMON_DIR)rules_osx.mk
else
include $(MK_COMMON_DIR)rules.mk
endif

build:
		grub-mkrescue -o $(ISO) $(ROOT_DIR)/build/

run:
		qemu-system-i386 -s $(ISO)

clean::
		$(RM) $(ISO)

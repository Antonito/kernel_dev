# Makefile dependencies directory
MK_DIR=		./mk/
MK_COMMON_DIR=	./../mk/

# Definitions for Makefile
NAME=		$(ROOT_DIR)/build/libbordos_c.a

LOCAL_ASFLAGS=

LOCAL_DEBUG_CFLAGS=

LOCAL_COMP_CFLAGS=	-O2												\
										-nostdlib									\
										-m32											\
										-ffreestanding						\
										--target=i686-pc-none-elf	\
										-isystem ./include

LOCAL_COMP_FLAGS=

LOCAL_LINK_FLAGS=

LOCAL_DEBUG_FLAGS=

include $(MK_COMMON_DIR)colors.mk $(MK_COMMON_DIR)defs.mk

LD=			$(CC)

# Project's directories

SRC_DIR=	./src/

INCLUDE_DIR=	../kernel/include

DIR_LIST=	$(SRC_DIR)

HEADER=

# Project's files

SRC_FILES= $(shell find -E . -regex '.*\.(asm|s|c|cpp)' | cut -c7-)

SRC=			$(addprefix $(SRC_DIR), $(SRC_FILES))


INC_DIR=	$(DIR_LIST:$(SRC_DIR)%=$(INCLUDE_DIR)%)

PRECOMPILED_HEADER= $(addprefix $(INCLUDE_DIR),$(HEADER))

# Rules
ifeq ($(shell uname -s),Darwin)
include $(MK_COMMON_DIR)rules_osx.mk
else
include $(MK_COMMON_DIR)rules.mk
endif

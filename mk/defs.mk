# Commands definitions
RM=							rm -f
RM_DIR=					rm -rf
RM_EMPTY_DIR=		rm -d
ECHO=						printf
RANLIB=					i386-unknown-linux-gnu-ar rcs
CP=							cp
CHDIR=					cd
MKDIR=					mkdir

ROOT_DIR:=	$(realpath $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))/../)
INSTALL_DIR=	bin/
INSTALL_PATH=	$(ROOT_DIR)/$(INSTALL_DIR)
UNAME_S:=	$(shell uname -s)

TARGET=		native

# Compilation and link definitions
AS=				nasm
CC=				gcc
CXX=			g++
CPP_VER=	c++14
C_VER=		c11

# Simple OSX check
ifeq ($(UNAME_S),Darwin)
CXX=			clang++
CC=				i386-elf-gcc
endif

# Debug Infos
ifeq ($(DEBUG), yes)
CFLAGS=		-g -DDEBUG -O0 $(LOCAL_DEBUG_CFLAGS)
CXXFLAGS=	-g -DDEBUG -O0 $(LOCAL_DEBUG_FLAGS)

LDFLAGS=	-g
ifeq ($(SANITIZER), yes)
LDFLAGS+=	 -fsanitize=undefined -fsanitize=address
endif

else
CXXFLAGS=	-DNDEBUG
CFLAGS=		-DNDEBUG
LDFLAGS=

endif

ASFLAGS=	$(LOCAL_ASFLAGS)

CFLAGS+=	-std=$(C_VER) -W -Wall -Wextra $(LOCAL_COMP_CFLAGS)		\
					$(addprefix -I, $(INC_DIR))

CXXFLAGS+=	-std=$(CPP_VER) -W -Wall -Wextra -Weffc++ \
						$(addprefix -I, $(INC_DIR))								\
						$(LOCAL_COMP_FLAGS)

LDFLAGS+=	$(LOCAL_LINK_FLAGS)

ifeq ($(CC),clang)
CFLAGS+=		-Weverything --target=i686-pc-none-elf
endif

ifeq ($(CXX),clang++)
CXXFLAGS+=	-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic 	\
						-Wno-c++11-compat
LDFLAGS+=
else ifeq ($(CXX),g++)
CXXFLAGS+=	-Wlogical-op -Wstrict-null-sentinel
LDFLAGS+=
else
CXXFLAGS+=
LDFLAGS+=
endif

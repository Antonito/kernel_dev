MAIN_OBJ_DIR=	$(ROOT_DIR)/obj/
EMPTY=
OBJ_DIR=	$(MAIN_OBJ_DIR)$(shell basename $(CURDIR))/
ifeq ($(CXX),clang++)
PRECOMPILED_OBJ=$(PRECOMPILED_HEADER:%.hpp=%.hpp.pch)
else
PRECOMPILED_OBJ=$(PRECOMPILED_HEADER:%.hpp=%.hpp.gch)
endif
OBJ_C=		$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
OBJ_CPP=	$(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
OBJ_MM=		$(SRC:$(SRC_DIR)%.mm=$(OBJ_DIR)%.o)
OBJ_ASM=	$(SRC:$(SRC_DIR)%.asm=$(OBJ_DIR)%.o)
OBJ_S=		$(SRC:$(SRC_DIR)%.s=$(OBJ_DIR)%.o)
OBJ=			$(filter %.o, $(OBJ_C) $(OBJ_CPP) $(OBJ_MM) $(OBJ_ASM) $(OBJ_S))
OBJ_DIR_LIST=	$(DIR_LIST:$(SRC_DIR)%=$(OBJ_DIR)%)

NAME_EXTENSION=	$(suffix $(NAME))

$(NAME):
ifneq ($(HEADER),)
		@$(MAKE) $(PRECOMPILED_OBJ)
endif
		@$(foreach dir, $(MAIN_OBJ_DIR) $(OBJ_DIR_LIST), $(MKDIR) $(dir) &> /dev/null && \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Created obj directory $(CYAN)"$(dir)"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Cannot create obj directory $(CYAN)"$(dir)"\n$(CLEAR)";)
		@$(MAKE) binary

binary:	 $(OBJ)
ifeq ($(NAME_EXTENSION),.a)
		@$(RANLIB) $(NAME) $(OBJ) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Generated $(CYAN)"$(NAME)"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Generated $(CYAN)"$(NAME)"\n$(CLEAR)"
else
		@$(LD) $(OBJ) -o $(NAME) $(LDFLAGS) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Linked $(CYAN)"$(NAME)"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Linked $(CYAN)"$(NAME)"\n$(CLEAR)"
endif

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
		@$(CC) $(CFLAGS) -c -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.s
		@$(AS) $(ASFLAGS) -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Assembled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Assembled "$<"\n$(CLEAR)"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.asm
		@$(AS) $(ASFLAGS) -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Assembled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Assembled "$<"\n$(CLEAR)"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.mm
ifeq ($(CXX),clang++)
		@$(CXX) $(CXXFLAGS) -c -o $@ $< $(addprefix -include ,$(PRECOMPILED_HEADER)) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"
else
		@$(CXX) $(CXXFLAGS) -c -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"
endif

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp
ifeq ($(CXX),clang++)
		@$(CXX) $(CXXFLAGS) -c -o $@ $< $(addprefix -include ,$(PRECOMPILED_HEADER)) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"
else
		@$(CXX) $(CXXFLAGS) -c -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"
endif

%.hpp.gch:		%.hpp
		@$(CXX) -x c++-header $(CXXFLAGS) -c -o $@ $(SRC_DIR)stdafx.cpp && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Precompiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Precompiled "$<"\n$(CLEAR)"

%.hpp.pch:		%.hpp
		@$(CXX) -x c++-header $(CXXFLAGS) -c -o $@ $(SRC_DIR)stdafx.cpp && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Precompiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Precompiled "$<"\n$(CLEAR)"

$(MAIN_OBJ_DIR) $(OBJ_DIR_LIST):
		@$(MKDIR) $@ && \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Created obj directory $(CYAN)"$@"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Cannot create obj directory $(CYAN)"$@"\n$(CLEAR)"

all:		$(NAME)

infos:
		@$(ECHO) "$(CYAN)C++ Compiler:\t\t$(CXX)\n"
		@$(ECHO) "C++ Compilation Flags:\t$(CXXFLAGS)\n"
		@$(ECHO) "C Compiler:\t\t$(CC)\n"
		@$(ECHO) "C Compilation Flags:\t$(CFLAGS)\n"
		@$(ECHO) "Assembly :\t\t$(AS)\n"
		@$(ECHO) "Assembly Flags:\t\t$(ASFLAGS)\n"
ifeq ($(NAME_EXTENSION),.a)
		@$(ECHO) "Library Generation:\t$(RANLIB)\n$(CLEAR)"
else
		@$(ECHO) "Link Flags:\t\t$(LDFLAGS)\n$(CLEAR)"
endif

clean:
		@$(RM) $(OBJ)
		@$(RM) $(PRECOMPILED_OBJ)
		@$(RM_DIR) $(OBJ_DIR)
		@$(RM_EMPTY_DIR) $(MAIN_OBJ_DIR) >&- 2>&- || $(ECHO) ""
		@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed OBJs files and directory\n$(CLEAR)"


fclean:		clean
		@$(RM) $(NAME)
		@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed $(CYAN)"$(NAME)"\n$(CLEAR)"

re:
		@$(MAKE) fclean
		@$(MAKE) all

run:
		./$(NAME)

.PHONY: all clean fclean re infos run

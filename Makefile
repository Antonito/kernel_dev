MK_DIR=			./mk/

include $(MK_DIR)colors.mk $(MK_DIR)defs.mk

###### Add your projects here #####
PROJECT_DIR=		./

PROJECTS=		kernel

LIB_DIR=		./

LIBS=				libc

PROJECTS_PATH=		$(addprefix $(LIB_DIR), $(LIBS))		\
									$(addprefix $(PROJECT_DIR), $(PROJECTS))

# Some useful variables
DEBUG=						yes

ARGS=			DEBUG=$(DEBUG) SANITIZER=no -s --warn-undefined-variables -C

CURRENT_DIR=		${PWD}

PRINT_DIR=		$(ECHO) "$(YELLOW)$(path) :$(CLEAR)\n";

RULES=			all				\
						clean			\
						fclean		\
						re				\
						infos

no_rule:
			@$(foreach path, $(PROJECTS_PATH),		\
			$(PRINT_DIR)													\
			$(MAKE) $(ARGS) $(path);)

$(RULES):
			@$(foreach path, $(PROJECTS_PATH),		\
			$(PRINT_DIR)													\
			$(MAKE) $(ARGS) $(path) $@;)


$(PROJECTS):
			@$(ECHO) "$(YELLOW)./$(PROJECT_DIR)$@/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(PROJECT_DIR)$@

$(foreach _rule, $(RULES), $(addsuffix -$(_rule),$(PROJECTS))):
			$(eval _rule := $(lastword $(subst -, ,$@)))
			$(eval _proj := $(@:%-$(_rule)=%))
			@$(ECHO) "$(YELLOW)$(PROJECT_DIR)$(_proj)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(PROJECT_DIR)$(_proj) $(_rule);


$(LIBS):
			@$(ECHO) "$(YELLOW)./$(PROJECT_DIR)$@/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(PROJECT_DIR)$@

$(foreach _rule, $(RULES), $(addsuffix -$(_rule),$(LIBS))):
			$(eval _rule := $(lastword $(subst -, ,$@)))
			$(eval _proj := $(@:%-$(_rule)=%))
			@$(ECHO) "$(YELLOW)$(LIB_DIR)$(_proj)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(LIB_DIR)$(_proj) $(_rule);

run:
	@$(MAKE) $(ARGS) kernel run

.PHONY: no_rule all $(RULES) $(PROJECTS) $(foreach _rule, $(RULES), $(addsuffix -$(_rule),$(PROJECTS))) \
	$(LIBS) $(foreach _rule, $(RULES), $(addsuffix -$(_rule),$(LIBS)))

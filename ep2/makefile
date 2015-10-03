BIN_SIMULATOR = simulator
SRCS_SIMULATOR = $(wildcard src/*.c) $(wildcard src/simulator/*.c)
BIN_SHELL = shell
SRCS_SHELL = $(wildcard src/*.c) $(wildcard src/shell/*.c)

INCS = -Isrc/
LIBS = -lreadline

CFLAGS = $(INCS) -c -g -Wall -std=c99 -pedantic
LDFLAGS = $(LIBS)
OUTFLAG = -o
INFLAG =

CC = gcc
DEPGEN = gcc $(INCS) -MM -MP -MT $(patsubst %$(SRCEXT),%$(OBJEXT),$<)
RM = rm -f
TOUCH = touch

OBJEXT = .o
DEPEXT = .d
SRCEXT = .c
OBJS_SIMULATOR = $(SRCS_SIMULATOR:$(SRCEXT)=$(OBJEXT))
OBJS_SHELL = $(SRCS_SHELL:$(SRCEXT)=$(OBJEXT))
DEPS = $(SRCS_SIMULATOR:$(SRCEXT)=$(DEPEXT)) $(SRCS_SHELL:$(SRCEXT)=$(DEPEXT))

all: $(BIN_SIMULATOR) $(BIN_SHELL)

$(BIN_SIMULATOR): $(OBJS_SIMULATOR)
	$(CC) $(LDFLAGS) $(INFLAG)$^ $(OUTFLAG)$@

$(BIN_SHELL): $(OBJS_SHELL)
	$(CC) $(LDFLAGS) $(INFLAG)$^ $(OUTFLAG)$@

clean: clean_simulator clean_shell

clean_simulator:
	$(RM) $(BIN_SIMULATOR) $(OBJS_SIMULATOR) $(DEPS)

clean_shell:
	$(RM) $(BIN_SHELL) $(OBJS_SHELL) $(DEPS)

%$(OBJEXT): %$(SRCEXT)
	$(DEPGEN) $< $(OUTFLAG)$(patsubst %$(OBJEXT),%$(DEPEXT),$@)
	$(CC) $(CFLAGS) $(INFLAG)$< $(OUTFLAG)$@

%$(DEPEXT): %$(SRCEXT)
	@$(TOUCH) $@

-include $(DEPS)

CC=gcc
LD=ld

CFLAGS=-g -std=c99
LDFLAGS=-lpcre

SRCDIR=src
OBJDIR=obj
BINDIR=bin

ifdef SYSTEMROOT
	BINEXT=.exe
endif

HELPER_SOURCES=$(wildcard $(SRCDIR)/helpers/*.c)
HELPER_OBJS=$(patsubst $(SRCDIR)/helpers/%.c,$(OBJDIR)/helpers/%.o,$(HELPER_SOURCES))

PUZZLE_SOURCES=$(wildcard $(SRCDIR)/day*.c)
PUZZLE_PROGS=$(patsubst $(SRCDIR)/%.c,$(BINDIR)/%$(BINEXT),$(PUZZLE_SOURCES))

.PHONY: all clean variables list

.PRECIOUS: $(OBJDIR)/%.o $(HELPER_OBJS)

$(BINDIR)/%$(BINEXT): $(OBJDIR)/%.o $(HELPER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

all: $(PUZZLE_PROGS)

clean:
	rm -f $(BINDIR)/*.exe
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/**/*.o
	
variables:
	@echo "CC: $(CC)"
	@echo "LD: $(LD)"
	@echo "CLAGS: $(CFLAGS)"
	@echo "SRCDIR: $(SRCDIR)"
	@echo "OBJDIR: $(OBJDIR)"
	@echo "BINDIR: $(BINDIR)"
	@echo "HELPER_SOURCES: $(HELPER_SOURCES)"
	@echo "HELPER_OBJS: $(HELPER_OBJS)"
	@echo "PUZZLE_SOURCES: $(PUZZLE_SOURCES)"
	@echo "PUZZLE_PROGS: $(PUZZLE_PROGS)"

list:
	@$(MAKE) -pRrq -f $(lastword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/^# File/,/^# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | egrep -v -e '^[^[:alnum:]]' -e '^$@$$' | xargs

ifdef SYSTEMROOT
	BINEXT=.exe
endif

ifeq ($(shell uname),Darwin)
	PCRE_INCLUDE=-I/usr/local/Cellar/pcre/8.39/include
	OPENSSL_INCLUDE=-I/usr/local/Cellar/openssl/1.0.2j/include
	OPENSSL_LIB=-L/usr/local/Cellar/openssl/1.0.2j/lib -lcrypto
endif

EXTRA_INCLUDES=$(PCRE_INCLUDE) $(OPENSSL_INCLUDE)

CC=clang
LD=lld

ifeq ($(USE_GCC),1)
	CC=gcc
	LD=ld
endif

CFLAGS=-std=c99
CFLAGS_DEBUG=-g -Wall -Wextra -Werror
ifeq ($(DEBUG),1)
	CFLAGS+=$(CFLAGS_DEBUG) -DDEBUG
endif
LDFLAGS=-lpcre

SRCDIR=src
OBJDIR=obj
BINDIR=bin

HELPER_SOURCES=$(wildcard $(SRCDIR)/helpers/*.c)
HELPER_OBJS=$(patsubst $(SRCDIR)/helpers/%.c,$(OBJDIR)/helpers/%.o,$(HELPER_SOURCES))

PUZZLE_SOURCES=$(wildcard $(SRCDIR)/day*.c)
PUZZLE_PROGS=$(patsubst $(SRCDIR)/%.c,$(BINDIR)/%$(BINEXT),$(PUZZLE_SOURCES))

.PHONY: all clean variables list

.PRECIOUS: $(OBJDIR)/%.o $(HELPER_OBJS)

# day5 - custom build for openssl/md5 include
$(BINDIR)/day5$(BINEXT): $(OBJDIR)/day5.o $(HELPER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(OPENSSL_LIB)

$(BINDIR)/%$(BINEXT): $(OBJDIR)/%.o $(HELPER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(EXTRA_INCLUDES) -c -o $@ $^

all: $(PUZZLE_PROGS)

clean:
	rm -f $(BINDIR)/*.exe
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/**/*.o
	
variables:
	@echo "OS: $(OS)"
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

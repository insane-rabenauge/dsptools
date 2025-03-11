PROGRAM=asm56000
SOURCES=\
asm56000.c\


ifeq ($(TARGET),win)
CC=i686-w64-mingw32-gcc
else ifeq ($(TARGET),l32)
CC=i686-linux-gnu-gcc
else
CC=gcc
endif

CFLAGS=-Wall -Wno-switch -Os

LDFLAGS=

CFILES=$(filter %.c,$(SOURCES))
OBJECTS=$(addsuffix .o,$(basename $(SOURCES)))

DEPFILES=$(addsuffix .d,$(basename $(CFILES)))
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

.PHONY:	all
all:	$(PROGRAM)

.PHONY:	run
run:	$(PROGRAM)
	./$(PROGRAM) -A -B -L "mp2 decoder.asm"

.PHONY: clean
clean:	
	@rm -f $(DEPFILES) $(OBJECTS)

.PHONY:	backup
backup:
	find . -path ./old -prune -o -path ./data -prune -o -type f -print0 | tar cfz `date +old/v%Y%m%d%H%M.tar.gz` --null -T -

$(PROGRAM): $(OBJECTS)
	@echo " LD $@"
	@$(CC) $(OBJECTS) -s -o $@ $(LDFLAGS)

%.o : %.c %.d
	@echo " CC $<"
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPFILES):
include $(wildcard $(DEPFILES))

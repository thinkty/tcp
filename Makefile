.POSIX:    # Parse it an run in POSIX conforming mode
.SUFFIXES: # Delete the default suffixes (inference rules)

CC=gcc
CFLAGS=-g -Wall -Wextra -Wconversion -Wsign-conversion -I$(IDIR)
OUTPUT=server
ROOTDIR=.
IDIR=$(ROOTDIR)/include
SDIR=$(ROOTDIR)/src
ODIR=$(ROOTDIR)/obj

_DEPS=tcp.h server.h common.h
DEPS=$(addprefix $(IDIR)/,$(_DEPS))

_OBJS=tcp.o server.o
OBJS=$(addprefix $(ODIR)/,$(_OBJS))

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJS): | $(ODIR)

$(ODIR):
	mkdir $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean bear 
clean:
	rm -rf $(ODIR) $(OUTPUT)

bear: clean
	bear -- make

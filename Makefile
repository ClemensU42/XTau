CFLAGS += -g -std=c99 -Wall -Wextra -I/usr/local/include -Iinclude
LDFLAGS += -L/usr/local/lib -lxcb #-lxcb-randr -lxcb-keysyms -lxcb-iccm -lxcb-util

RM=/bin/rm

TARGETS = XTau

SOURCEDIR = src
INCDIR = include
BUILDDIR = build
OUTDIR = bin

SOURCES = $(wildcard *.c $(foreach fd, $(SOURCEDIR), $(fd)/*.c))
INCS = $(wildcard *.h $(foreach fd, $(INCDIR), $(fd)/*.h))
OBJS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY := all 

all: $(TARGETS)

XTau: $(OBJS)
	mkdir -p $(OUTDIR)
	$(CC) -o $(OUTDIR)/$@ $(OBJS) $(LDFLAGS)

$(BUILDDIR)/%.o : $(SOURCEDIR)/%.c $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $<


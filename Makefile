CC = gcc
CFLAGS = -O3
LNFLAGS = -lncurses
DESTDIR ?= $(HOME)/bin
EXEC = fireplace


all: $(EXEC)

.PHONY: debug clean install uninstall

debug: CFLAGS=-g
debug: $(EXEC)

$(EXEC): main.c
	$(CC) main.c $(CFLAGS) $(LNFLAGS) -o $(EXEC)

clean:
	rm $(EXEC)

install:
	install -d $(DESTDIR)
	install -m 755 $(EXEC) $(DESTDIR)

uninstall:
	rm $(DESTDIR)/$(EXEC)


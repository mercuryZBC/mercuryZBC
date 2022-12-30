.PHONY:Server clean

INCLUDE_PATH=../include
INSTALL_PATH=/usr/bin
LIBRARY=../library
CC=gcc
CFALGS=-I$(INCLUDE_PATH) -g -Wall -c
LDFLAGS=-lpthread
TARGET=Server

SFILE=$(wildcard *.c)
DFILE=$(patsubst %.c,%.o,$(SFILE))

$(TARGET):$(DFILE)
	$(CC) $^ $(LDFLAGS) -o $@


%.o:%.c
	$(CC) $(CFALGS) $<

clean:
	rm -rf $(DFILE)
	rm -rf $(TARGET)

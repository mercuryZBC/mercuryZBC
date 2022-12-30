.PHONY:Talk_client clean

INCLUDE_PATH=./
INSTALL_PATH=/usr/bin
LIBRARY=../library
CC=gcc
CFALGS=-I$(INCLUDE_PATH) -g -Wall -c
LDFLAGS=
TARGET=Talk_client

SFILE=$(wildcard *.c)
DFILE=$(patsubst %.c,%.o,$(SFILE))

$(TARGET):$(DFILE)
	$(CC) $^  -o $@

%.o:%.c
	$(CC) $(CFALGS) $<

clean:
	rm -rf $(DFILE)
	rm -rf $(TARGET)


CC=clang
CFLAGS=
SRCS=$(wildcard ./*.c)

all :
	$(CC) $(CFLAGS) -o ./main $(SRCS) -I./
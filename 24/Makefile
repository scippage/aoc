CC := clang++

ifeq ($(DEBUG),1)
	CFLAGS := -std=c++20 -Wall -O0 -g
else
	CFLAGS := -std=c++20 -Wall -O1 -DNDEBUG
endif

LDFLAGS := -lrt -flto -fuse-ld=gold

UTIL_DIR := ../util
CFLAGS += -I /usr/include/eigen3/ -I /usr/include/ -lz3

all: a b

a: a.cc $(UTIL_DIR)/util.cc
	$(CC) $(CFLAGS) $^ -o $@

b: b.cc $(UTIL_DIR)/util.cc
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f a b *.o $(UTIL_DIR)/util.o
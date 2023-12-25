CC := clang++

ifeq ($(DEBUG),1)
	CFLAGS := -std=c++20 -Wall -O0 -g
else
	CFLAGS := -std=c++20 -Wall -O1 -DNDEBUG
endif

LDFLAGS := -lrt -flto -fuse-ld=gold

UTIL_DIR := ../util
CFLAGS += -I /usr/include/eigen3/ -I /usr/include/ -lz3
# CFLAGS += -I /opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3 -I /opt/homebrew/Cellar/z3/4.12.4/include -L /opt/homebrew/Cellar/z3/4.12.4/lib -lz3

all: a b

a: a.cc $(UTIL_DIR)/util.cc
	$(CC) $(CFLAGS) $^ -o $@

b: b.cc $(UTIL_DIR)/util.cc
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f a b *.o $(UTIL_DIR)/util.o
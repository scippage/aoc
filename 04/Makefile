CC := clang++

ifeq ($(DEBUG),1)
	CFLAGS := -std=c++17 -Wall -O0 -g
else
	CFLAGS := -std=c++17 -Wall -O1 -DNDEBUG
endif

LDFLAGS := -lrt -flto -fuse-ld=gold

all: a b

a: a.cc
	$(CC) $(CFLAGS) a.cc -o a

b: b.cc
	$(CC) $(CFLAGS) b.cc -o b

clean:
	rm -f a b *.o

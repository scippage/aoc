CC := clang++

ifeq ($(DEBUG),1)
	CFLAGS := -std=c++20 -Wall -O0 -g
else
	CFLAGS := -std=c++20 -Wall -O1 -DNDEBUG
endif

LDFLAGS := -lrt -flto -fuse-ld=gold

all: a b b_alt

a: a.cc
	$(CC) $(CFLAGS) a.cc -o a

b: b.cc
	$(CC) $(CFLAGS) b.cc -o b

b_alt: b_alt.cc
	$(CC) $(CFLAGS) b_alt.cc -o b_alt

clean:
	rm -f a b b_alt *.o

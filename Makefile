SRCS=main.cpp
EXECUTABLES=result

CC=g++
CFLAGS=-std=c++17 -g -Wall
LDFLAGS=

OBJS=$(SRCS:.cpp=.o)

all: $(EXECUTABLES)

$(EXECUTABLES): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLES) $(OBJS)

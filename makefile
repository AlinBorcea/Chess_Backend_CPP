CPPC=g++
CFLAGS=-Wall -g -pedantic
SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst src/%.cpp, obj/%.o, $(SRCS))
BIN=target/main.exe

all: $(BIN)

$(BIN): $(OBJS)
	$(CPPC) $(CFLAGS) $(OBJS) -o $(BIN)

obj/%.o: src/%.cpp
	$(CPPC) $(CFLAGS) -c $< -o $@

clean:
	rm obj/*.o target/*.exe
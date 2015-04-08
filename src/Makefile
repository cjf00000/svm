CXXFLAGS=-g --std=c++11 -O3 -I../third_party/include

TARGETS=main
OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))

LDFLAGS=-L../third_party/lib -lglog -lgflags -Wl,-rpath,../third_party/lib

all: main

main: $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	-rm $(TARGETS)
	-rm *.o

.PHONY: clean

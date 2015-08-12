# Makefile for Project Timeline
#
# Your compiler
CXX = g++

# Compilation flags
# '-g' turns debugging flags on.
# Not Using O2 flag for optimisation.
CXXFLAGS = -g -ljpeg -mavx -lm -lpthread -lX11 -O3 `pkg-config --cflags opencv`

# Linker flags
# When you need to add a library
LDFLAGS = -ljpeg -mavx -lm -lpthread -lX11 `pkg-config --libs opencv` -O3

# all is a target
# $(VAR) gives value of the variable.
# $@ stores the target
# $^ stores the dependency
all: bin/dft_demo bin/curve_predict

bin/dft_demo: obj/dft_demo.o
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/curve_predict: obj/curve_predict.o
	$(CXX) -o $@ $^ $(LDFLAGS)

obj/dft_demo.o: src/dft_demo.cpp
	mkdir -p obj bin
	$(CXX) -c $(CXXFLAGS) -o $@ $<

obj/curve_predict.o: src/curve_predict.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# .PHONY tells make that 'all' or 'clean' aren't _actually_ files, and always
# execute the compilation action when 'make all' or 'make clean' are used
.PHONY: all

# Delete all the temporary files we've created so far
clean:
	rm -rf obj/*.o
	rm -rf bin/oic

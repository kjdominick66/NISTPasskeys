CXX = gcc
CXXFLAGS = -Wall -g
X11FLAGS = -lX11
PROGRAM = main.c generator.c

all:
	$(CXX) $(CXXFLAGS) $(PROGRAM) -o passphrase.out $(X11FLAGS)

run: all
	./passphrase.out

clean:
	rm -f *.out


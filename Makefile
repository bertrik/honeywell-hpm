CXXFLAGS = -W -Wall -O2 -DDEBUG

all: test

clean:
	rm -f hpma115s_test

test: hpma115s_test
	./hpma115s_test


CFLAGS=-Wall $(shell python3-config --includes) -I/usr/local/lib/python3.7/site-packages/numpy/core/include/ -I/usr/local/include/eigen3 -Wno-unused -pthread -fPIC -shared -ggdb3 -O3 -march=native
CXXFLAGS=-std=gnu++0x $(CFLAGS)
LDFLAGS=$(shell python3-config --ldflags)
OBJS=libtheta.o common.o

.PHONY: check
check: libtheta.so
	python3 test.py

libtheta.so: $(OBJS)
	$(CXX) -shared $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) libtheta.so
	find . -name "*.pyc" -type f -exec rm -f {} ";"

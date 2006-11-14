H_FILES = \
	dysocket.h\
	dysocketprivate.h

CC_FILES = \
	dysocketaddr.cc\
	dysocketimpl.cc\
	dysocketmisc.cc\
	dytcpsocket.cc\
	dyudpsocket.cc

OBJECTS = \
	dysocketaddr.o\
	dysocketimpl.o\
	dysocketmisc.o\
	dytcpsocket.o\
	dyudpsocket.o

FILES = $(HEADERS) $(H_FILES) $(CC_FILES)
MAKE=make
AR=ar
CXX=g++
CFLAGS=-g -ansi -O3 -pedantic -fomit-frame-pointer -Werror -Wall -Wno-trigraphs

INCLUDES=-I. -I..

all: libdysocket.a test
	@echo "done building all"

lib: libdysocket.so
	@echo "done building libdysocket.so"

libdysocket.a: $(HEADERS) $(OBJECTS)
	@if [ `uname -s` == "Darwin" ]; then \
		libtool -static -o $@ $(OBJECTS); \
	else \
		$(AR) cru $@ $^; \
		ranlib $@; \
	fi
	@echo "done building libdysocket.a"

.cc.o: $(FILES) 
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<

test: libdysocket.a echo_server udp_server udp_client getip
	@echo "done building tests"

echo_server: libdysocket.a echo_server.o
	$(CXX) echo_server.o libdysocket.a -o echo_server
echo_server.o: echo_server.cc
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<

udp_client: libdysocket.a udp_client.o
	$(CXX) udp_client.o libdysocket.a -o udp_client
udp_client.o: udp_client.cc
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<
udp_server: libdysocket.a udp_server.o
	$(CXX) udp_server.o libdysocket.a -o udp_server
udp_server.o: udp_server.cc
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<

getip: libdysocket.a getip.o
	$(CXX) getip.o libdysocket.a -o getip
getip.o: getip.cc
	$(CXX) $(INCLUDES) $(CFLAGS) -c $<

clean:
	rm -rf *~ *.o libdysocket.a udp_server udp_client echo_server getip


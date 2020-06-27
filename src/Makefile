CXX = g++
CC = gcc
CXXFLAGS = -Wall -g
CFLAGS = -Wall -g
INCLUDES = -I.
CLIBS = -lpthread

# CGI = external/fastcgi
PARSER = external/http-parser
UTILS = utils
SRCS = $(wildcard *.cpp $(PARSER)/*.c $(UTILS)/*.cpp)
DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -fsanitize=address -fno-omit-frame-pointer
endif

MAIN = webserver

.PHONY: depend clean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $^


$(MAIN): %.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CLIBS)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ : ]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: ;
#include $(wildcard $(SRCS:.cpp=.d))

clean:
	rm $(MAIN) $(PARSER)/*.o $(UTILS)/*.o


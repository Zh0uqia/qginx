CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -I.
CLIBS = -lpthread

CGI = external/fastcgi/*.c
PARSER = external/http-parser/*.c
ARCHIVED = archived/*.cpp
SRCS = $(wildcard *.cpp $(CGI) $(PARSER) $(ARCHIVED))
OBJS = $(SRCS:.c=.o)

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -fsanitize=address -fno-omit-frame-pointer
endif

MAIN = webserver

.PHONY: depend clean 

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(CLIBS)

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^


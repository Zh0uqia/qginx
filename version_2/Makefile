CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -I.
CLIBS = -lpthread
SRCS = $(wildcard *.cpp lib/*.cpp httpServer/*.cpp)
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



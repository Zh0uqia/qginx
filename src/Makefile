CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -I.
CLIBS = -lpthread

CGI = external/fastcgi
PARSER = external/http-parser
ARCHIVED = archived
UTILS = utils
SRCS = $(wildcard *.cpp $(CGI)/*.c $(PARSER)/*.c $(ARCHIVED)/*.cpp $(UTILS)/*.cpp)

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -fsanitize=address -fno-omit-frame-pointer
endif

MAIN = webserver

.PHONY: depend clean 

$(MAIN): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(CLIBS)

clean:
	rm $(MAIN) $(CGI)/*.o $(PARSER)/*.o $(ARCHIVED)/*.o $(UTILS)/*.o

depend: $(SRCS)
	makedepend $(INCLUDES) $^


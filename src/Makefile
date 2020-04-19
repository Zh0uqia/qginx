CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -I./ -I./fastcgi 
SRCS = Main.cpp Server.cpp MasterProcess.cpp Process.cpp Handler.cpp WorkerProcess.cpp Epoll.cpp \
	   RequestHandler.cpp Dispatcher.cpp Controller.cpp \
	   Response.cpp

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -fsanitize=address -fno-omit-frame-pointer
endif

OBJS = $(SRCS:.c=.o)

MAIN = webserver

.PHONY: depend clean 

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./fastcgi/fcgi.c -o $(MAIN) $(OBJS) -lpthread

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^



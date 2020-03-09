CXX = g++
CXXFLAGS = -Wall -g
INCLUDES = -I./ -I./fastcgi
SRCS = Main.cpp Server.cpp RequestHandler.cpp
# Dispatcher.cpp Controller.cpp Response.cpp RequestHandler.cpp
OBJS = $(SRCS:.c=.o)

MAIN = webserver

.PHONY: depend clean 

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./fastcgi/fcgi.c -o $(MAIN) $(OBJS)

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm *.o $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^



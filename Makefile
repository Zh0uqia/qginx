webserver: Main.cpp Server.cpp CommParser.cpp Request.cpp Dispatcher.cpp Controller.cpp Response.cpp  
	g++ -o Server.o -c Server.cpp -I./ -I./fastcgi 
	g++ -o Request.o -c Request.cpp -I./ -I./fastcgi 
	g++ -o CommParser.o -c CommParser.cpp -I./ -I./fastcgi 
	g++ -o Dispatcher.o -c Dispatcher.cpp -I./ -I./fastcgi 
	g++ -o Controller.o -c Controller.cpp -I./ -I./fastcgi
	g++ -o Response.o -c Response.cpp -I./ -I./fastcgi 
	g++ -o Main.o -c Main.cpp -I./ -I./fastcgi 
	g++ ./fastcgi/fcgi.c -o webserver Server.o Request.o CommParser.o Dispatcher.o Controller.o Response.o Main.o -I.

clean: 
	rm webserver Server.o Request.o CommParser.o Main.o Dispatcher.o Controller.o Response.o


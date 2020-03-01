webserver: Main.cpp Server.cpp
	g++ -o Server.o -c Server.cpp -I.
	g++ -o Main.o -c Main.cpp -I.
	g++ -o webserver Server.o Main.o -I.

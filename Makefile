target : 
# Question 1
	g++ -c parseBibFile.cpp
	g++ -g -o debug1 parseBibFile.o
	g++ -O3 -o optimize1 parseBibFile.o
# Question 2
	g++ -c main.cpp
	g++ -g -o debug2 main.o
	g++ -O3 -o optimize2 main.o



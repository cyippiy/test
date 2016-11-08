all: main.cpp course.cpp functions.cpp
	g++ -std=c++11 main.cpp -o scheduler course.cpp 
clean:
	rm scheduler
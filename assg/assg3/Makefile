a.out : main.o Automobile.o Car.o SUV.o Truck.o
	g++ -o output main.o Automobile.o Car.o SUV.o Truck.o

main.o : main.cpp Car.h SUV.h Truck.h 
	g++ -c main.cpp

Automobile.o : Automobile.cpp Automobile.h
	g++ -c Automobile.cpp

Car.o : Car.cpp Automobile.h Car.h
	g++ -c Car.cpp

SUV.o : SUV.cpp Automobile.h SUV.h
	g++ -c SUV.cpp

Truck.o : Truck.cpp Automobile.h Truck.h
	g++ -c Truck.cpp

clean: 
	rm -f a.out *.o

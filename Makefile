all: clear m2g

m2g: main.cpp structures.h utils.h format_functions.h
	g++ main.cpp structures.h utils.h format_functions.h -o m2g 

clear:
	rm -rf *.o *~ m2g

testF: cl_screen all build

build:
	./m2g --format teste.img

cl_screen:
	clear
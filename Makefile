all: clear m2g

m2g: main.cpp src/structures.h src/utils.h src/format_controller.h src/reader_controller.h src/writer_controller.h src/view_mode.h
	g++ main.cpp src/structures.h src/utils.h src/format_controller.h -o m2g -std=c++11

clear:
	rm -rf *.o *~ m2g

testF: cl_screen all build

build:
	./m2g --format teste.img

cl_screen:
	clear

SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .h .o
cgb : cgb.o
	g++ -o cgb cgb.o /usr/lib64/libboost_program_options.so

cgb.o: cgb.cpp cgb.h
	g++ -c -ggdb3 -I/usr/local/include/boost/ cgb.cpp

clean : 
	rm cgb cgb.o

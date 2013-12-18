SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .h .o
CXX = g++
LD = g++
CFLAGS = -ggdb3 -Wall -I/usr/local/include/boost/
LDFLAGS = 
RM = /bin/rm -f
OBJS = cgb.o
PROG = cgb

$(PROG) : $(OBJS)
	 $(LD) $(LDFLAGS) $(OBJS) /usr/lib64/libboost_program_options.so -o $(PROG)

cgb.o: cgb.cpp cgb.h
	$(CXX) $(CFLAGS) -c cgb.cpp

clean : 
	$(RM) $(PROG) $(OBJS)

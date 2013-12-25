SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp .h .o
CXX = g++
LD = g++
# release
#CFLAGS = -Os -I/usr/local/include/boost/
# debug
CFLAGS = -ggdb3 -Wall -I/usr/local/include/boost/
LDFLAGS = 
RM = /bin/rm -f
OBJS = cgb.o
STATIC_LIBS = /usr/lib64/libboost_program_options.so
PROG = cgb

$(PROG) : $(OBJS)
	 $(LD) $(LDFLAGS) $(OBJS) $(STATIC_LIBS) -o $(PROG)

cgb.o: cgb.cpp cgb.h
	$(CXX) $(CFLAGS) -c cgb.cpp

clean : 
	$(RM) $(PROG) $(OBJS)

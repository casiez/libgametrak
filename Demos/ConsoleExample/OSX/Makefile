VPATH    =  .. \
			../../../libgametrak/ \
			../../../libgametrak/utils/

CPPFLAGS =  -I.. \
			-I../../../ \
			-I/usr/local/include/hidapi

LIBS     =   -framework IOKit -framework CoreFoundation -framework ApplicationServices -lhidapi

OBJS     =  consoleExample.o ../../../libgametrak/hidapi-0.7.0/mac/hid.o GameTrak.o HIDAPIGameTrak.o URI.o TimeStamp.o Quaternion.o OneEuroFilter.o 

EXE      =  main

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(EXE)

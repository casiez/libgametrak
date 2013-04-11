VPATH    =  .. \
			../../../libgametrak/ \
			../../../libgametrak/utils/

CPPFLAGS =  -I.. \
			-I../../../ \
			-I/usr/local/include/hidapi

LIBS     =    -lhidapi-libusb

OBJS     =  consoleExample.o GameTrak.o HIDAPIGameTrak.o URI.o TimeStamp.o Quaternion.o OneEuroFilter.o 

EXE      =  main

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(EXE)

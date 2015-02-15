VPATH    =  ../../libgametrak \
			../../libgametrak/utils/

CPPFLAGS =  -I../.. \
			-I../../libgametrak \
			-I/usr/local/include/hidapi

LIBS     =   -lhidapi-libusb

OBJS     =  GameTrak.o HIDAPIGameTrak.o URI.o TimeStamp.o Quaternion.o OneEuroFilter.o 

LIB      =  libgametrak.so

$(LIB): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(LIB)

uninstall:
	rm -rf /usr/local/include/libgametrak
	rm -rf /usr/local/lib/$(LIB)	

install:
	rm -rf /usr/local/include/libgametrak
	rm -rf /usr/local/lib/$(LIB)
	cp -r ../../libgametrak/ /usr/local/include/libgametrak/
	rm -rf /usr/local/include/libgametrak/*.cpp
	rm -rf /usr/local/include/libgametrak/utils/*.cpp
	cp $(LIB) /usr/local/lib

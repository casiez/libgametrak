VPATH    =  ../../libgametrak \
			../../libgametrak/utils/

CPPFLAGS =  -I../../libgametrak \
			-I/usr/local/include/hidapi

LIBS     =   -framework IOKit -framework CoreFoundation -framework ApplicationServices -lhidapi

OBJS     =  GameTrak.o HIDAPIGameTrak.o URI.o TimeStamp.o Quaternion.o OneEuroFilter.o 

LIB      =  libgametrak.dylib

$(LIB): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(LIB)

install:
	rm -rf /usr/local/include/libgametrak
	rm -rf /usr/local/lib/$(LIB)
	cp -r ../../libgametrak /usr/local/include/libgametrak
	cp $(LIB) /usr/local/lib

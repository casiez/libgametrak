VPATH    =  .. \
			../../../libgametrak/ \
			../../../libgametrak/utils/

CPPFLAGS =  -I.. \
			-I../../../ \
			-I../../../libgametrak/hidapi-0.7.0/hidapi

LIBS     =   -framework IOKit -framework CoreFoundation -framework ApplicationServices -framework GLUT -framework OpenGL

OBJS     =  main.o randomtargets.o ../../../libgametrak/hidapi-0.7.0/mac/hid.o GameTrak.o HIDAPIGameTrak.o URI.o TimeStamp.o Quaternion.o OneEuroFilter.o 

EXE      =  main

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(EXE)

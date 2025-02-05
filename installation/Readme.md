To install libgametrak on the system, type:

make
sudo make install

----
On OS X and Linux, a Makefile then looks like this:

LIBS     =  -lgametrak

OBJS     =  example.o

EXE      =  example

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(EXE)
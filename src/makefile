CXX = $(shell wx-config --cxx)
WX-CONFIG_CXXFLAGS	= $(shell wx-config --cxxflags)
WX-CONFIG_LIBS		= $(shell wx-config --libs)


PROGRAM	= UsbChopper.exe
OBJECTS	= Main.o wxForm.o MyMainFrame.o Serial_win.o fifo.o

# implementation

.SUFFIXES:      .o .cpp

.cpp.o :
	$(CXX) -c $(WX-CONFIG_CXXFLAGS) -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(WX-CONFIG_LIBS)

clean:
	rm -f *.o $(PROGRAM)
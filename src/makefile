#CXX = $(shell wx-config --cxx)
#WX-CONFIG_CXXFLAGS	= $(shell wx-config --cxxflags)
#WX-CONFIG_LIBS		= $(shell wx-config --libs)
CXX = $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxx)
WX-CONFIG_CXXFLAGS	= $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxxflags)
WX-CONFIG_LIBS		= $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --libs)

PROGRAM	= UsbChopper
OBJECTS	= Main.o wxForm.o MyMainFrame.o Serial_linux.o fifo.o

# implementation

.SUFFIXES:      .o .cpp

.cpp.o :
	$(CXX) -c $(WX-CONFIG_CXXFLAGS) -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(WX-CONFIG_LIBS)

clean:
	rm -f *.o $(PROGRAM)

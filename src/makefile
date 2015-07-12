#CXX = $(shell wx-config --cxx)
#WX-CONFIG_CXXFLAGS	= $(shell wx-config --cxxflags)
#WX-CONFIG_LIBS		= $(shell wx-config --libs)
CXX = $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxx)
WX-CONFIG_CXXFLAGS	= $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxxflags)
WX-CONFIG_LIBS		= $(shell ../../library/wxWidgets-3.0.2/build-cocoa-debug/wx-config --libs)

OBJDIR = ../obj
BINDIR = ../bin

PROGRAM	= $(BINDIR)/UsbChopper
# Compile except source files for windows
SOURCES = $(wildcard *.cpp) $(filter-out serial/Serial_win.cpp, $(wildcard serial/*.cpp))
OBJECTS	= $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

# implementation

.SUFFIXES:      .o .cpp


$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@[ -d $(OBJDIR)/serial ] || mkdir -p $(OBJDIR)/serial
	$(CXX) -c $(WX-CONFIG_CXXFLAGS) -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
	$(CXX) -o $(PROGRAM) $(OBJECTS) $(WX-CONFIG_LIBS)

clean:
	rm -f $(OBJDIR)/*.o $(PROGRAM)

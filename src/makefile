#CXX = $(shell wx-config --cxx)
#WX-CONFIG_CXXFLAGS	= $(shell wx-config --cxxflags)
#WX-CONFIG_LIBS		= $(shell wx-config --libs)
CXX = $(shell ../../lib/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxx)
CFLAGS = -MMD
WX-CONFIG_CXXFLAGS	= $(shell ../../lib/wxWidgets-3.0.2/build-cocoa-debug/wx-config --cxxflags)
WX-CONFIG_LIBS		= $(shell ../../lib/wxWidgets-3.0.2/build-cocoa-debug/wx-config --libs) -lwx_osx_cocoau_gl-3.0

OBJDIR = ../obj
BINDIR = ../bin

PROGRAM	= $(BINDIR)/TottoDashboard
# Compile except source files for windows
SOURCES = $(wildcard *.cpp) $(filter-out serial/Serial_win.cpp, $(wildcard serial/*.cpp)) $(wildcard GraphFrame/*.cpp) $(wildcard CtrlFrame/*.cpp)
OBJECTS	= $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
DEPENDS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.d))

# implementation

.SUFFIXES:      .o .cpp


$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	@[ -d $(OBJDIR)/serial ] || mkdir -p $(OBJDIR)/serial
	@[ -d $(OBJDIR)/GraphFrame ] || mkdir -p $(OBJDIR)/GraphFrame
	@[ -d $(OBJDIR)/CtrlFrame ] || mkdir -p $(OBJDIR)/CtrlFrame
	$(CXX) $(CFLAGS) -c $(WX-CONFIG_CXXFLAGS) -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	@[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
	$(CXX) $(CFLAGS) -o $(PROGRAM) $(OBJECTS) $(WX-CONFIG_LIBS)
	SetFile -t APPL $(PROGRAM)

clean:
	rm -rf $(OBJDIR)/*
	rm -f $(PROGRAM)

-include $(DEPENDS)

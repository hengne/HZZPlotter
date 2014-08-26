# Author Hengne Li @ UVa CERN 2014

CC = g++
INCDIR = ./include
EXEDIR = ./
SRCDIR = ./src

ROOTLIBS = -L$(ROOTSYS)/lib \
           -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d \
           -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics \
           -lMathCore -lThread -lMinuit -lMinuit2 -lpthread -lTreePlayer \
           -Wl,-rpath,$(ROOTSYS)/lib -stdlib=libc++ -lm -ldl 

ROOTCFLAGS = -m64 -I$(ROOTSYS)/include

CFLAGS = $(ROOTCFLAGS) -I$(INCDIR)
LIBS = $(ROOTLIBS)

DEPS = $(wildcard $(INCDIR)/*.hpp)
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(SRCDIR)/%.o,$(SRCS))

vpath %.hpp $(INCDIR)
vpath %.cpp $(SRCDIR)
vpath %.o   $(SRCDIR)
vpath %.exe $(EXEDIR)

# this group of exes use config.o
_EXES1 = plotter.exe

EXES1 = $(patsubst %.exe,$(EXEDIR)/%.exe,$(_EXES1))

all: $(EXES1) 

$(SRCDIR)/plotter.o: $(SRCDIR)/%.o: $(SRCDIR)/%.cpp config.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(SRCDIR)/config.o: $(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJS3):  $(SRCDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CC) -o $@ -c $< $(CFLAGS)


$(EXES1): $(EXEDIR)/%.exe: $(SRCDIR)/%.o $(SRCDIR)/config.o | $(EXEDIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(EXEDIR):
	mkdir $(EXEDIR)

clean: 
	\rm -f $(EXEDIR)/*.exe $(SRCDIR)/*.o




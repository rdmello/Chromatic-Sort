
# Set the paths for the compilation step
.PHONY: SRCDIR TARGETDIR RUNDIR LIBDIR SRCFILES OBJECTFILES STATICLIBFILE
SRCDIR = src
TARGETDIR = target
RUNDIR = runners
LIBDIR = lib
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTFILES = $(patsubst $(SRCDIR)/%.cpp, $(TARGETDIR)/%.o, $(SRCFILES))
STATICLIBFILE = $(LIBDIR)/PixelSort.a

# Magick++ setup 
.PHONY: MAGICKFLAGS 
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`

# Boost setup
.PHONY: BOOST_LIBS
BOOST_LIBS = -lboost_program_options

#
# MAIN MAKE COMMANDS
# 
.PHONY: all run clean libs build

all: libs

run: $(TARGETDIR)/$(file).run
	./$(TARGETDIR)/$(file).run

build: $(TARGETDIR)/$(file).run

$(STATICLIBFILE): $(OBJECTFILES) 
	ar rvs $(STATICLIBFILE) $(OBJECTFILES)

$(TARGETDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	g++ -c -Wall -Werror -std=c++14 -O3 $< -o $@ $(MAGICKFLAGS) -I $(SRCDIR) 

$(TARGETDIR)/%.run: $(RUNDIR)/%.cpp $(STATICLIBFILE)
	g++ -Wall -Werror -std=c++14 -O3 $< $(STATICLIBFILE) $(BOOST_LIBS) -I $(SRCDIR) -o $@ $(MAGICKFLAGS) 

clean:
	rm -f ./$(TARGETDIR)/*.run
	rm -f ./$(TARGETDIR)/*.o
	rm -f ./$(LIBDIR)/*.o
	rm -f ./$(LIBDIR)/*.a


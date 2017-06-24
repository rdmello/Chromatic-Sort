
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
.PHONY: MAGICKFLAGS MAGICK_LOCATION MAGICK_DIRECTIVES MAGICK_INCLUDE_FLAGS MAGICK_RUN_FLAGS
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
MAGICK_LOCATION = ../ImageMagick_builds/ImageMagick-7.0.6-0/ImageMagick-7.0.6-target
MAGICK_DIRECTIVES = -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 
MAGICK_INCLUDE_FLAGS = $(MAGICK_DIRECTIVES) -I $(MAGICK_LOCATION)/include/ImageMagick-7 
MAGICK_RUN_FLAGS = $(MAGICK_DIRECTIVES) -I $(MAGICK_LOCATION)/include/ImageMagick-7 -L $(MAGICK_LOCATION)/lib -lMagick++-7.Q16HDRI

# Boost setup
.PHONY: BOOST_LOCATION BOOST_INCLUDE_FLAGS BOOST_LIBS
BOOST_LOCATION = ../../../cpp/boost_test/boost_1_64_0_compiled
BOOST_INCLUDE_FLAGS = -I $(BOOST_LOCATION)/include
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
	g++ -c -Wall -Werror -std=c++14 -O3 $< -o $@ $(MAGICK_INCLUDE_FLAGS) -I $(SRCDIR) 

$(TARGETDIR)/%.run: $(RUNDIR)/%.cpp $(STATICLIBFILE)
	g++ -Wall -Werror -std=c++14 -O3 $< $(STATICLIBFILE) $(BOOST_LIBS) -I $(SRCDIR) -o $@ $(MAGICK_RUN_FLAGS) 

clean:
	rm -f ./$(TARGETDIR)/*.run
	rm -f ./$(TARGETDIR)/*.o
	rm -f ./$(LIBDIR)/*.o
	rm -f ./$(LIBDIR)/*.a


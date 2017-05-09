
# Set the paths for the compilation step
.PHONY: SRCDIR TARGETDIR RUNDIR LIBDIR SRCFILES OBJECTFILES 
SRCDIR = src
TARGETDIR = target
RUNDIR = runners
LIBDIR = lib
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTFILES = $(patsubst $(SRCDIR)/%.cpp, $(TARGETDIR)/%.o, $(SRCFILES))

# Magick++ setup 
.PHONY: MAGICKFLAGS
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
MAGICK_INCLUDE_FLAGS = -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/local/Cellar/imagemagick/7.0.4-10/include/ImageMagick-7 
MAGICK_RUN_FLAGS = -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/local/Cellar/imagemagick/7.0.4-10/include/ImageMagick-7 -L/usr/local/Cellar/imagemagick/7.0.4-10/lib -lMagick++-7.Q16HDRI

#
# MAIN MAKE COMMANDS
# 
.PHONY: all run clean libs build

all: libs

run: $(TARGETDIR)/$(file).run
	./$(TARGETDIR)/$(file).run

build: $(TARGETDIR)/$(file).run

libs: $(OBJECTFILES) 
	ar rvs $(LIBDIR)/PixelSort.a $(OBJECTFILES)

$(TARGETDIR)/%.o: $(SRCDIR)/%.cpp
	clang++ -c -Wall -Werror -std=c++14 -O3 $< -o $@ $(MAGICK_INCLUDE_FLAGS)

$(TARGETDIR)/%.run: $(RUNDIR)/%.cpp $(OBJECTFILES)
	clang++ -Wall -Werror -std=c++14 -O3 $(OBJECTFILES) $< -o $@ $(MAGICK_RUN_FLAGS)

clean:
	rm -f ./$(TARGETDIR)/*.run
	rm -f ./$(TARGETDIR)/*.o
	rm -f ./$(LIBDIR)/*.o
	rm -f ./$(LIBDIR)/*.a



.PHONY: MAGICKFLAGS
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`
MAGICKFLAGS2 = -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -I/usr/local/Cellar/imagemagick/7.0.4-10/include/ImageMagick-7 -L/usr/local/Cellar/imagemagick/7.0.4-10/lib -lMagick++-7.Q16HDRI

.PHONY: SRCDIR TARGETDIR SRCFILES TARGETFILES
SRCDIR = src
TARGETDIR = target
RUNDIR = runners
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
TARGETFILES = $(patsubst $(SRCDIR)/%.cpp, $(TARGETDIR)/%.run, $(SRCFILES))

.PHONY: all run clean

all: $(TARGETDIR)/test.run
	./$(TARGETDIR)/test.run

run: $(TARGETDIR)/$(file).run
	./$(TARGETDIR)/$(file).run

$(TARGETDIR)/%.run: $(RUNDIR)/%.cpp $(SRCFILES)
	g++ -std=c++14 $(SRCFILES) $< -o $@ $(MAGICKFLAGS)

clean:
	rm ./$(TARGETDIR)/*.run


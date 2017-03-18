
.PHONY: IMFLAGS
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`

.PHONY: SRCDIR TARGETDIR SRCFILES TARGETFILES
SRCDIR = src
TARGETDIR = target
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
TARGETFILES = $(patsubst $(SRCDIR)/%.cpp, $(TARGETDIR)/%.run, $(SRCFILES))

.PHONY: all run clean
all: $(TARGETFILES)

run: $(TARGETDIR)/$(file).run
	./$(TARGETDIR)/$(file).run

$(TARGETDIR)/%.run: $(SRCDIR)/%.cpp
	g++ $< -o $@ $(MAGICKFLAGS)

clean:
	rm ./$(TARGETDIR)/*.run


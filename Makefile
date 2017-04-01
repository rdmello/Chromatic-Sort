
.PHONY: IMFLAGS
MAGICKFLAGS = `Magick++-config --cppflags --cxxflags --ldflags --libs`

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


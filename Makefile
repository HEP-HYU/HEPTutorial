# Makefile for HEP Tutorial

C=c++
LD=c++
RDICT = rootcling -v0
CXXFLAGS += $(shell root-config --cflags) -fPIC -I.
LDFLAGS += $(shell root-config --glibs)

SRC1= MyAnalysis.o MyJet.o MyMuon.o MyElectron.o MyPhoton.o Plotter.o

%.o: %.C %.h
		$(C) -o $@ -c $(CXXFLAGS) $<

all: libMyAnalysis.so LinkDef.cxx

LinkDef.cxx: LinkDef.h
	$(RDICT) -f $@ -c MyAnalysis.h Plotter.h $<

libMyAnalysis.so: LinkDef.cxx $(SRC1)
	$(LD) -o $@ -shared $(CXXFLAGS) $(LDFLAGS) $(SRC1) $<

clean:
	@rm -f *.so *.o *.cxx *.pcm

# Makefile

CXXFLAGS = -g -O2 -Wall -Wuninitialized

# optional ZLIB library

CXXFLAGS += -DHAVE_ZLIB

# ROOT libraries

ifdef ROOTSYS
ROOTGLIBS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs) -lXMLParser -lXMLIO -lThread
HAVE_RHTTP = $(shell $(ROOTSYS)/bin/root-config --has-http)
ifeq ($(HAVE_RHTTP),yes)
ROOTGLIBS += -lRHTTP
endif
CXXFLAGS += -DHAVE_ROOT $(shell $(ROOTSYS)/bin/root-config --cflags)
endif

ifdef MIDASSYS
MIDASLIBS = $(MIDASSYS)/linux/lib/libmidas.a -lutil -lrt
CXXFLAGS += -DHAVE_MIDAS -DOS_LINUX -Dextname -I$(MIDASSYS)/include

UNAME=$(shell uname)
RTLIB=
ifeq ($(UNAME),Darwin)
#CXXFLAGS += -DOS_LINUX -DOS_DARWIN
MIDASLIBS = $(MIDASSYS)/darwin/lib/libmidas.a
#RPATH=
else
RTLIB=-lrt
endif

endif

ifdef ROOTSYS
CXXFLAGS += -DHAVE_LIBNETDIRECTORY -IlibNetDirectory
endif


# Check for ROOTANASYS env variable.
ifdef ROOTANASYS
ROOTANAINC = -I$(ROOTANASYS)/include
ROOTANALIBS = $(ROOTANASYS)/lib/librootana.a
else
ROOTANAINC = -I../include
ROOTANALIBS = ../lib/librootana.a
endif

OBJS:=
#OBJS += TV792Histogram.o TV1190Histogram.o
#OBJS += TL2249Histogram.o TAgilentHistogram.o
#OBJS += TV1720Waveform.o TDT724Waveform.o
#OBJS += TV1730DppWaveform.o TV1730RawWaveform.o
#OBJS += TV1720Correlations.o
#OBJS += TAnaManager.o

all:: $(OBJS)
#all:: ana.exe
#all:: analyzer.exe
#all:: anaDisplay.exe
#all:: midas2root.exe
all:: emmana.exe

#emma_module.o: v1190unpack.h
#Unpack.o: Unpack.h Alpha16.h
#anaDisplay.o: Alpha16.o

#OBJS += v1190unpack.o
OBJS += mesadc32unpack.o

emmana.exe: $(OBJS) manalyzer.o manalyzer_main.o emma_module.o
	$(CXX) -o $@ $(CXXFLAGS) $(ROOTANAINC) $^ $(ROOTANALIBS) $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread -lssl -lutil

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(ROOTANAINC) -c $<

dox:
	doxygen

clean::
	-rm -f *.o *.a *.exe

# end

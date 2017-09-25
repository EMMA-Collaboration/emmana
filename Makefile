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
CXXFLAGS += -DHAVE_MIDAS -DOS_LINUX -Dextname -I$(MIDASSYS)/include -Iinclude

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

BUILD		:= $(PWD)/build
# LDIR		:= $(PWD)/lib
# CINT		:= $(PWD)/cint
SDIR		:= $(PWD)/src
HDIR		:= $(PWD)/include
BINDIR      := $(PWD)/bin

SRCEXT		:= cxx
SRCS      	:= $(shell find $(SDIR) -type f -name *.$(SRCEXT))
OBJS      	:= $(patsubst $(SDIR)/%, $(BUILD)/%, $(SRCS:.$(SRCEXT)=.o))
# HDRS      	:= $(patsubst $(SDIR)/%, $(HDIR)/%, $(SRCS:.$(SRCEXT)=.hxx))

BIN         := $(BINDIR)/emmana.exe


############# TARGETS #############
all: $(OBJS) $(BIN)

$(OBJS): | $(BUILD)

$(BUILD):
	mkdir $(BUILD)

$(BINDIR):
	mkdir $(BINDIR)


############# Rules #############
$(BUILD)/%.o: $(SDIR)/%.cxx
	$(CXX) $(CXXFLAGS) $(ROOTANAINC) -c -o $@ $<

$(BIN): $(OBJS) | $(BINDIR)
	$(CXX) -o $@ $(CXXFLAGS) $(ROOTANAINC) $^ $(ROOTANALIBS) $(MIDASLIBS) $(ROOTGLIBS) -lm -lz -lpthread -lssl -lutil

dox:
	doxygen

clean::
	rm -f $(BUILD)/* $(BINDIR)/*

# end

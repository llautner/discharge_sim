# name := GEMsim
# G4TARGET := $(name)
# G4EXLIB := true
# 
# .PHONY: all
# all: lib bin
# 
# CPPFLAGS        := $(shell root-config --cflags)
# ROOTLIBS         := $(shell root-config --nonew --libs)
# EXTRALIBS       += $(ROOTLIBS)
# include $(G4INSTALL)/config/binmake.gmk
# 
# visclean:
# rm -f g4*.prim g4*.eps g4*.wrl
# rm -f .DAWN_*

# NEW MAKEFILE WITH CLASS SUPPORT

name := GEMsim
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: dict.cxx lib bin

dict.cxx :
	rootcint -f dict.cxx -c include/GEMEvent.hpp include/LinkDef.h
	
myclean :
	rm -rf dict.* $(G4TMP)/$(G4SYSTEM)/GEMsim

include $(G4INSTALL)/config/architecture.gmk

G4NOHIST := true
CPPFLAGS += $(shell root-config --cflags)
# Here comes dict.cxx
LDFLAGS  += $(shell root-config --libs) dict.cxx

include $(G4INSTALL)/config/binmake.gmk 

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
EXE=Parking

COMP=g++
COMP_FLAGS=-Wall

EDL=g++
EDL_FLAGS=

CLEAN=clean
DEV=dev

LIBS=-lncurse -ltcl -ltp
LIB_PATH=-L /shares/public/tp/tp-multitache/

INC=
INC_PATH=-I /shares/public/tp/tp-multitache/

RM=rm
RM_FLAGS=-f
ECHO=echo

INT=Mother.h

REAL=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)

.PHONY: $(CLEAN) $(DEV)

$(EXE): $(OBJ)
	$(EDL) $(EDL_FLAGS) $(LIB_PATH) -o $(EXE) $(OBJ) $(LIBS)
	$(RM) $(RM_FLAGS) $(OBJ)

$(DEV): INC_PATH=-I /Users/gustavemonod/Desktop/
$(DEV): LIB_PATH=-L /Users/gustavemonod/Desktop/
$(DEV): $(EXE)

%.o: %.cpp %.h
	$(ECHO) Compiling $<...
	$(COMP) $< $(COMP_FLAGS) $(INC_PATH)

$(CLEAN):
	$(RM) $(RM_FLAGS) $(OBJ) $(EXE) core

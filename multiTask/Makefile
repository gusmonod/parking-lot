RM=rm
ECHO=echo
COMP=gcc
EDL=gcc
EXE=Parking
CLEAN=efface
COMP_FLAGS=-Wall
EDL_FLAGS=
LIBS=
LIB_PATH=
INC_PATH=
RM_FLAGS=-f

INT=Mother.h

REAL=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)

.PHONY: $(CLEAN)

$(EXE): $(OBJ)
	$(EDL) $(EDL_FLAGS) $(LIB_PATH) -o $(EXE) $(OBJ) $(LIBS)
	$(RM) $(RM_FLAGS) $(OBJ)

%.o: %.cpp %.h
	$(ECHO) Compiling $<...
	$(COMP) $(COMP_FLAGS) $(INC_PATH)

$(CLEAN):
	$(RM) $(RM_FLAGS) $(OBJ) $(EXE) core

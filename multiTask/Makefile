EXE=Parking

COMP=g++
COMP_FLAGS=-Wall

EDL=g++
EDL_FLAGS=

CLEAN=clean
DISPLAY=disp

LIBS=-lncurses -ltcl -ltp
LIB_PATH=-L /shares/public/tp/tp-multitache/

INC=
INC_PATH=-I /shares/public/tp/tp-multitache/

RM=rm
RM_FLAGS=-f
ECHO=echo

INT=Mother.h Keyboard.h EntranceDoor.h Information.h

REAL=$(INT:.h=.cpp)
OBJ=$(INT:.h=.o)

.PHONY: $(CLEAN) $(DISPLAY)

$(EXE): $(OBJ)
	$(EDL) $(EDL_FLAGS) $(LIB_PATH) -o $(EXE) $(OBJ) $(LIBS)
	$(RM) $(RM_FLAGS) $(OBJ)

$(DISPLAY): EXE=DisplayParking
$(DISPLAY): INT=Display.h Information.h
$(DISPLAY): REAL=Display.cpp Information.cpp
$(DISPLAY): OBJ=Display.o Information.o
$(DISPLAY):
	$(ECHO) Compiling $(REAL)...
	$(COMP) -c $(REAL) $(COMP_FLAGS) $(INC_PATH)
	$(EDL) $(EDL_FLAGS) $(LIB_PATH) -o $(EXE) $(OBJ) $(LIBS)
	$(RM) $(RM_FLAGS) $(OBJ)


%.o: %.cpp %.h
	$(ECHO) Compiling $<...
	$(COMP) -c $< $(COMP_FLAGS) $(INC_PATH)

$(CLEAN):
	$(RM) $(RM_FLAGS) $(OBJ) $(EXE) core

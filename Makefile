# ---------------------------------------------------------------------
# Objects
# ---------------------------------------------------------------------
PRG_PATH=.

ifeq ($(TEST), 1)
MAIN=$(PRG)
else
MAIN=concordance
endif

H_FOLDER=host_core

HEADERS_CORE=node.h\
	     token.h\
	     prefix_tree.h\
	     rb_tree.h\
	     input_data.h\
	     concordance_visitor.h\
	     factory_store.h\
	     parser.h\

HEADERS=$(HEADERS_CORE:%.h=${H_FOLDER}/%.h)

SOURCES=${MAIN}.cpp $(HEADERS:%.h=%.cpp) 
OBJECTS = $(SOURCES:%.cpp=%.o)

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------
CC = g++
#CC = clang++
CCOPT =    # -Wall -W -Wno-sign-compare -ansi -pedantic -Werror -Wno-deprecated

## debug, don't optimize 
# -m64: generate code for 64 bit env. Use -m32 for a 32 bit env.
# -DIL_STD -> #define IL_STD 1. Used to enable some STL features
CCOPT   += -m64 -DIL_STD

## debug, optimize
CCOPT += -O3 -std=c++0x 

vpath %.o obj

## lib dirs -L...
CCLNDIRS= 
## include dirs -I...
INCLDIRS = -I$(PRG_PATH) -I$(PRG_PATH)/$(H_FOLDER) 

## Compiler Flags
CCFLAGS = $(CCOPT) $(INCLDIRS)


all:	concordance

concordance: $(OBJECTS) 
	     $(CC) $(CCLNDIRS) $(DFLAGS) -o $(MAIN) \
	     $(OBJECTS:%=$(PRG_PATH)/obj/%)

$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CCFLAGS) $< -o $(PRG_PATH)/obj/$@

clean:
	rm -f obj/host_core/*.o
	rm -f obj/$(MAIN).o	
	rm -f $(MAIN)	


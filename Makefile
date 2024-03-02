#
# pgpasswd Makefile
#
IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc
EDIR=$(shell pg_config --bindir)

EXE = pgpasswd
OBJ = pgpasswd.o
SRC = pgpasswd.c

all: pgpasswd

$(EXE): $(OBJ) 
	$(CC) $(INCLUDES) $(LINKLIBS) -lpq -o $(EXE) $(OBJ)
$(OBJ) : $(SRC)
	$(CC) $(INCLUDES) $(INCLUDES)/server -c $(SRC) -Wall

install:
	install	$(EXE) $(EDIR)
clean distclean maintainer-clean:
	rm -f pgpasswd *.o


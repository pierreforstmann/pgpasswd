#
# pgpasswd Makefile
#
IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc

EXE = pgpasswd
OBJ = pgpasswd.o
SRC = pgpasswd.c

all: pgpasswd

pgpasswd: $(OBJ) 
	$(CC) $(INCLUDES) $(LINKLIBS) -lpq -o $(EXE) $(OBJ)
$(OBJ) : $(SRC)
	$(CC) $(INCLUDES) $(INCLUDES)/server -c $(SRC) -Wall

install:
	$(INSTALL_PROGRAM) pgpasswd '$(DESTDIR)$(bindir)'

clean distclean maintainer-clean:
	rm -f pgpasswd *.o


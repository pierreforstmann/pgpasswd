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
	$(CC) $(INCLUDES) -static -o $(EXE) $(OBJ) $(LINKLIBS) -lpq -lpgcommon -lpgport -lpthread -lpgcommon_shlib -L/usr/lib64 -lc -lm  -Wall
$(OBJ) : $(SRC)
	$(CC) $(INCLUDES) $(INCLUDES)/server -c $(SRC) -Wall

install:
	install	$(EXE) $(EDIR)
clean distclean maintainer-clean:
	rm -f pgpasswd *.o

pgxn:
	git archive --format zip  --output ../pgxn/pgpasswd/pgpasswd-0.0.1.zip main

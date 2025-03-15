#
# pgpasswd Makefile
#
IDIR=$(shell pg_config --includedir)
INCLUDES=-I$(IDIR)
LDIR=$(shell pg_config --libdir)
LINKLIBS=-L$(LDIR)
CC=gcc
EDIR=$(shell pg_config --bindir)

EXE1 = chpasswd 
OBJ1 = chpasswd.o
SRC1 = chpasswd.c

EXE2 = resetpasswd 
OBJ2 = resetpasswd.o
SRC2 = resetpasswd.c

all: chpasswd resetpasswd

$(EXE1): $(OBJ1) 
	$(CC) $(INCLUDES) -static -o $(EXE1) $(OBJ1) $(LINKLIBS) -lpq -lpgcommon -lpgport -lpthread -lpgcommon_shlib -L/usr/lib64 -lc -lm  -Wall
$(OBJ1) : $(SRC1)
	$(CC) $(INCLUDES) $(INCLUDES)/server -c $(SRC1) -Wall

$(EXE2): $(OBJ2) 
	$(CC) $(INCLUDES) -static -o $(EXE2) $(OBJ2) $(LINKLIBS) -lpq -lpgcommon -lpgport -lpthread -lpgcommon_shlib -L/usr/lib64 -lc -lm  -Wall
$(OBJ2) : $(SRC2)
	$(CC) $(INCLUDES) $(INCLUDES)/server -c $(SRC2) -Wall

install:
	install	$(EXE1) $(EDIR)
	install	$(EXE2) $(EDIR)
clean distclean maintainer-clean:
	rm -f *passwd *.o

pgxn:
	git archive --format zip  --output ../pgxn/pgpasswd/pgpasswd-0.0.1.zip main

# pgpasswd Makefile
# from src/test/examples/Makefile and psql Makefile

subdir = contrib/pgpasswd 
top_builddir = ../..
include $(top_builddir)/src/Makefile.global

ifeq ($(PORTNAME), win32)
LDFLAGS += -lws2_32
endif

override CPPFLAGS := -I. -I$(srcdir) -I$(libpq_srcdir) $(CPPFLAGS)
LDFLAGS_INTERNAL += $(libpq_pgport)

OBJS = pgpasswd.o

all: pgpasswd

pgpasswd: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDFLAGS_EX) $(LIBS) -o $@$(X)

install:
	$(INSTALL_PROGRAM) pgpasswd '$(DESTDIR)$(bindir)'


clean distclean maintainer-clean:
	rm -f pgpasswd *.o


include ../Makefile.common

.PHONY : clean all test

PROG= \
$(OBJDIR)/libpng.a


LIBSRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)



OPT  = $(CFLAGS) -I../zlib

OBJ   = $(patsubst %.c, $(OBJDIR)/%.o  , $(LIBSRC) ) 

all: $(PROG)


$(PROG):$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

$(OBJDIR)/%.o:%.c  $(HEADERS)
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR) ;fi
	$(CC) -c -o $@ $< $(OPT)
clean:
	-rm -f $(PROGS) */*.o */*.a  *~

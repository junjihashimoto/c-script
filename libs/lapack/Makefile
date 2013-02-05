include ../Makefile.common

.PHONY : clean all test


PROGS= \
liblapack.a 

LIBSRC=$(wildcard *.f)


#OPT  = $(CFLAGS)
OPT  = -O4 -DNDEBUG

OBJ   = $(patsubst %.f, $(OBJDIR)/%.o  , $(LIBSRC) ) 

all: $(PROGS)


libblas.a:$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

$(OBJDIR)/%.o:%.f
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR) ;fi
	$(FC) -c -o $@ $< $(OPT)

clean:
	-rm -f $(PROGS) */*.o */*.a  *~

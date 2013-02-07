include ../Makefile.common

.PHONY : clean all test

ifdef WIN32
OBJDIR=win32
else
OBJDIR=linux
endif

PROG= \
$(OBJDIR)/libthread.a


LIBSRC=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)



OPT  = $(CFLAGS)

OBJ   = $(patsubst %.cpp, $(OBJDIR)/%.o  , $(LIBSRC) ) 

all: $(PROG)


$(PROG):$(OBJ)
	rm -f $@
	$(AR) cru $@ $^
	$(RANLIB) $@

$(OBJDIR)/%.o:%.cpp  $(HEADERS)
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR) ;fi
	$(CXX) -c -o $@ $< $(OPT)
clean:
	-rm -f $(PROGS) */*.o */*.a  *~

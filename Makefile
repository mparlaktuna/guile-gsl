INCL   = guile_gsl.h
SRC    = main.c guile_gsl.c
OBJ    = $(SRC:.c=.o)
LIBS   = -lgsl -lgslcblas -lm
EXE    = guile_gsl
FNS		 = 

# compiler, linker defines
CC      = gcc
CFLAGS  = -Wall -O2 -g `guile-config compile`
LIBPATH =	-L/usr/local/shared/gsl/1.11/32/lib/
LDFLAGS = $(LIBPATH) $(LIBS) -o $(EXE) `guile-config link`
CFDEBUG = -ansi -pedantic -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

# compile and assemble C source files into object files
%.o: %.c
				$(CC) -I/usr/local/shared/gsl/1.11/32/include -c $(CFLAGS) $*.c
 
# link all object files with external libraries into binaries
$(EXE): $(OBJ)
				$(CC) $(LDFLAGS) $(OBJ)

# objects depend on these libraries
$(OBJ): $(INCL)

# clean up objects, output
clean:
				$(RM) $(OBJ) $(FNS)

# clean up objects, output, executable
clobber:
				$(RM) $(OBJ) $(EXE) $(FNS)

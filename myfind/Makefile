##
## @file Makefile
## Betriebssysteme myFind Makefile
## Beispiel 0
##
## @author Adam Kerenyi <ic14b080@technikum-wien.at>
## @author Romeo Beck <ic14b037@technikum-wien.at>
## @author Thomas Zeitinger <ic14b033@technikum-wien.at>
## @date 2015/02/09
##
## @version $Revision: 972 $
##
## @todo
##
## URL: $HeadURL: https://svn.petrovitsch.priv.at/ICSS-BES/trunk/2015/CIS/lu/myFind/Makefile $
##
## Last Modified: $Author: tom $
##

##
## ------------------------------------------------------------- variables --
##

CC=gcc
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3
CP=cp
CD=cd
MV=mv
GREP=grep
DOXYGEN=doxygen

OBJECTS=myFind.o

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
        $(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: myFind

myFind: $(OBJECTS)
        $(CC) $(CFLAGS) -o $@ $^

clean:
        $(RM) *.o *~ myFind

distclean: clean
        $(RM) -r doc

doc: html pdf

html:
        $(DOXYGEN) doxygen.dcf

pdf: html
        $(CD) doc/pdf && \
        $(MV) refman.tex refman_save.tex && \
        $(GREP) -v $(EXCLUDE_PATTERN) refman_save.tex > refman.tex && \
        $(RM) refman_save.tex && \
        make && \
        $(MV) refman.pdf refman.save && \
        $(RM) *.pdf *.html *.tex *.aux *.sty *.log *.eps *.out *.ind *.idx \
              *.ilg *.toc *.tps Makefile && \
        $(MV) refman.save refman.pdf

##
## ---------------------------------------------------------- dependencies --
##

##
## =================================================================== eof ==
##

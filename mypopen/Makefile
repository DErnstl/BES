##
## @file Makefile
## Betriebssysteme mypopen Makefile
## Beispiel 1
##
## @author Adam Kerenyi <ic14b080@technikum-wien.at>
## @author Romeo Beck <ic14b037@technikum-wien.at>
## @author Thomas Zeitinger <ic14b033@technikum-wien.at>
## @date 2015/02/09
##
## @version $Revision: 972 $
##
##

##
## ------------------------------------------------------------- variables --
##

CC=gcc
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra -Wstrict-prototypes -fno-common -g -O3 
CP=cp
CD=cd
MV=mv
RM=rm
GREP=grep
DOXYGEN=doxygen

OBJECTS=mypopen.o

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: test-pipe

test-pipe: test-pipe.o mypopen.o
	$(CC) $(OPTFLAGS) test-pipe.o mypopen.o -o test-pipe

clean:
	$(RM) *.o *.h.gch test-pipe

clean_doc:
	$(RM) -r doc/

doc: clean_doc
	$(DOXYGEN)

clean_all: clean clean_doc


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


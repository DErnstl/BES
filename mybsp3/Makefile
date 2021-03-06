##
## @file Makefile
## Betriebssysteme myfind
## Beispiel 3
##
## @author Adam Kerenyi <ic14b080@technikum-wien.at>
## @author Romeo Beck <ic14b037@technikum-wien.at>
## @author Thomas Zeitinger <ic14b033@technikum-wien.at>
## @date 2015/06/05
## @version 100
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

OBJECTS=mybsp3.o

EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

all: mybsp3 sender empfaenger

mybsp3: 
	$(CC) $(CFLAGS) -c mybsp3.c -lsem182 $^

sender:
	$(CC) $(CFLAGS) -o sender sender.c mybsp3.o -lsem182

empfaenger:
	$(CC) $(CFLAGS) -o empfaenger empfaenger.c mybsp3.o -lsem182

clean:
##	$(RM) *.o sender empfaenger

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


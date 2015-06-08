##
## @file Makefile
## Betriebsysteme Shared Memory C Makefile
## Beispiel 3
##
##
## @author Stammgruppe 20
## @author Zuebide Sayici 	ic14b002
## @author Kurt Liebenberg 	ic14b062
## @author Markus Wals		ic14b076
## @author Manfred Lackner  ic13b071
##
## @date 2015/06/05
##
## URL: $HeadURL$
##
## Last Modified: $Author: Lackner $
##
##
## ------------------------------------------------------------- variables --
##

CC=gcc
CFLAGS=-DDEBUG -Wall -pedantic -Werror -Wextra
CSEM182=-lsem182

CP=cp
CD=cd
MV=mv
GREP=grep
## DOXYGEN=/Applications/Doxygen.app/Contents/Resources/doxygen ## MacOS
DOXYGEN=doxygen

SENDER=sender
EMPFAENGER=empfaenger

OBJECTS_SENDER=$(SENDER).o sender.o prog.o bsp3lib.o ringbuffer.o
OBJECTS_EMPFAENGER=empfaenger.o prog.o bsp3lib.o ringbuffer.o
EXCLUDE_PATTERN=footrulewidth

##
## ----------------------------------------------------------------- rules --
##

all: $(SENDER) $(EMPFAENGER)

$(SENDER).o: main.c
	$(CC) $(CFLAGS) -c $< -o $(SENDER).o -DSENDER

$(EMPFAENGER).o: main.c
	$(CC) $(CFLAGS) -c $< -o $(EMPFAENGER).o -DEMPFAENGER

%.o: %.c
	$(CC) $(CFLAGS) -c $<

##
## --------------------------------------------------------------- targets --
##

$(SENDER): $(OBJECTS_SENDER) 
	$(CC) $(CFLAGS) -o $@ $^ $(CSEM182)

$(EMPFAENGER): $(OBJECTS_EMPFAENGER)
	$(CC) $(CFLAGS) -o $@ $^ $(CSEM182)

clean:
	$(RM) *.o *~ $(SENDER) $(EMPFAENGER)

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

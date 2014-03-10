# $Id: Makefile,v 1.1 2014/03/10 21:44:36 luis Exp $
# Author: Luis Colorado <lc@luiscoloradosistemas.com>
# Date: Mon Mar 10 22:37:15 CET 2014
# Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
#			All rights reserved.

RM= rm -f
targets = regr_polin gauss

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(targets) $(objects)

regr_polin_objs = regr_polin.o
regr_polin: $(regr_polin_objs)
	$(CC) $(LDFLAGS) -o regr_polin $(regr_polin_objs)

gauss_objs = gauss.o
gauss: $(gauss_objs)
	$(CC) $(LDFLAGS) -o gauss $(gauss_objs)

objects = $(regr_polin_objs) $(gauss_objs)

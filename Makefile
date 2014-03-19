# $Id: Makefile,v 1.3 2014/03/19 14:24:33 luis Exp $
# Author: Luis Colorado <lc@luiscoloradosistemas.com>
# Date: Mon Mar 10 22:37:15 CET 2014
# Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
#			All rights reserved.

RM=rm -f

targets = regr_polin sist_lin

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(targets) $(objects)

regr_polin_objs = regr_polin.o gauss.o
regr_polin: $(regr_polin_objs)
	$(CC) $(LDFLAGS) -o regr_polin $(regr_polin_objs)

sist_lin_objs = gauss.o sist_lin.o
sist_lin: $(sist_lin_objs)
	$(CC) $(LDFLAGS) -o sist_lin $(sist_lin_objs)

objects = $(regr_polin_objs) $(sist_lin_objs)

regr_polin.o sist_lin.o gauss.o: gauss.h
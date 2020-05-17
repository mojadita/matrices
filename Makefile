# $Id: Makefile,v 1.4 2014/04/01 17:22:44 luis Exp $
# Author: Luis Colorado <lc@luiscoloradosistemas.com>
# Date: Mon Mar 10 22:37:15 CET 2014
# Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
#			All rights reserved.

RM=rm -f

targets = regr_polin sist_lin gram_schmidt cos
toclean = $(targets)

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(toclean)

cos_deps              =
cos_objs			  = cos.o
cos_ldflags			  =
cos_libs			  = -lm
cos: $(cos_deps) $(cos_objs)
	$(CC) $(LDFLAGS) $($@_ldflags) -o $@ $($@_objs) $($@_libs)
toclean				 += $(cos_objs)

regr_polin_deps		  =
regr_polin_objs		  = regr_polin.o gauss.o io.o
regr_polin_ldflags	  =
regr_polin_libs		  =
regr_polin: $(regr_polin_deps) $(regr_polin_objs)
	$(CC) $(LDFLAGS) $($@_ldflags) -o $@ $($@_objs) $($@_libs)
toclean              += $(regr_polin_objs)

sist_lin_deps		  =
sist_lin_objs		  = gauss.o sist_lin.o io.o
sist_lin_ldflags	  =
sist_lin_libs		  =
sist_lin: $(sist_lin_deps) $(sist_lin_objs)
	$(CC) $(LDFLAGS) $($@_ldflags) -o $@ $($@_objs) $($@_libs)
toclean            	 += $(sist_lin_objs)

gram_schmidt_deps	  = 
gram_schmidt_objs	  = gram_schmidt.o io.o
gram_schmidt_ldflags  =
gram_schmidt_libs	  =
gram_schmidt: $(gram_schmidt_deps) $(gram_schmidt_objs)
	$(CC) $(LDFLAGS) $($@_ldflags) -o $@ $($@_objs) $($@_libs)
toclean           	 += $(gram_schmidt_objs)

objects = $(regr_polin_objs) $(sist_lin_objs)

regr_polin.o sist_lin.o gauss.o: gauss.h io.h

# Makefile for source rpm: gnutls
# $Id$
NAME := gnutls
SPECFILE = $(firstword $(wildcard *.spec))

include ../common/Makefile.common

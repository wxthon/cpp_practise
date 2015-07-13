
AUTOMAKE_OPTIONS = foreign
SUBDIRS =

# what flags you want to pass to the C compiler & linker
CFLAGS = -Wall -std=C99 -O2
CPPFLAGS = -Wall -std=c++14
LDFLAGS =
LIBS = -lgtest

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = lambda_demo 	type_traits_demo cast_demo

lambda_demo_SOURCES = lambda.cc
type_traits_demo_SOURCES = type_traits.cc
cast_demo_SOURCES = cast.cc

AUTOMAKE_OPTIONS = foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = -std=c++11
AM_CPPFLAGS = -I$(top_srcdir)/include
EXTRA_DIST = \
  CMakeLists.txt \
  README.md

lib_LTLIBRARIES = libvhdf.la
libvhdf_la_LDFLAGS = -shared
libvhdf_la_SOURCES = src/vhdf.cpp
include_HEADERS = include/vhdf.hpp
libvhdf_INCLUDES = include

bin_PROGRAMS = tests
tests_SOURCES = test/main.cpp
tests_LDADD = libvhdf.la
ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = gnu 

AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_DATE_TIME_LIB) -llog4cxx -lcppunit
AM_CXXFLAGS = -DLOG4BITS $(BOOST_CPPFLAGS)  -Wall  
AM_CXXFLAGS += -g -I$(top_srcdir)/bits++ -I$(builddir)/bits++

check_PROGRAMS= test_tsmat test_matrix test_column  test_periods  test test_base test_parse 

pkginclude_HEADERS = tsmat.h tsmat.cch tsidx.h tsparse.cch  tsparse.h matrix.h matrix.cch log4bits

test_SOURCES=  test.cpp
test_base_SOURCES=  test_base.cpp
test_periods_SOURCES= test_periods.cpp tsidx.h
test_tsmat_SOURCES= test_tsmat.cpp tsmat.cch tsmat.h tsidx.h matrix.h matrix.cch 
test_parse_SOURCES= test_parse.cpp tsparse.cch tsparse.h tsmat.cch tsmat.h tsidx.h matrix.h matrix.cch
test_matrix_SOURCES= test_matrix.cpp matrix.h matrix.cch
test_column_SOURCES= test_column.cpp matrix.h matrix.cch

TESTS=test_tsmat test_matrix test_column

EXTRA_DIST = go_test.sh






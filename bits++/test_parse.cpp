/*

  This file is part of BITS++ -  C++ Banca d'Italia Time Series project

  Copyright (c) 2005-2012 Banca d'Italia - Area Ricerca Economica e
                          Relazioni Internazionali

  Author: Emmanuele Somma (emmanuele_DOT_somma_AT_bancaditalia_DOT_it)
          Area Ricerca Economica e Relazioni Internazionali 
          Banca d'Italia
		  
  Any parts of this program derived from the xMule, lMule or eMule project,
  or contributed by third-party developers are copyrighted by their
  respective authors.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA

*/

#include <boost/config.hpp>                      // for BOOST_MSVC
#include <boost/cstdlib.hpp>                     // for boost::exit_success
#include <boost/operators.hpp>                   // for the tested items
#include <boost/random/linear_congruential.hpp>  // for boost::minstd_rand

#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>  // for std::cout (std::endl indirectly)
#include <algorithm>  
#include <numeric>  
#include <functional> 

#include "tsidx.h"
#include "matrix.h"
#include "tsmat.h"
#include "tsparse.h"
#include "log4bits"

using namespace bits;

#define PRIVATE_EXPR_TEST(e, t)  CPPUNIT_ASSERT( ((e), (t)) )

// Generate unique random numbers from 0 to mod:

namespace bits {
template<class T> T sqrt(T i) {return i*i;}
//template<class T> void look(T& t) { cout << t << endl;  }
}

struct linear { 
  linear(double d) : D(d), acc(0.0) {} 
  linear(double i, double d) : D(d), acc(i) {} 
   double D; 
  double acc;
  double operator()() { acc+=D; return acc; }
 };

template<class Operation>
void transform(vector<column>& c, const Operation& op)
{
  transform(c.begin(), c.end(), op);
}

template<class Operation>
void transform(matrix& m, const Operation& op)
{
  transform(m.begin(), m.end(), m.begin(), op);
} 


#define TESTNAME test_parse

class TESTNAME : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( TESTNAME );
  CPPUNIT_TEST( testParse1 ) ;
  CPPUNIT_TEST( testParse2 ) ;
  CPPUNIT_TEST( testParse3 ) ;
  CPPUNIT_TEST_SUITE_END();

private:
  LOGGER4BITS;

   matrix v;         // My wonderful data matrix
   matrix w;

public:
  void tearDown() {}

  void setUp()
  {
   using std::cout;
   using std::endl;

   tseries t1;                    // This' a void timeseries
   tseries t2(1980,1,daily);      // This' an empty timeseries
   const int sz = 20;             // Supporting vectors
   column a(sz), b(sz), c(sz);
   ::linear L(2.0);

   generate_n(a.begin(),a.size(), L); // Generate random vector
   CPPUNIT_ASSERT(std::accumulate(a.begin(),a.end(),0.0,plus<double>()) == 420);
   v.push_back(a);   // Add a column
   transform (a.begin(), a.end(), b.begin(), bits::sqrt<double>); // squared copy on a new column
   CPPUNIT_ASSERT(std::accumulate(b.begin(),b.end(),0.0,plus<double>()) == 11480 );
   v.push_back(b); // and add to the matrix
   CPPUNIT_ASSERT(a < b); // Column a must be smaller than b


   transform(a.begin(), a.end(), b.begin(), b.begin(), std::plus<double>()); 
   v.push_back(b); // Third column a+b

   for_each(c.begin(), c.end(), uniform<double>(7.2));
   v.push_back(c); // Fourth column U(7.2)

   generate_n(a.begin(),a.size(), ::linear(2.0,0.0)); // Generate random vector
   w.push_back(a);
   w.push_back(a);
   w.push_back(a);
   w.push_back(a);
  }

  void testParse1()
  {
	// Set up some useful timeseries
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	tseries t3x1 = tsparse(t3,"TS1=TS1;TS2=TS2;A4=TS1+TS2");
  }

  void testParse2()
  {
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)

	tseries t3x2 = tsparse(t3,"TSK=LAG(TS1);TSK2=LAG(TSK,2)");
	double d = t3x2.value(t3x2.end()-1,2);
	CPPUNIT_ASSERT(d==1600);

	d = t3x2.value(t3x2.start+1,1);
	CPPUNIT_ASSERT(d==4);

  }

  void testParse3()
  {
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	tseries t3x = tsparse(t3,"TS0=TS0;TS1=TS1;"
						  "A=1.1*TS1;B=2.0+TS0;C=TS0+TS1;A2=TS0*(TS2+TS3+TS1);A4=A+1");
  }
};


LOGGERDEF(matrix);
LOGGERDEF(tsidx);
LOGGERDEF(tseries);
LOGGERDEF(TESTNAME);

int main(int argc, char**argv) 
{
  PropertyConfigurator::configure(LOGGERXQUOTE(TESTNAME)".log.conf");
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( TESTNAME::suite() );
  runner.run();
  return 0;
}

  

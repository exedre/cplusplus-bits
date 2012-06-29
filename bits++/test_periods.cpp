/*

  This file is part of BITS++ -  C++ Banca d'Italia Time Series project

  Copyright (c) 2005-2012 Banca d'Italia - Area Ricerca Economica e
                          Relazioni Internazionali

  Author: Emmanuele Somma (emmanuele_DOT_somma_AT_bancaditalia_DOT_it)
          Area Ricerca Economica e Relazioni Internazionali 
          Banca d'Italia
		  
  Any parts of this program derived from the project,
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

#define BOOST_TEST_MAIN

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

#include "tsidx.h"
#include "log4bits"

#define PRIVATE_EXPR_TEST(e, t)  CPPUNIT_ASSERT( ((e), (t)) )

using namespace std;
using namespace bits;

using std::cout;
using std::endl;

namespace bits {
template<class T> T sqrt(T i) {return i*i;}
//template<class T> void look(T& t) { cout << t << endl;  }
}

#define TESTNAME test_periods

LoggerPtr logger(Logger::getLogger("test_periods"));

class TESTNAME : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( TESTNAME );
  CPPUNIT_TEST( testIFreq );
  CPPUNIT_TEST( testIPeriods );
  CPPUNIT_TEST( testCTOR1 );
  CPPUNIT_TEST( testB4Leap );
  CPPUNIT_TEST( testEqualFloat );
  CPPUNIT_TEST( testDaysCount1 );
  CPPUNIT_TEST( testDaysCount2 );
  CPPUNIT_TEST( testDaysCount3 );
  CPPUNIT_TEST( testDaysCount4 );
  CPPUNIT_TEST( testTests );
  CPPUNIT_TEST( testNormalize );
  CPPUNIT_TEST( testDistance1 );
  CPPUNIT_TEST( testDistance2 );
  CPPUNIT_TEST( testDistance3 );
  CPPUNIT_TEST( testLag );
  CPPUNIT_TEST( test2Date );
  CPPUNIT_TEST( testDistanceFrom );
  CPPUNIT_TEST( testPreincrement );
  CPPUNIT_TEST( testPostincrement );
  CPPUNIT_TEST( testCopy1 );
//   CPPUNIT_TEST( testPrependingValues );
//   CPPUNIT_TEST( testOperationsMatrixPlusScalar );
//   CPPUNIT_TEST( testOperationsMatrixMultColumn );
//   CPPUNIT_TEST( testOperationsMatrixMultMatrix );
//   CPPUNIT_TEST( testExtractValue );
//   CPPUNIT_TEST( testEquality );
//   CPPUNIT_TEST( testAddition );
  CPPUNIT_TEST_SUITE_END();

private:



public:
  void setUp()
  {
  }

  void tearDown() 
  {

  }

  void testIFreq()
  {
	CPPUNIT_ASSERT( ifreq(daily,2000) == 366    );
	CPPUNIT_ASSERT( ifreq(daily,1900) == 365    );
	CPPUNIT_ASSERT( ifreq(quarterly,2000) == 4  );
	CPPUNIT_ASSERT( ifreq(quarterly,1900) == 4  );
  }

  void testIPeriods()
  {
	CPPUNIT_ASSERT( iperiods(quarterly,1990,2000) == 40  );
	CPPUNIT_ASSERT( iperiods(monthly,1990,2000) == 120   );
	CPPUNIT_ASSERT( iperiods(daily,1990,2000) == 3653    );
	CPPUNIT_ASSERT( iperiods(quarterly,2004,2000) == -16 );
	CPPUNIT_ASSERT( iperiods(daily,2004,2000) == -1462   );
  }

  void testCTOR1()
  {
	CPPUNIT_ASSERT( tsidx(1981,59,unknown).good() == false );
	CPPUNIT_ASSERT_THROW( tsidx(1981,59,monthly).good() == false, outside_frequency ); 
	CPPUNIT_ASSERT_THROW( tsidx(0,59,daily).good() == false, outside_frequency      ); 
	CPPUNIT_ASSERT( tsidx(1981,59,daily).good() == true ); 
  }

  void testB4Leap()
  {
	CPPUNIT_ASSERT( tsidx(1981,59,daily).before_leap()== 0 ); // 28 Feb (non leap)
	CPPUNIT_ASSERT( tsidx(1981,60,daily).before_leap()== 0 ); // 01 Mar (non leap)
	CPPUNIT_ASSERT( tsidx(1980,59,daily).before_leap()==-1 ); // 28 Feb
	CPPUNIT_ASSERT( tsidx(1980,60,daily).before_leap()==-1 ); // 29 Feb
	CPPUNIT_ASSERT( tsidx(1980,61,daily).before_leap()== 1 ); // 01 Mar

	// Catch the error raised on tsidx out of range
	CPPUNIT_ASSERT_THROW(tsidx(1999,366,daily).before_leap(), outside_frequency);

  }

  void testEqualFloat()
  {
	int i=0;
	for(double d=1980.001;d<1980.367;d+=.001)
	  {
		++i;
		tsidx t;
		t=d;
		printf("%f %d (%d-%d) \n", d , i ,t.first, t.second);
		if (1)
		  {
			CPPUNIT_ASSERT(t.first == 1980 ); // 
			CPPUNIT_ASSERT(t.second ==   i ); // 
		  }
	  }
  }

  void testDaysCount1()
  {
	int j=0;
	int b=0;
	for(int y=1800;y<=2000;y++)
	  {
		b=tsidx(y, 60, daily).before_leap();
		j = j + b;
	  }

	CPPUNIT_ASSERT(j == -49);
  }

  void testDaysCount2()
  {
	int j=0;
	for(int p=1;p<=366;p++)
	  j+=tsidx(2004, p, daily).before_leap();

	CPPUNIT_ASSERT(j == 246);
  }

  void testDaysCount3()
  {
	int j=0;
	for(int p=1;p<=12;p++)
	  j +=  tsidx(2000,p,monthly).before_leap();
	CPPUNIT_ASSERT(j == 8);
  }

  void testDaysCount4()
  {
	int j=0;
	for(int p=1;p<=365;p++)
	  j += tsidx(1999,p,daily).before_leap();
	CPPUNIT_ASSERT(j == 0);
  }

  void testTests()
  {
    tsidx p1(monthly);
    p1 = 1980,2;
	tsidx p11(p1);
	tsidx p2(1989,2,monthly);


	CPPUNIT_ASSERT( p1 < p2 );
	CPPUNIT_ASSERT( p2 > p1 );
	CPPUNIT_ASSERT( p1 == p11 );
	CPPUNIT_ASSERT( p1 != p2 );
  }

  void testNormalize()
  {
    tsidx p1(monthly);
    p1 = 1980,2;
	//	cerr << "NORMALIZE" << endl;
	// 	for (int i=-26;i<26;i++)
	// 	  cerr << tsidx(0, i,12) << tsidx(0, i,12).normalize() << endl;
	//  cerr << "END NORMALIZE" << endl;
  }

  void testDistance1()
  {
	tsidx_distance p1d(12,9);
	// cerr << p1d << endl;
	// cerr << -p1d << endl;
  }

  void testDistance2()
  {
    tsidx p1(monthly);
    p1 = 1980,2;

	// Adding 25 tsidxs to (1980,1) move to  (1982,2)
	//
	CPPUNIT_ASSERT((p1 + 25 ) == tsidx(1982,3));	
	CPPUNIT_ASSERT((p1 + tsidx_distance(0,25)) == tsidx(1982,3));	
	CPPUNIT_ASSERT((p1 + tsidx_distance(-1,0)) == tsidx(1979,2));
	CPPUNIT_ASSERT((p1 + tsidx_distance(-1,-1)) == tsidx(1979,1));
  }

  void testDistance3()
  {
    tsidx p1(monthly);
    p1 = 1980,2;
	//	for(int i=-20;i<21;i++)
	//	  cerr << p1 << "-" << tsidx_distance(0,i) << " = " << p1 - tsidx_distance(0,i) << endl;
  }


  void testLag()
  {
	// cout << tsidx(1980,1,monthly).lag(14) << endl;
	CPPUNIT_ASSERT(tsidx(1980,1,monthly).lag(14) == tsidx(1981,3,monthly)) ;
  }

  void test2Date()
  {
	CPPUNIT_ASSERT(to_simple_string(tsidx(1980,1,monthly).to_date()) == "1980-Jan-01"); 
  }

  void testDistanceFrom()
  {
	// tsidx_distance
	// returns the distance between two timeseries indexes in a given frequency

	CPPUNIT_ASSERT(tsidx(1980, 1,monthly).distance_from(tsidx(1982, 9,monthly)) ==32);

	// Trying to assign period 9 to quarterly tsidx throws an outside_frequence
	CPPUNIT_ASSERT_THROW( tsidx(1980,1,quarterly).distance_from(tsidx(1982,9,quarterly)) == 32, outside_frequency );

	// stress last day of leap year
	  CPPUNIT_ASSERT(tsidx(1980, 1, daily).distance_from(tsidx(1984,366,daily)) == 1826);

	// stress range
	for(int i=1;i<=365;i++)
	  CPPUNIT_ASSERT(tsidx(1980, 1,daily).distance_from(tsidx(1980,i,daily))==(i-1));

	for(int i=365;i>0;i--)
	  CPPUNIT_ASSERT(tsidx(1980,1,daily).distance_from(tsidx(1979,i,daily))==-(365-i+1));

	// stress negative distance
	for(int i=1;i<=100;i++) {
	  //cerr <<  tsidx(1980, i,daily).distance_from(tsidx(1984, 365-i,daily)) << endl; 
	  //cerr << -tsidx(1984, 365-i, daily).distance_from(tsidx(1980, i,daily)) << endl;
	  CPPUNIT_ASSERT(tsidx(1980, i,daily).distance_from(tsidx(1984, 365-i,daily)) == -tsidx(1984, 365-i, daily).distance_from(tsidx(1980, i,daily)));
	}
  }

  void testPreincrement()
  {
    CPPUNIT_ASSERT(++tsidx(1980,  365, daily) == tsidx(1981,1,daily));
  }
  void testPostincrement()
  {
    CPPUNIT_ASSERT(tsidx(1980,  365, daily)++ == tsidx(1981,1,daily));
  }

  void testCopy1()
  {
	{
	  tsidx t1(1980,  365, daily);
	  tsidx t2(t1);
	  CPPUNIT_ASSERT(t1==t2);
	}
	{
	  tsidx t1(1980,  365, unknown);
	  tsidx t2(t1);
	}
  }

};

LoggerPtr bits::tsidx::logger(Logger::getLogger("bits.tsidx"));

int main(int argc, char**argv) 
{
  PropertyConfigurator::configure("test_periods.log.conf");
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( TESTNAME::suite() );
  runner.run();
  return 0;
}


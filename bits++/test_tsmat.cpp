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
#include "tsmat.h"
#include "matrix.h"
#include "log4bits"

using namespace bits;

#define PRIVATE_EXPR_TEST(e, t)  CPPUNIT_ASSERT( ((e), (t)) )

// Generate unique random numbers from 0 to mod:

namespace bits{
  template<class T> T sqrt(T i) {return i*i;}
  //template<class T> void look(T& t) { cout << t << endl;  }
}

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

#define TESTNAME test_tsmat

class TESTNAME : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( TESTNAME );
  CPPUNIT_TEST( testCTORS ) ;
  CPPUNIT_TEST( testName ) ;
  CPPUNIT_TEST( testMeta ) ;
  CPPUNIT_TEST( testCTORSextended ) ;
  CPPUNIT_TEST( testFrequencyMismatch ) ;
  CPPUNIT_TEST( testOperation1 ) ;
  CPPUNIT_TEST( testExtract1 ) ;
  CPPUNIT_TEST( testCompatiblty ) ;
  CPPUNIT_TEST( testExtract2 ) ;
  CPPUNIT_TEST( testShift ) ;
  CPPUNIT_TEST( testAppend1 ) ;
  CPPUNIT_TEST( testAppend2 ) ;
  CPPUNIT_TEST( testAppend3 ) ;
  CPPUNIT_TEST( testPrepend1 ) ;
  CPPUNIT_TEST( testJoinWithHole ) ;
  CPPUNIT_TEST( testJoinWithOperators ) ;
  CPPUNIT_TEST( testJoinWithName ) ;
  CPPUNIT_TEST( testAccessWithName ) ;
  CPPUNIT_TEST( testAccessValue ) ;
  CPPUNIT_TEST_SUITE_END();

private:
  LOGGER4BITS;

   matrix v;         // My wonderful data matrix
   matrix w;
  tseries t;

public:
  void setUp()
  {

	LOG4CXX_INFO(logger,"setUp()");
	const int sz = 20;             // Supporting vectors
	column a(sz), b(sz), c(sz);

	generate_n(a.begin(),a.size(), linear<double>(2.0)); // Generate random vector
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
	generate_n(a.begin(),a.size(), linear<double>(2.0,0.0)); // Generate random vector
	w.push_back(a); 
	w.push_back(a); 
	w.push_back(a); 
	w.push_back(a); 

  }

  void tearDown() 
  {
	LOG4CXX_INFO(logger,"tearDown()");
	v.clear();
	w.clear();
	  
  }

  void testCTORS()
  {
	LOG4CXX_INFO(logger,"testCTORS()");
   tseries t1;                    // This' a void timeseries
   tseries t2(1980,1,daily);      // This' an empty timeseries
   
   // Set up some useful timeseries
   tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
   // This' an equal one 
   tseries t31(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
   CPPUNIT_ASSERT(v==w);
   CPPUNIT_ASSERT(t3==t31);
   CPPUNIT_ASSERT(t3.name(0)=="T0");
   CPPUNIT_ASSERT(t3.get_freq()==daily);
   
   // Lagged late
   tseries t32(1980,24,daily,v);   // (1980,24,daily)->(1980,44)
   // Leaded b4
   tseries t33(1979,300,daily,v);  // (1979,300,daily)->(1979,320)
   // Same data Monthly
   tseries t4 (1980,1,monthly,w);  //  (1980,1,monthly)->(1981,8)
  }

  void testName()
  {
	LOG4CXX_INFO(logger,"testName()");

	tseries t1(1980,1,daily,v);
	CPPUNIT_ASSERT(t1.name(0)=="TS0"); 
	CPPUNIT_ASSERT(t1.name(3)=="TS3");		
	CPPUNIT_ASSERT_THROW(t1.name(4)=="<EMPTY>",no_element);  // TODO: Should change
	t1.name(0,"FOO");
	CPPUNIT_ASSERT(t1.name(0)=="FOO");

  }

  void testMeta()
  {
	LOG4CXX_INFO(logger,"testMeta()");
	
	{
	  tseries t1(1980,1,daily,v);
	  CPPUNIT_ASSERT_THROW( t1.meta("ABSENT","M",0.0) , std::out_of_range);
	  CPPUNIT_ASSERT_THROW( t1.meta(99,"M",0.0) , std::out_of_range);

	  
	  t1.meta("TS0","M",10.0);
	  t1.meta("TS0","M",11.0);
	  t1.meta("TS3","N",string("foo"));
	  any x = t1.meta(string("TS3"),"N");
	  string s = any_cast<string>( t1.meta(string("TS3"),"N") );
	  double d = any_cast<double>( t1.meta(string("TS0"),"M") );
	  LOG4CXX_INFO(logger,"s="<<s<<" d="<<d);
	  CPPUNIT_ASSERT(s=="foo");	
	  CPPUNIT_ASSERT(d==11.0);	
	  CPPUNIT_ASSERT_THROW( t1.meta("TS1","M") , no_element);
	  CPPUNIT_ASSERT_THROW( t1.meta("TS3","M") , no_element);
	}
	{
	  tseries t1(1980,1,daily,v);
	  t1.meta(1,"M",10.0);
	  t1.meta(1,"M",11.0);
	  t1.meta(2,"N",string("foo"));
	  string s = any_cast<string>( t1.meta(2,"N") );
	  double d = any_cast<double>( t1.meta(1,"M") );
	  CPPUNIT_ASSERT(s=="foo");	
	  CPPUNIT_ASSERT(d==11.0);	
	  CPPUNIT_ASSERT_THROW( t1.meta(0,"M") , no_element);
	  CPPUNIT_ASSERT_THROW( t1.meta(1,"M") , no_element);
	}

  }

  void testCTORSextended()
  {
	LOG4CXX_INFO(logger,"testCTORSextended()");

	LOG4CXX_INFO(logger, "CTOR+name" );
	vector<string> n;
	n.push_back(string("FOO"));
	n.push_back("BAR");
	n.push_back("BAZ");
	n.push_back("BOZ");
	tseries t1(1980,1,daily,v,n);
	LOG4CXX_INFO(logger, "SName=" << t1.name(0) );
	CPPUNIT_ASSERT(t1.name(0)=="FOO");	

	LOG4CXX_INFO(logger, "CTOR+name+releases" );
	vector<double> r;
	r.push_back(711033.0);
	r.push_back(711034.0);
	r.push_back(711035.0);
	r.push_back(711036.0);
	tseries t2(1980,1,daily,v,n,r);
	cerr << t2 << endl;
  }

  void testFrequencyMismatch()
  {
	LOG4CXX_INFO(logger," testFrequencyMismatch");
   tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
   // Same data Monthly
   tseries t4 (1980,1,monthly,w);  //  (1980,1,monthly)->(1981,8)
   // Check for frequency mismatch
   CPPUNIT_ASSERT_THROW(t3 += t4 , frequency_mismatch);
  }

  void testOperation1()
  {
	LOG4CXX_INFO(logger," testOperation1");
   // Set up some useful timeseries
   tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
   // This' an equal one 
   tseries t31(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
   // Now adds t31 to t3
   t3+=t31;

   // CPPUNIT_ASSERT( some norm )  TODO
  }

  void testExtract1()
  {
	LOG4CXX_INFO(logger," testExtract1");
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)

	// define a new tseries with the same freq of t3
	// beginning at (10,1980)
	tseries tp(tsidx(1980,10), t3.get_freq()) ;
	
	//put into tp 3 elements from 1980,3
	t3.extract(tp,tsidx(1980,10,t3.get_freq()),tsidx_distance(0,3)) ;
	CPPUNIT_ASSERT(tp(tsidx(1980,12),0)==24);
  }


  void testCompatiblty()
  {
	LOG4CXX_INFO(logger," testCompatiblty");
   // Set up some useful timeseries
   tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
   // This' an equal one 
   tseries t31(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
   // Lagged late
   tseries t32(1980,24,daily,v);   // (1980,24,daily)->(1980,44)
   tseries tp(tsidx(1980,10), t3.get_freq()) ;
   t3.extract(tp,tsidx(1980,10,t3.get_freq()),tsidx_distance(0,3)) ;

   CPPUNIT_ASSERT( t3.compatible_with(t31) == true);
   CPPUNIT_ASSERT( t3.compatible_with(t32) == false);
   // CPPUNIT_ASSERT_THROW( tp << t3 , not_compatible);
  }

  void testExtract2()
  {
	LOG4CXX_INFO(logger," testExtract2");
	// Set up some useful timeseries
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	tseries tp1( tsidx(1980,2), t3.get_freq()) ;
	tp1 = t3.extract(
					 tsidx(1980,2,t3.get_freq()),
					 tsidx_distance(0,3)          );
  }


  void testShift()
  {
	LOG4CXX_INFO(logger," testShift");
   /*
	* Shift operation 
	*
	*/

   // Set up some useful timeseries
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)

   // This' an equal one 
	vector<string> n;
	n.push_back(string("FOO"));
	n.push_back("BAR");
	n.push_back("BAZ");
	n.push_back("BUZ");
	tseries t31(1980,1,daily,w,n);    //  (1980,1,daily)->(1980,20)
	t31.meta("BAZ","release",733100.0);
	t31.meta(1,"release",733101.0);
	t3 << t31;
   cerr << t3 << endl;
   double d = any_cast<double>(t3.meta("BAZ","release"));
   LOG4CXX_INFO(logger,"META.RELEASE="<<d);
   CPPUNIT_ASSERT(d==733100.0);
   d = any_cast<double>(t3.meta(6,"release"));
   LOG4CXX_INFO(logger,"META.RELEASE="<<d);
   CPPUNIT_ASSERT(d==733101.0);
  }


  void testAppend1()
  {
	LOG4CXX_INFO(logger," testAppend1");
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	// appending self to t3 (throw)
   CPPUNIT_ASSERT_THROW(t3.append( t3 ), overlapping);
  }


  void testAppend2()
  {
	LOG4CXX_INFO(logger," testAppend2");
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	tseries t31(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
   tseries t32(1980,24,daily,v);   // (1980,24,daily)->(1980,44)
   // appending t32 to t31" << endl;
   t31.append( t32 );
  }

  void testAppend3()
  {
	LOG4CXX_INFO(logger," testAppend3");
	column a(1);
	a[0]=(1.1);
	matrix m1;
	m1.push_back(a);
	tseries t1(2000,1,12,m1);
	cout << m1 << endl;
	cout << "M!S=" << m1.size()
		 << "M!E=" << t1.start
		 << "M!E=" << t1.end()
		 << " T1=" << t1
		 << endl;
	a[0]=(1.2);
	a.push_back(1.3);
	matrix m2;
	m2.push_back(a);
	tseries t2(2000,3,12,m2);
	cout << m2 << endl;
	cout << "M!S=" << m2.size()
		 << "M!E=" << t2.start
		 << "M!E=" << t2.end()
		 << " T2=" << t2
		 << endl;

	t1.join (t2);

	cout << "M!E=" << t1.start
		 << "M!E=" << t1.end()
		 << " T1=" << t1
		 << " T2=" << t2
		 << endl;
  }

  void testPrepend1()
  {
	LOG4CXX_INFO(logger," testPrepend1");
	tseries t3 (1980,1,daily,v);    // (1980,1,daily)->(1980,20)
	tseries t31(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
	tseries t32(1980,24,daily,v);   // (1980,24,daily)->(1980,44)
	tseries t33(1979,300,daily,v);  // (1979,300,daily)->(1979,320)
	// "prepending t33 to t32" << endl;
	t32.prepend( t33 );
  }


  void testJoinWithHole()
  {
	LOG4CXX_INFO(logger," testJoinWithHole");
   tseries t51(1980,1,daily,v);
   tseries t511(1980,1,daily,w);
   tseries t512(1980,1,daily,w);
   tseries t52(1979,360,daily,v);
   tseries t53(1980,30,daily,v);
   tseries t55(1980,51,daily,v);

   t511.join(t53); // XXXXXX______XXXXXX -->20_____30<---
   t511.join(t55); // XXXXXX,XXXXXX -->49,60<---
  }
  
  void testJoinWithOperators()
  {
	LOG4CXX_INFO(logger," testJoinWithOperators");
   tseries t51(1980,1,daily,v);
   tseries t511(1980,1,daily,w);
   tseries t512(1980,1,daily,w);
   tseries t52(1979,360,daily,v);
   tseries t53(1980,30,daily,v);
   tseries t55(1980,51,daily,v);

   t511.join(t53); // XXXXXX______XXXXXX -->20_____30<---
   t511.join(t55); // XXXXXX,XXXXXX -->49,60<---

   CPPUNIT_ASSERT_THROW(t511.join(t55), overlapping );
   t511 = t512;

   t511.join(t55,plus<matrix>());
  }

  void testJoinWithName()
  {
	LOG4CXX_INFO(logger," testJoinWithName");
   tseries t51(1980,1,daily,v);
   tseries t511(1980,1,daily,w);
   tseries t512(1980,1,daily,w);
   tseries t52(1979,360,daily,v);
   tseries t53(1980,30,daily,v);
   tseries t55(1980,51,daily,v);


   t511.name(0,"pippo");
   t511.name(1,"PLUTO");
   t511.join( t52, plus<matrix> () );
   t511.join( t53, minus<matrix>() );
  }

  void testAccessWithName()
  {
	LOG4CXX_INFO(logger," testAccessWithName");

   tseries t51(1980,1,daily,v);
   tseries t511(1980,1,daily,w);
   tseries t512(1980,1,daily,w);
   tseries t52(1979,360,daily,v);
   tseries t53(1980,30,daily,v);
   tseries t55(1980,51,daily,v);

   tseries t5111 =  t511["TS1"] ;
  }

  void testAccessValue()
  {
	LOG4CXX_INFO(logger," testAccessValue");

   tseries t51(1980,1,daily,v);
   tseries t511(1980,1,daily,w);
   tseries t512(1980,1,daily,w);
   tseries t52(1979,360,daily,v);
   tseries t53(1980,30,daily,v);
   tseries t55(1980,51,daily,v);
   tseries t5111 =  t511["TS1"] ;
   double d = t5111.value(t5111.start,0);
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

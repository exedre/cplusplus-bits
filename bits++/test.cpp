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

using namespace std;

class myTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( myTest );
  CPPUNIT_TEST( testEquality );
  CPPUNIT_TEST( testAddition );
  CPPUNIT_TEST_SUITE_END();

private:

public:
  void setUp()
  {
  }

  void tearDown() 
  {

  }

  void testEquality()
  {
    CPPUNIT_ASSERT( 1 == 1 );
  }

  void testAddition()
  {
    CPPUNIT_ASSERT( 1 );
  }
};
namespace bits{
  template<class T> T sqrt(T i) {return i*i;}
  //template<class T> void look(T& t) { cout << t << endl;  }
}

template<> LoggerPtr tseries::logger(Logger::getLogger("bits.tsmat"));

LOGGERDEF(matrix);
LOGGERDEF(tsidx);
//LOGGERDEF(tseries);
LoggerPtr logger(Logger::getLogger("nps"));
main() 
{
  matrix w,v;
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

  v==w;
  tseries t1(1980,1,daily,w);    //  (1980,1,daily)->(1980,20)
  tseries t2(1980,1,daily,v);    //  (1980,1,daily)->(1980,20)
  t1==t1;

  /*
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( myTest::suite() );
	runner.run();
  */
  return 0;

}

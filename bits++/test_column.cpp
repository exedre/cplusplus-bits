#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tsidx.h"
#include "matrix.h"
#include "log4bits"

using namespace std;
using std::cout;
using std::endl;

using namespace bits;
namespace bits {
  template<class T> T sqrt(T i) {return i*i;}
}

#define TESTNAME test_column

LoggerPtr logger(Logger::getLogger("test_column"));

class TESTNAME : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( TESTNAME );
  CPPUNIT_TEST( testCTORS );
  CPPUNIT_TEST( testOP1 );
  CPPUNIT_TEST( testShrink );
  CPPUNIT_TEST( testExtract );
  CPPUNIT_TEST_SUITE_END();

private:  
  LOGGER4BITS;

  column* A;
  column* B;
  column* C;
	
public:
  void setUp()
  {
	LOG_TRACE(logger,"setUp()");
	int sz;             // Supporting vectors
	sz = 3;

	A = new column(sz,0.0);
	B = new column(sz,0.0);
	C = new column(sz,0.0);

	column& a = *A;
	column& b = *B;
	column& c = *C;
	
	generate_n(a.begin(),a.size(), linear<double>(12.0,7.7)); // Generate random vector
	transform (a.begin(), a.end(), b.begin(), bits::sqrt<double>); 
	for_each(c.begin(), c.end(), uniform<double>(7.2));

  }

  void tearDown() 
  {
	LOG4CXX_INFO(logger," tearDown()");
	delete A;
	delete B;
	delete C;
  }

  void testCTORS()
  {
	LOG4CXX_INFO(logger," testCTORS()");
	column c;
	column d(1000);
	column e(1000,2.2);
	column f(e);
	CPPUNIT_ASSERT(isnan(d[999]));
	CPPUNIT_ASSERT(e[888]==2.2);
	CPPUNIT_ASSERT(f[188]==2.2);
	column g=*A;
	CPPUNIT_ASSERT(g[1]==(*A)[1]);
  }

  void testOP1()
  {
	LOG4CXX_INFO(logger," testOP1()");
	column a(10,0.0);
	column b(10,0.0);
	generate_n(a.begin(),a.size(), linear<double>(7.7)); // Generate random vector
	for_each(b.begin(), b.end(), uniform<double>(7.2));

	// Column-Column operators
	column c;
	c  = a;
	c += b;
	CPPUNIT_ASSERT(c[5]==a[5]+b[5]);
	c  = a;
	c -= b;
	CPPUNIT_ASSERT(c[5]==a[5]-b[5]);
	c  = a;
	c *= b;
	CPPUNIT_ASSERT(c[5]==a[5]*b[5]);
	c  = a;
	c /= b;
	CPPUNIT_ASSERT(c[5]==a[5]/b[5]);

	// Column-double operations
	c  = a;
	c += 2.2;
	CPPUNIT_ASSERT(c[5]==a[5]+2.2);
	c  = a;
	c -= 2.2;
	CPPUNIT_ASSERT(c[5]==a[5]-2.2);
	c  = a;
	c *= 2.2;
	CPPUNIT_ASSERT(c[5]==a[5]*2.2);
	c  = a;
	c /= 2.2;
	CPPUNIT_ASSERT(c[5]==a[5]/2.2);
	c  = 2.2;
	CPPUNIT_ASSERT(c[5]==2.2);
  }
  
  void testShrink()
  {
	LOG4CXX_INFO(logger," testShrink");
	column a(1000,0.0);
	generate_n(a.begin(),a.size(), linear<double>(7.7)); // Generate random vector
	column b = a.shrink(99,120);
	double d=b.at(0);
	CPPUNIT_ASSERT(equals(d,770));
  }

  void testExtract()
  {
	LOG4CXX_INFO(logger," testExtract");
	column a(1000,0.0);
	generate_n(a.begin(),a.size(), linear<double>(7.7)); // Generate random vector
	column b=a.shrink(99,173);
	b.extract(a,1,22);
	//cout << a << endl;
	CPPUNIT_ASSERT(equals(a[0],777.7));
  }
};

LOGGERDEF(matrix);
LOGGERDEF(tsidx);
LOGGERDEF(TESTNAME);

int main(int argc, char**argv) 
{
  PropertyConfigurator::configure("test.log.conf");
  LOG4CXX_INFO(logger,"TestCase " LOGGERXQUOTE(TESTNAME) );
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( TESTNAME::suite() );
  runner.run();
  return 0;
}

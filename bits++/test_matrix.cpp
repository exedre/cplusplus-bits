#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestSuite.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tsidx.h"
#include "tsmat.h"
#include "matrix.h"
#include "log4bits"



using namespace std;
using std::cout;
using std::endl;

using namespace bits;
namespace bits {
template<class T> T sqrt(T i) {return i*i;}
//template<class T> void look(T& t) { cout << t << endl;  }
}

#define TESTNAME test_matrix

LoggerPtr logger(Logger::getLogger("test_matrix"));

class TESTNAME : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( TESTNAME );
  CPPUNIT_TEST( testCTORS );
  CPPUNIT_TEST( testNobs );
  CPPUNIT_TEST( testShrink );
  CPPUNIT_TEST( testExtract );
  CPPUNIT_TEST( testColumnShift );
  CPPUNIT_TEST( testAppendingValues );
  CPPUNIT_TEST( testOperationsMatrixPlusScalar );
  CPPUNIT_TEST( testOperationsMatrixMultColumn );
  CPPUNIT_TEST( testOperationsMatrixMultMatrix );
  CPPUNIT_TEST( testEquality );
  CPPUNIT_TEST( testAddition );
  CPPUNIT_TEST_SUITE_END();

private:
  matrix v;         // My wonderful data matrix
  int sz;             // Supporting vectors

public:
  void setUp()
  {
	LOG4CXX_INFO(logger," setUp");
	sz = 20;
	column a(sz), b(sz), c(sz);
	generate_n(a.begin(),a.size(), linear<double>(7.7)); // Generate random vector
	v.push_back(a);   // Add a column
	a += a;
	v.push_back(a); 

	a -= 3.1;
	v.push_back(a); 

	a *= 4.2;
	v.push_back(a); 
	// a *= a;

	transform (a.begin(), a.end(), b.begin(), bits::sqrt<double>); 
	v.push_back(b); 
	CPPUNIT_ASSERT(a < b); 

	transform(a.begin(), a.end(), a.begin(), b.begin(), std::plus<double>()); 
	v.push_back(b); 

	generate_n(c.begin(), c.size(), uniform<double>(7.2));
	v.push_back(c); 
	// cerr << v << endl;

  }

  void tearDown() 
  {
	LOG4CXX_INFO(logger," tearDown");
	v.clear();
  }

  void testCTORS()
  // tests copy CTOR and assignment operator
  {
	LOG4CXX_INFO(logger," testCTORS");
	matrix w(v); // Copy ctor
	matrix z = w; // Assignment operator
	CPPUNIT_ASSERT(w[4][7]=v[4][7]);
	matrix x(10,10,7.1);
	CPPUNIT_ASSERT(x[9][9]=7.1);
	double d[9] =
	  { 0.0, 0.1, 0.3,
		3.0, 3.1, 3.2,
		6.7, 8.9, 9.8 };
	matrix y(3,3,d);
	CPPUNIT_ASSERT(x[2][2]=9.8);
  }

  void testNobs()
  // Tests nobs
  {
	int sz=1000;
	matrix x(sz,sz,7.1);
	CPPUNIT_ASSERT(x.nobs()==1000);
  }

  void testAppendingValues()
  // Tests append, prepend, append_missing, prepend_missing
  {
	LOG4CXX_INFO(logger," testAppendingValues");
	matrix x=v;
	x.append(v);
	x.append_missing  (18);
	x.prepend_missing (18);
	x.prepend(v);
	cout << x << endl;
	CPPUNIT_ASSERT(x.nobs()==96);
	CPPUNIT_ASSERT(isnan(x[4][95]));
	CPPUNIT_ASSERT(x.at(5).at(77)-2561.16<1e-3);
	CPPUNIT_ASSERT(x.at(6).at(0)-7.2<1e-3);

  }
 
  void testShrink()
  {
	LOG4CXX_INFO(logger," testShrink (NIY)");
  }

  void testExtract()
  {
	LOG4CXX_INFO(logger," testExtract");
	matrix m=v;
	v.extract(m,3,6);
	CPPUNIT_ASSERT(m.size()==7);
	CPPUNIT_ASSERT(m.nobs()==3);
	CPPUNIT_ASSERT(equals(m[5][2],750.12));
  }

  void testJoin()
  {
	LOG4CXX_INFO(logger," testJoin (NYI)");
	
  }

  void testMerge()
  {
	LOG4CXX_INFO(logger," testMerge");
	matrix m=v;
	v.extract(m,3,6);
	cerr << m << endl;	
  }

  void testColumnShift()
  {
	LOG4CXX_INFO(logger," testColumnShift");
	matrix w = v;

	CPPUNIT_ASSERT(w==v);

	column b(sz);
	generate_n(b.begin(),b.size(), std::rand); // Generate random vector
	w.push_back(b);

	// shifting in columns
	w << v ;

	// appending values (even missing)
	CPPUNIT_ASSERT_THROW( w.append(v) ,  size_mismatch );
  }


  void testOperationsMatrixPlusScalar()
  {
	LOG4CXX_INFO(logger," testOperationsMatrixPlusScalar");
	matrix l=v;
	l += 2.2;
  }

  void testOperationsMatrixMultColumn()
  {
	LOG4CXX_INFO(logger," testOperationsMatrixMultColumn");
	matrix l=v;
	column b(sz);
	generate_n(b.begin(),b.size(), std::rand); // Generate random vector
	l *= b;
  }

  void testOperationsMatrixMultMatrix()
  {
	LOG4CXX_INFO(logger," testOperationsMatrixMultMatrix");
	matrix l=v;
	l *= l;
  }

  void testExtractValue()
  {
	LOG4CXX_INFO(logger," testExtractValue");
	matrix m = v(0);
  }

  void testEquality()
  {
	LOG4CXX_INFO(logger," testEquality");
	matrix m = v;
    CPPUNIT_ASSERT( m == v );
  }

  void testAddition()
  {
	LOG4CXX_INFO(logger," testAddition");
    CPPUNIT_ASSERT( 1 );
  }

};

LoggerPtr bits::matrix::logger(Logger::getLogger("bits.matrix"));

int main(int argc, char**argv) 
{
  PropertyConfigurator::configure("test.log.conf");
  LOG4CXX_INFO(logger,"TestCase " LOGGERXQUOTE(TESTNAME) );
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( TESTNAME::suite() );
  runner.run();
  return 0;
}

/*

  This file is part of BITS++ -  C++ Banca d'Italia Time Series project

  Copyright (c) 2005-2012 Banca d'Italia - Area Ricerca Economica e
                          Relazioni Internazionali

  Author: Emmanuele Somma (emmanuele_DOT_somma_AT_bancaditalia_DOT_it)
          Area Ricerca Economica e Relazioni Internazionali 
          Banca d'Italia
		  
  Any parts of this program derived from this project,
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

#ifndef _TSMAT_H
#define _TSMAT_H

#include <cstdlib>
#include <utility>
#include <iostream>
#include <iomanip>
#include <exception>
#include <functional>
#include <stdexcept>
using namespace std;

#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/parse_tree.hpp>
#include <boost/lambda/algorithm.hpp>
#include <boost/operators.hpp>
#include <boost/any.hpp>
using namespace boost;
using namespace boost::lambda;
using namespace boost::spirit;

#include "tsidx.h"
#include "matrix.h"

typedef const char* iterator_t;
typedef pt_match_policy<iterator_t> match_policy_t;
typedef scanner_policies<iteration_policy, match_policy_t, action_policy> scanner_policy_t;
typedef scanner<iterator_t, scanner_policy_t> scanner_t;
typedef rule<scanner_t> rule_t;
typedef tree_match<iterator_t> parse_tree_match_t;
typedef parse_tree_match_t::const_tree_iterator iter_t;

using namespace bits;





namespace bits {

class not_compatible: public std::exception {}; ///< Exception: series are not compatible
class overlapping: public std::exception {}; ///< Exception:  series are overlapping
class syntax_error: public std::exception {}; ///< Exception:  syntax error
class no_element: public std::exception {}; ///< Exception:  container has no such element
class out_of_range: public std::exception {}; ///< Exception:  out of range


/** @brief map string compare function 
 */
struct lstr 
{
  bool operator()(const string& s1, const string& s2) const
  {
	return s1.compare(s2) < 0;
  }
};

/** @brief multivariate timeseries
 *  @class tsmat
 *

 A tsmat is a multivariate (multiple columns) timeseries
 where all are defined on the same time range and have 
 the same frequency.

 Each timeseries has a name (stored in 'series' property) and own
 metadata (stored in the 'metadata' map indexed by the timeseries
 name)

 All the data are packed in a Matrix, stored column by column.

 As helper is defined a type 'tseries' equal to tsmat<matrix, double>
 (a tsmat over 'double' values stored in a 'matrix' containers @see
 matrix.h)


 Time series creation:

 tseries t1; 
 creates a timeseries with default setup (not very useful)

 tseries t2(1980,1,daily);
 creates a daily timeseries beginning the first day of 1980 (01/01/1980)
 without any content, is a well defined but empty timeseries.

 being 'm' a matrix (@see matrix.h) you can define 

 tseries t3(1980,1,daily,m);
 creates a daily timeseries beginning the first day of 1980
 (01/01/1980) the content is defined in the matrix 'm'. The number of
 columns of the matrix is the number of series, whereas the number of
 rows is the number of observations

 The length of the timeseries is determined by the length of the data
 in the matrix.

 tseries t4(1980,1,monthly,m);
 the same data are used to set up a monthly time series. 

 Also a timeseries could be constructed passing it a tsidx (@see tsidx.h):
 tseries tp(tsidx(1980,10),t3.get_freq());

 If exists, fourth argument to the constructor function is the vector
 of columns (variables) names. If thay are not expressed names will be
 T1, T2, ...

 Accessors:
 Some accessors function and properties are defined

@code
 t.start
 t.freq
 t.get_freq()
@endcode

 Operations:

 Two timeseries can be added or subtracted if they have the same
 frequency and number of columns.

 Trying operations between t3 and t4 throws frequency_mismatch
 exception (i.e. t3 += t4 ).

 Extractions:

 */
template<class Matrix, class T>
  class tsmat :
  boost::additive< tsmat<Matrix,T> >,    // timeseries could be added and subtracted
	boost::shiftable< tsmat<Matrix,T> >  // shifting timeseries?!?
{
  LOGGER4BITS; 

  // Data support
  Matrix data;					                 /**< these're data (each column is a single series) */

  // Timeseries basic settings
  tsidx _start;					/**< starting index of the whole group */
  frequency _freq;				/**< frequency for all the series */

  // Metadata support
  typedef vector<string>              names_t;

  names_t series;         		/**< the names of the series */

  typedef map<string,boost::any,lstr> metadata_val_t;
  typedef vector<metadata_val_t>      metadata_t ;

  mutable metadata_t metadata;          /**< per series metadata */

 public:
  const tsidx& start;					/**< starting index (read only) */
  const frequency& freq;				/**< series frequency (read only) */

  tsmat() ;                             ///< void constructor
  tsmat(const tsmat& t);                ///< copy constructor
  tsmat& operator =(const tsmat& t);    ///< assignment operator
  ~tsmat();                              ///< Default DTOR

  T value(const tsidx where, unsigned int which) const  ;

  // General ctors
  tsmat(int y, int p, frequency f);
  tsmat(int y, int p, frequency f, Matrix& Data);
  tsmat(int y, int p, frequency f, Matrix& Data, vector<string> names);
  tsmat(int y, int p, frequency f, Matrix& Data, vector<string> names, vector<double>& releases);
  tsmat(tsidx Start) ;
  tsmat(tsidx Start, frequency Freq) ;
  tsmat(tsidx Start, frequency Freq, Matrix& Data) ;
  tsmat(tsidx Start, frequency Freq, Matrix& Data, vector<string> names) ;
  tsmat(tsidx Start, frequency Freq, Matrix& Data, vector<string> names, vector<double>& releases) ;


  // Metadata manage
  boost::any meta(string name, string key, boost::any value);
  boost::any meta(unsigned int pos, string key, boost::any value);
  boost::any meta(string name, string key) const ;
  boost::any meta(unsigned int pos, string key) const ;

  string name(unsigned int n, string name); ///< Define name for a series
  string name(unsigned int n) const ;       ///< Gets the name for a series

  // Accessor methods
  tsmat operator()(const tsidx start, const tsidx end, const vector<unsigned int> which) const;           ///< multiple extract
  tsmat operator()(const tsidx start, const tsidx end, const unsigned int which) const;                   ///< single extract
  tsmat operator()(const tsidx start, const tsidx_distance len, const vector<unsigned int> which) const;  ///< multiple extract
  tsmat operator()(const tsidx start, const tsidx_distance len, const unsigned int which) const;          ///< single extract
  tsmat operator()(const tsidx start, const vector<unsigned int> which) const;
  tsmat operator()(const tsidx start, const unsigned int which) const;
  tsmat operator()(const vector<unsigned int> which) const;
  tsmat operator()(const unsigned int which) const;

  // Shift operators
 
  /** @brief Horizontal Concatenation of Tseries (from right to left)
   *  @param   t tseries to concat
   *  @returns this
   * 
   *  Series must have be compatibile (same start date and frequency)
   *  and the same length
   *
   * @todo Make it more general (support of any size of operands)
   *
   *
   */
  tsmat& operator <<(const tsmat& t);   ///< shift in
  
  
  /** @brief Horizontal Concatenation of Tseries (from right to left)
   *  @param   t tseries to concat
   *  @returns this
   * 
   *
   * @todo see << , YTBD
   */
  tsmat& operator >>(tsmat& t);   ///< shift out

  tsmat& operator =(const double& t);    ///< assignment operator
  
  /** @brief insert series into tseries if they are not yet in
   *  @param   t tseries to concat
   *  @returns this
   *  
   * Use the Metadata support
   * 
   * @todo YTBD
   */
  tsmat& operator ||(tsmat& t);   ///< Union
  
  /** @brief leave only common series
   *  @param   t tseries to leave
   *  @returns this
   *  
   * Use the Metadata support
   * 
   * @todo YTBD
   */
  tsmat& operator &&(tsmat& t);   ///< Intersection
  
  tsmat& operator  +(const tsmat& t);   ///< Vectorial Sum
  tsmat& operator  +(T t);     ///< Scalar Sum
  
  pair<size_t,size_t> size(void) const; ///< returns a pair (num series x n observation)
  
  tsidx  end(void) const;  ///< returns end index
  
  frequency get_freq(void) ///< returns the timeseries frequency
  { return freq; } 

  // cast operator
  operator T(void) const ; 

  // timeseries are totally ordered
  bool    operator   <(const tsmat&); 
  bool    operator  ==(const tsmat&); 

  // timeseries are additive
  tsmat&  operator  +=(const tsmat&); 
  tsmat&  operator  -=(const tsmat&); 
 
  // multiplicative 
  tsmat&  operator  *=(const tsmat&); 
  tsmat&  operator  /=(const tsmat&); 

  // some advanced operators not yet defined
  /*  tsmat&  operator  %=(const tsmat&); */
  /*  tsmat&  operator  |=(const tsmat&); */
  /*  tsmat&  operator  &=(const tsmat&); */
  /*  tsmat&  operator  ^=(const tsmat&); */
  /*  tsmat&  operator <<=(const tsmat&); */
  /*  tsmat&  operator >>=(const tsmat&); */
  /*  tsmat&  operator  ++(const tsmat&); */
  /*  tsmat&  operator  --(const tsmat&); */
  
  // ostream& operator <<(ostream& o);

  // Timeseries specific operations

  tsmat& lag(const int n=1) { _start+=n; return *this;}
  tsmat& lead(const int n=1) { return lag(-n); }
  
  bool compatible_with(const tsmat& t) ;
  bool   starts_before(const tsmat& t) ; // means that this->start < t.start


  tsmat& fill  (const T& v); // merge values if nan
 
  /** @brief merge timeseries
   *
   * If value of this timeseries if nan then substitute 
   * with incoming timeseries
   */
  tsmat& merge  (const tsmat& t); // merge values if nan

  /** @brief joins two non-overlapping timeseries
   *  
   */
  tsmat& join   (const tsmat& t);

  /** @brief joins timeseries even if overlapping
   *
   * if overlapping then use Functional to merge values
   */
  template<class Functional> 
	tsmat& join   (const tsmat& t, const Functional& f);

  /** @brief append values to timeseries
   *
   * timeseries must have the same # of timeseries
   */
 tsmat& append (const tsmat& t);

  /** @brief prepend values to timeseries
   *
   * timeseries must have the same # of timeseries
   */
 tsmat& prepend(const tsmat& t);

 /** @brief subref
  *
  */
 tsmat&  extract(tsmat& t, const tsidx& start, const tsidx_distance& dist) const ;
 tsmat&  shrink()  ;
 tsmat    extract(const tsidx& start, const tsidx_distance& dist) const;
 tsmat operator[](vector<string> select) const;
 tsmat operator[](string select) const;

 bool has_name(const string name) const;


 void data_copy(double* D) const;

 template<class A, class B>
   friend ostream& operator <<(ostream& o, const tsmat<A,B>& t);

 template<class A, class B>
   friend ostream& operator >>(ostream& o, const tsmat<A,B>& t);

};

/* @brief helper definition 
 * 
 */
typedef tsmat<matrix, double> tseries;

#include "tsmat.cch"

};

#endif // _TSMAT_H

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

#ifndef _MATRIX_H
#define _MATRIX_H

#include <boost/operators.hpp>
using namespace boost;

#include <cmath>

#include <vector>
#include <algorithm>
using namespace std;

#include "log4bits"

namespace bits {

  inline
  bool equals(double d1, double d2, double precision=1e-6)
  {
	double eps1 = fabs(d1), eps2 = fabs(d2), eps;
	eps = (eps1 > eps2) ? eps1 : eps2;
	if (eps == 0.0)
	  return true; //both numbers are 0.0
	//eps hold the minimum distance between the values
	//that will be considered as the numbers are equal
	//considering the magnitude of the numbers
	eps *= precision;
	return (fabs(d1 - d2) < eps);
  }

  /** @brief if X value is nan substitute it with Y 
   */
  template<class _T>
  struct subst_nan : binary_function<_T, _T, _T>
  { _T operator()(const _T& _X, const _T& _Y) const;  };

  class size_mismatch: public std::exception {}; ///< Exception: 

  /** @brief generate uniform double distribution 
	 
   */
  template<class _T>
	struct uniform : unary_function<_T,_T> { 
  uniform(_T d) : D(d) {} 
	_T D; 
	_T operator()(_T& d) { d=D; return D; }
	_T operator()() { return D; }
  };


  /** @brief generate linear double distribution
   */
  template<class _T>
  struct linear : unary_function<_T,_T> { 
  linear(_T d) : D(d), acc(0.0) {} 
  linear(_T i, _T d) : D(d), acc(i) {} 
	_T D; 
	_T acc;
	_T operator()() { acc+=D; return acc; }
  };

  /** @brief column class
   */
  class column : public vector<double>, 
	arithmetic<column, 
	arithmetic<column,double
	> >
  {
	LOGGER4BITS;

  public:
	column() {};   
	column(const int& sz);	
	column(const int& sz, const double& d);
	column(const column& c);

	// extract values
	column& extract(column& c, const int b, const int e) const ;
	column shrink(const int b, const int e) const ;

	// COLUMN <op> COLUMN
	column& operator =(const column&);

	column& operator+=(const column&);
	column& operator-=(const column&);
	column& operator*=(const column&);
	column& operator/=(const column&);

	   bool operator==(const column&) const;

	// COLUMN <op> DOUBLE
	column& operator=(const double&);

	column& operator+=(const double&);
	column& operator-=(const double&);
	column& operator*=(const double&);
	column& operator/=(const double&);

  };

  ostream& operator << (ostream& o, const column& m) ;

  template<>
	struct uniform<column> : unary_function<double,column> { 
	double D; 
	unsigned int size;
  uniform(unsigned int size, double d) : D(d), size(size) {}
	column operator()() { 
	  column c(size); 
	  generate_n(c.begin(),c.size(),uniform<double>(D));
	  return c;
	}
	column operator()(column& c) { 
	  generate_n(c.begin(),c.size(),uniform<double>(D));
	  return c;
	}
  };

  /** @brief substitute nans in a column
   */
  template<>
  struct subst_nan<column> : 
	binary_function<column, column, column>
  {
	const int b; const int e;
	subst_nan<column>() : b(0),e(0) {}
	subst_nan<column>(const int _b, const int _e) : b(_b),e(_e) {}
	column operator()(const column& _X, const column& _Y) const ;
  };

  /** @brief matrix class
   */
  class matrix 
	: public vector<column>,
	arithmetic<matrix,
	arithmetic<matrix,column,
	arithmetic<matrix,double
	> >	>
  {
	LOGGER4BITS;

  public:

	// VOID CTOR
	matrix(void) ;  ///< Base constructor
	matrix(const matrix&); ///< Copy Constructor
	matrix(unsigned int columns, unsigned int rows,double D=NAN);
	matrix(unsigned int columns, unsigned int rows,double *D);	

	matrix& append_missing (const long nx); ///< Append NAN values
	matrix& prepend_missing(const long nx); ///< Prepend NAN values
	matrix& append(const matrix& m) ; ///< Append (vertcat) a matrix
	matrix& prepend(const matrix& m) ; ///< Prepend (vertcat) a matrix

	long nobs() const ;  ///< Returns number of ROWS of a matrix 

	matrix& shrink() ; ///< Shrink the matrix (NYI)
	matrix& reduce(const int b, const int e) ; ///< Resize the rows of the matrix
	matrix& extract(matrix& m, const int b, const int e) const ; ///< Extract rows from the matrix m = extraction(this)
	matrix& join   (matrix& m, const int b, const int e) const ; ///< Join two matrices (NYI)
	matrix& merge  (const matrix& m, const int b, const int e) ; ///< Merge two matrices 
	matrix& operator<<(const matrix& m) ;
	matrix& operator>>(matrix& m) ;

	matrix& fill   (const double&) ;
	void copy(double* D) const;

	// MATRIX <op> MATRIX
	matrix& operator=(const matrix&); ///< Assignment operator

	matrix& operator+=(const matrix&);
	matrix& operator-=(const matrix&);
	matrix& operator*=(const matrix&);
	matrix& operator/=(const matrix&);

	// MATRIX <op> COLUMN
	matrix& operator=(const column&);

	matrix& operator+=(const column&);
	matrix& operator-=(const column&);
	matrix& operator*=(const column&);
	matrix& operator/=(const column&);

	// MATRIX <op> DOUBLE
	matrix& operator=(const double&);

	matrix& operator+=(const double&);
	matrix& operator-=(const double&);
	matrix& operator*=(const double&);
	matrix& operator/=(const double&);


	matrix operator()(const unsigned int which) const;
	matrix operator()(const vector<unsigned int> which);


	/** @brief Equivalence operator
	 *  Two matrices are equal iff have same size, same type and each element is equal
	 */
	bool operator==(const matrix& m);

  };

  ostream& operator << (ostream& o, const matrix& m) ;

}

#include "matrix.cch"

#endif // _MATRIX_H

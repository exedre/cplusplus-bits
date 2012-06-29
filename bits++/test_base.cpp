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

#define BOOST_INCLUDE_MAIN

#include <boost/config.hpp>                      // for BOOST_MSVC
#include <boost/cstdlib.hpp>                     // for boost::exit_success
#include <boost/operators.hpp>                   // for the tested items
#include <boost/random/linear_congruential.hpp>  // for boost::minstd_rand
//#include <boost/test/test_tools.hpp>             // for main
#include <boost/test/minimal.hpp> 


#include <iostream>  // for std::cout (std::endl indirectly)

#define PRIVATE_EXPR_TEST(e, t)  BOOST_CHECK( ((e), (t)) )

int test_main(int, char*[])
{ 
   using std::cout;
   using std::endl;

//    cout << "BEGIN TEST" << endl;

//    // Regression test.
//    period p(monthly);

//    p = 1980,1;

   std::cout <<  "SUCCESS" << std::endl;
   return boost::exit_success;
}


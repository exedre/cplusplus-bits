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

#ifndef _TSPARSE_H
#define _TSPARSE_H

#include <string>
#include <vector>
#include <stack>
#include <map>
using namespace std;


namespace bits 
{
  namespace tscalc
  {

	LoggerPtr logger(Logger::getLogger("tscalc"));

	string s_name;
	stack<tseries> stk;
	tseries* in;
	tseries* tnew;
	void clear()     ;
	tseries answer() ;
	tseries toppop() ;

	void PUSH_NUM(const char* b, const char* e) ;
	void PUSH_VAR(const char* b, const char* e) ;
	void    ADD(const char* , const char* ) ;
	void    SUB(const char* , const char* ) ;
	void    MUL(const char* , const char* ) ;
	void    DIV(const char* , const char* ) ;
	void   LAG1(const char* , const char* ) ;
	void   LAG2(const char* , const char* ) ;
	void    INV(const char* , const char* ) ;
	void  LEAD1(const char* , const char* ) ;
	void  LEAD2(const char* , const char* ) ;
	void   NAME(const char* , const char* ) ;
	void  ENTRY(const char* , const char* ) ;
  }
  tseries tsparse(tseries t, char * buffer) ;

#include "tsparse.cch"

}


#endif // _TSPARSE_H

/**
 * @file   tsidx.h
 * @author Emmanuele Somma
 * @date   Tue May 22 23:20:28 2007
 * 
 * @brief  time series indices and distances
 
 tsidx is a 'time-series index' a two components element that
 represents the large scale and small scale units of a time series
 (f.i. year and day) 

 */
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


#ifndef _TSIDX_H
#define _TSIDX_H

#include <cstdlib>
#include <cmath>
#include <utility>
#include <iostream>
#include <iomanip>
#include <limits>
#include <exception>
using namespace std;

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/operators.hpp>
using namespace boost;
using namespace boost::gregorian;

#include "log4bits"

namespace bits {

/** @brief tsidx frequency
 *  
 *  
 */
class frequency {
  int freq;

 public:
  frequency() :  freq(0) {}
  frequency(int i) :  freq(i) {}
  frequency(string m) : freq(0) {

	if (m=="undefined") { freq = -1; } else 
	  if (m=="bidaily") { freq = 733; } else 
		if (m=="daily") { freq = 365; } else 
		  if (m== "weekly") { freq = 52; } else 
			if (m== "monthly") { freq = 12; } else 
			  if (m== "quarterly") { freq = 4; } else 
				if (m== "annual") { freq = 1; } else 
				  if (m== "unknown") { freq = 0; } ;
  }
  operator int()    const { return freq; }  
  // operator double() const { return (double)freq; }  
  frequency& operator=(const frequency f) { freq=f; return *this; }
  bool operator==(const frequency f) const {return f.freq==freq; }
  bool operator!=(const frequency f) const {return f.freq!=freq; }
  bool good() const { return freq != 0; }
  friend ostream& operator<<(ostream& o, const frequency& f)  ;
  operator string() const {
	string o;
	switch (freq) {
	case   ( 733 ): o = '2' ; break;
	case   ( 365 ): o = 'D' ; break;
	case   ( 52  ): o = 'W' ; break;
	case   ( 12  ): o = 'M' ; break;
	case   ( 4   ): o = 'Q' ; break;
	case   ( 1   ): o = 'A' ; break;
	case   ( 0   ): o = '?' ; break;
	case   ( -1  ): o = '!' ; break;
	}
	return o;
  }

};

inline
ostream& operator<<(ostream& o, const frequency& f)  {
	switch (f.freq) {
	case   ( -1 ): o << "undefined" ; break;
	case   ( 733 ): o << "bidaily" ; break;
	case   ( 365 ): o << "daily" ; break;
	case   ( 52  ): o << "weekly" ; break;
	case   ( 12  ): o << "monthly" ; break;
	case   ( 4   ): o << "quarterly" ; break;
	case   ( 1   ): o << "annual" ; break;
	case   ( 0   ): o << "unknown" ; break;
	}
	return o;
  }


// Some constant well-known frequencies
const frequency bidaily  ( 733 );
const frequency daily    ( 365 );
const frequency weekly   (  52 );
const frequency monthly  (  12 );
const frequency quarterly(   4 ) ;
const frequency annual   (   1 );
const frequency unknown  (   0 );
const frequency undefined(  -1 );

/** @brief returns frequency 
 *  
 */
inline 
int ifreq(frequency f) 
{
  return f;
}

/* @brief returns frequency (take in account leap years for daily freqs)
 *
 */
inline 
int ifreq(frequency f, int y) 
{
  if (f==undefined)
	return 1000;      //HACK
  if (f==daily & gregorian_calendar::is_leap_year(y))
	return 366;
  else
	return f;
}

/** @brief returrns the number of periods between years
 *  @param f    frequency
 *  @param ybeg beginning year
 *  @param yend ending year
 *  @return number of periods
 */
inline 
int iperiods(frequency f, int ybeg, int yend)
{
  if (ybeg == yend) return 0;
    
  if (yend < ybeg)
	return -iperiods(f,yend,ybeg);

  if ( f == daily )
	{
	  int aa=0;
	  for (int j=ybeg; j<=yend; j++)
		if (gregorian_calendar::is_leap_year(j)) aa++;
	  return 365*(yend-ybeg)+aa;
	}
  else
	return f*(yend-ybeg);
}


class outside_frequency : public std::exception {}; ///< Exception: period is outside frequency limit
class frequency_mismatch: public std::exception {}; ///< Exception: there is a frequency mismatch
class unknown_frequency : public std::exception {}; ///< Exception: the frequency is unkonown
class limits_mismatch   : public std::exception {}; ///< Exception: the operand is outside limits

/**
 * @class   tsidx_distance
 * @brief   Distance made by two integer components  used in tsindex 

 The two components represents the large scale and small scale units
 but they don't relate each others.

 Some operations on tsidxs return or need a tsidx_distance

 tsidx_distance are totally ordered and additive on tsidx_distance field and
 on int field (which increments only the small scale units). 

 tsidx_distance can be normalized upon an external limit (maximum
 number of units in small scale element to increment by one the large
 scale element). In its use on tsidx the tsidx_distance limit is the
 tsidx frequency, the parameter of normalize() function is so called
 for that reason.

 tsidx_distance hasn't an internal concept of limit on its own
 dimensions (in contrast to a valid tsidx itself, which contains the
 frequency itself). Components of tsidx_distance are independent and
 only external operations (as normalize() ) can partition them into
 correct values.

 *
 */


class tsidx_distance :
  public pair<int,int>,  
	boost::totally_ordered<tsidx_distance,
	boost::additive<tsidx_distance,
	boost::additive<tsidx_distance,int
	> > >
{
  LOGGER4BITS;

 public:
  
  // CTORS
 tsidx_distance() : pair<int,int>(0,0)    {}              ///< void ctor
 tsidx_distance(int y, int p) : pair<int,int>(y,p) {}     ///< base ctor
 tsidx_distance(const tsidx_distance& p)                  ///< copy ctor
   : pair<int,int>(p.first,p.second)  {}
  tsidx_distance&  operator   =(const tsidx_distance p)   ///< assign operator
	{ first=p.first;  second=p.second; return *this; } 

  
  tsidx_distance&  operator   =(const int p)                   { first=p;  return *this; } 
  void             operator   ,(const int p)                   { second=p; }

  bool             operator   <(const tsidx_distance&) const ;
  bool             operator  ==(const tsidx_distance&) const ;
  tsidx_distance&  operator  +=(const tsidx_distance&) ;
  tsidx_distance&  operator  -=(const tsidx_distance&) ;
  tsidx_distance&  operator  +=(const int&) ;
  tsidx_distance&  operator  -=(const int&) ;

  tsidx_distance   operator -  (void)                  const   
	{ return tsidx_distance(-first,-second); }

  tsidx_distance&  normalize   (frequency freq);
  int              units       (frequency freq) const ;

};


ostream& operator <<(ostream& o, tsidx_distance p); 

class tsidx : 
public pair<int,int>, 
  boost::additive<tsidx,tsidx_distance,
  boost::additive<tsidx,int,
  boost::subtractable<tsidx_distance,tsidx,
  boost::totally_ordered<tsidx> 
  > > >
{
  LOGGER4BITS;

  frequency _freq;
 public:
  const frequency& freq;


  // CTORS
 tsidx() : pair<int,int>(0,0), _freq(unknown), freq(_freq) {}

 tsidx(int y, int p, frequency f=unknown) : pair<int,int>(y,p), _freq(f), freq(_freq)
  { 
	if (freq.good() and not good()) 
	  {
		throw outside_frequency(); 
	  }
  }
  
 tsidx(double v, frequency f=unknown) : pair<int,int>(v,v*1000), _freq(f), freq(_freq)
  {}

 tsidx(frequency f) : _freq(f), freq(_freq) {}
 tsidx(frequency f, int p) : pair<int,int>(0,p), _freq(f), freq(_freq)
  { normalize(); }
 tsidx(const tsidx& p) : pair<int,int>(p.first,p.second), _freq(p.freq), freq(_freq)
	//	{ if (freq and not good()) throw outside_frequency(); }
	{}

 tsidx(const tsidx& p,frequency f) : pair<int,int>(p.first,p.second), _freq(f), freq(_freq)
	//	{ if (freq and not good()) throw outside_frequency(); }
	{}

  bool good(void) const
  { return ( freq != unknown
			 and second >  0 
			 and first  >  0
			 and second <= ifreq(freq,first) ); 
  }


 void      setFreq(frequency f)                      { _freq=f; }

 tsidx& operator   =(const tsidx& p)              { first=p.first; second=p.second; _freq=p.freq; return *this;}
 tsidx& operator   =(const int p)                 { first=p;  return *this; }
 tsidx& operator   =(const double d)              
   { 
	 double fractpart, intpart;
	 fractpart = modf (d , &intpart);
	 first=(int)intpart;  
	 second=(int)(fractpart*1000.0+0.5); //STUPID STUPID STUPID HACK
	 return *this; 
   }
 void   operator   ,(const int p)                 { if (p==0) throw limits_mismatch(); second=p; }
 bool   operator   <(const tsidx&);
 bool   operator  ==(const tsidx&);
 tsidx& operator  +=(const tsidx_distance&);
 tsidx& operator  +=(const int&);
 tsidx& operator  -=(const tsidx_distance&);
 tsidx& operator  -=(const int&);
 tsidx  operator   +(const tsidx_distance&);
 tsidx  operator   +(const int);
 tsidx& operator  ++(const int);
 tsidx& operator  --(const int);
 tsidx& operator  ++(void) ; 
 tsidx  operator   -(void)                  const { return tsidx(-first,-second,freq); }
 tsidx_distance operator  -(const tsidx&) const;
 tsidx& normalize(void); 

 long distance_from(const tsidx&) const;
 tsidx lag(int);
 operator double() const ;

 // BEFORE_LEAP checks if the timeseries index is before the leap day
 //     B = BEFORE_LEAP(FREQ,Y,P) returns 1 if year is a leap year and 
 //      tsidx is equal or before Feb. 29, 0 if year is not a leap year, 
 //     -1 otherwise 
 int before_leap();
 date to_date();
 string str(void) { stringstream s; s << first << '-' << second; return s.str(); }
};

/// ------------------------------ INLINES

inline 
int tsidx_distance::units(frequency freq) const
{ 
  int f = first;
  int s = second;
  return f*freq+s;
}

inline 
int tsidx::before_leap()
{
  int bfl;
  if (not gregorian_calendar::is_leap_year(first))
	bfl=0;
  else if (date(first,Feb,29) < to_date())
	bfl=1;
  else
	bfl=-1;
  //  cerr << "bl " << gregorian_calendar::is_leap_year(first) << " "  << to_date() << " " << bfl << endl;
  return bfl;
}

inline date tsidx::to_date()
{
  int month_days[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
  
  int n = (12/(int)freq);
  date dat(first,1,1);
  int days=0;
  
  if (n==0) 
	{
	  int ngp = 366/ifreq(freq,first);
	  days=days+second*ngp-1;
	}
  else
	for ( int j=1; j<=(second-1)*n;j++)
	  {
		days+=month_days[j-1];
		if (gregorian_calendar::is_leap_year(first) & j==2)
		  days++;
	  }
  
  dat += date_duration(days);
  return dat;
}

inline tsidx::operator double() const
{ 
  double f = first;
  double s = second;
  double x = (int)freq;
  //  cerr << "double() : " << f << " " << s << " " << x << endl;
  return f+(.1*s)/x; 
}

inline long tsidx::distance_from(const tsidx& p) const
{
  //  double d = (static_cast<double>(p)-static_cast<double>(*this));
  long y=0;
  if (p < *this) return -(p.distance_from(*this));
  LOG_TRACE(logger, "Freq=" << freq ) ;

  if (freq ==  daily)
	  {
		date date1(first,Jan,1);
		date1 += date_duration(second-1) ;
		date date2(p.first,Jan,1);
		date2 += date_duration(p.second-1) ;
		y= -date_period(date2,date1).length().days();
		LOG_TRACE(logger,  "DAILY " << freq << " " << first << setw(3) << setfill('0') << second << " "  << setw(4) << p.first << p.second << " == " << y );		
	  }
  else
	  {
		LOG_TRACE(logger, freq << " " << first << second << " "  << p.first << p.second );
		y= freq*(p.first-first);
		y+= (p.second-second);
	  }
	return y;
}

inline tsidx tsidx::lag(int p)
{
  tsidx t(first,second+p);
  t.setFreq(freq);
  t.normalize();
  return t;
}

inline ostream& operator <<(ostream& o, tsidx p)
{ return o << "(" << p.first << "," << setw(4) << setfill(' ') << p.second << ")" ; } // "/" << p.freq << ")" ; }

inline tsidx& tsidx::normalize(void)
{
  // cerr << "NORM " << first << " " << second << endl;
  if (freq==unknown) 
	return *this;

  if (first == 0 && second ==0) 
	{
	  first --;
	  second = freq;
	  return *this;
	}

  if ( second > freq )
	{
	  first ++;
	  second -= freq;
	  normalize();
	  return *this;
	}

  if (second < -freq )
	{
	  first --;
	  second += freq ;	
	  normalize();
	  return *this;
	}

  if ( second < 0) 
	{
	  first --;
	  second = second + (int)freq;
	  return *this;
	}
  if ( second > 0)
	{	  
	  return *this;
	}
    if ( second == 0) 
	{
	  first --;
	  second = freq;
	  return *this;
	}
  return *this;
}

inline bool tsidx::operator   <(const tsidx& p)
{
  // frequenza ?!?
  LOG_TRACE(logger, "is " << p << " < " << *this << "?" );
  if ( freq != unknown )
	if ( freq != p.freq )
	  throw frequency_mismatch();
  normalize();
  if (p.first  > first  ) return true;
  if (p.first  < first  ) return false;
  if (p.second > second ) return true;
  if (p.second < second ) return false;
  return false; // if equals
}

inline bool tsidx::operator  ==(const tsidx& p)
{
  normalize();
  LOG_TRACE(logger, "operator == " << p << *this );
  if (p.first == first && p.second == second ) return true;
  return false;
}

 inline tsidx&  tsidx::operator  +=(const tsidx_distance& p)
{
  if ( freq == unknown ) throw unknown_frequency();
  first  +=  p.first;
  second += p.second;
  normalize();
  return *this;
}
inline tsidx&  tsidx::operator  +=(const int& p)
{
  if ( freq == unknown ) throw unknown_frequency();
  second  +=  p;
  normalize();
  return *this;
}

inline tsidx_distance  tsidx::operator  -(const tsidx& p) const
{
  tsidx_distance d(first,second);
  tsidx_distance e(p.first,p.second);
  d -= e;
  d.normalize(freq);
  return d;  
}

 inline tsidx  tsidx::operator  +(const tsidx_distance& p)
{
  //  if ( freq == unknown ) throw;
  tsidx t(first+p.first+(second+p.second)/freq,(second+p.second)%int(freq)); //ACHTUNG: int(freq) 
  t.setFreq(freq);
  t.normalize();
  LOG_TRACE(logger, "plus " << p << t );
  return t;
}

 inline tsidx  tsidx::operator  +(int p)
{
  tsidx t(first+(second+p)/freq,(second+p)%int(freq)); // ACHTUNG:
  t.setFreq(freq);
  t.normalize();
  return t;
}

inline tsidx&  tsidx::operator  -=(const tsidx_distance& p)
{
  LOG_TRACE(logger, "sub " << p );
  *this += -p;
  return *this;
}

inline tsidx&  tsidx::operator  -=(const int& p)
{
  LOG_TRACE(logger, "sub " << p );
  *this += -p;
  return *this;
}

inline tsidx&  tsidx::operator  ++(void) { second++; normalize(); return *this; }
inline tsidx&  tsidx::operator  ++(const int i=1) { second++; normalize(); return *this; }
inline tsidx&  tsidx::operator  --(const int i=1) { second--; normalize(); return *this; }

inline ostream& operator <<(ostream& o, tsidx_distance p)
{ return o << "{" << p.first << "," << setw(4) << p.second << "}" ; }

inline bool tsidx_distance::operator   <(const tsidx_distance& p) const
{
  if (p.first  > first  ) return true;
  if (p.first  < first  ) return false;
  if (p.second > second ) return true;
  if (p.second < second ) return false;
  return false; // if equals
}

inline bool tsidx_distance::operator  ==(const tsidx_distance& p) const
{
  LOG_TRACE(logger,"operator == " << p << *this);
  if (p.first == first && p.second == second ) return true;
  return false;
}

inline tsidx_distance&  tsidx_distance::operator  +=(const tsidx_distance& p)
{
  first  +=  p.first;
  second += p.second;  
  return *this;
}

inline tsidx_distance&  tsidx_distance::operator  +=(const int& p)
{
  first  +=  p;
  return *this;
}

inline tsidx_distance&  tsidx_distance::operator  -=(const tsidx_distance& p)
{
  *this += -p;
  return *this;
}

inline tsidx_distance&  tsidx_distance::operator  -=(const int& p)
{
  *this += -p;
  return *this;
}

inline tsidx_distance& tsidx_distance::normalize(frequency freq)
{
  // cerr << "NORM " << first << " " << second << endl;
  if (freq==unknown) return *this;

  if (first == 0 && second ==0) 
	{
	  return *this;
	}
  if ( second > freq )
	{
	  first ++;
	  second -= freq;
	  normalize(freq);
	  return *this;
	}

  if (second < -freq )
	{
	  first --;
	  second += freq ;	
	  normalize(freq);
	  return *this;
	}
  if ( second < 0) 
	{
	  first --;
	  second = (int)freq + second;
	  return *this;
	}
  if ( second > 0)
	{	  
	  return *this;
	}
    if ( second == 0) 
	{
	  first --;
	  second = freq;
	  return *this;
	}
  return *this;
}

}


#endif // _TSIDX_H

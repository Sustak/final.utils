//
// Copyright (c) 2004, 2005 Marek Sestak
// Released under the GNU GPL 2.0 license.
//

#ifndef datetimeH
#define datetimeH

#include <time.h>
#include <set>
#include "str.h"

namespace utils {

// ---------------------------------------------------------------------------

typedef enum { TT_Day=0, TT_Week=1, TT_Month=2, TT_Year=3 } TTermType;

// ---------------------------------------------------------------------------

/** \brief Class TDate acts as a main class for manipulation with dates.

    Internal representation of dates is based on serial dates.
    Each date uniquely corresponds to an integer number,
    where January 1st 1900 is defined as 1, January 2nd 1900 as 2, etc.
    Dates preceeding year 1900 cannot be constructed.
*/

class TDate
{
private:
  int     day, month, year;

  int     _Serial;
  int     _IsValid, _IsLeap, _IsMonthEnd, _WeekDay;
  int     _NotDefined;
  void    _Recalculate();
          // funkce _Recalculate() prepocita uvedene hodnoty
          // cilem je, aby se nemusely vycislovat znovu a znovu, jinymi slovy,
          // tyto hodnoty se prepocitaji pri kazde zmene day, month, year
          // a funkce IsLeap(), IsValid(), Serial(), IsMonthEnd(), WeekDay()
          // potom vraceji pouze kopii techto hodnot

public:
          TDate();                       ///< Default constructor. Date isn't defined.
          TDate( const time_t& adate );  ///< TDate's constructor based on time_t structure.
          TDate( const tm& adate );
          TDate( int adate, int amonth, int ayear );
          TDate( int aserial );
          TDate( double aserial );
          TDate( const TDate& adate );

  void    Clear();

  TDate&  operator = ( const TDate& adate );
  int     operator == ( const TDate& adate ) const;
  int     operator != ( const TDate& adate ) const;
  int     operator < ( const TDate& adate ) const;
  int     operator <= ( const TDate& adate ) const;
  int     operator > ( const TDate& adate ) const;
  int     operator >= ( const TDate& adate ) const;
  TDate   operator + ( const int anum ) const;
  TDate&  operator ++ ( int );
  int     operator - ( const TDate& adate ) const;
  TDate   operator - ( const int anum ) const;
  TDate&  operator -- ( int );

  void    Set( int aday, int amonth, int ayear );
          // nastavi den, mesic, rok
  void    Set( int aserial );
  void    Set( double aserial );
  void    Get( int& aday, int& amonth, int& ayear ) const ;
          // warning!!! normally the order is oposite (i.e. year, month, day)!!!
          // you'll have to get used to it... too much code depends on it :-(
  int     Day() const
          { return day; }
  void    Day( int aday )
          { day=aday; _Recalculate(); }
  int     Month() const
          { return month; }
  void    Month( int amonth )
          { month=amonth; _Recalculate(); }
  int     Year() const
          { return year; }
  void    Year( int ayear )
          { year=ayear; _Recalculate(); }

  int     IsLeap() const { return _IsLeap; };
          // nezjistuje, zda je den platny, pouze zkontroluje, zda je rok prestupny
          // 0...neprestupny
          // 1...prestupny
  void    Validate() const;
          // pokud je den platny, nedela nic
          // pokud neni platny, vyhodi vyjimku TExInvalidDate
  int     IsValid() const { return _IsValid; };
          // zkontroluje, zda se jedna o "platny" den
          // 0...neplatny
          // 1...ok
          // napr. 29.2.2001, 31.4., 31.9. atd. budou neplatne a tudiz funkce vraci 0
  int     IsDefined() const { return _NotDefined; };
  int     Serial() const;
          // vraci "poradove" cislo dne
          // 2 = 1.1.1900, 3 = 2.1.1900 atd.
          // vzhledem k chybe, kterou obsahuje implementace excelu, nebude souhlasit
          // cislovani s applixem a excelem az do 28.2.1900, od 1.3.1900 uz vse
          // souhlasi
  int     IsMonthEnd() const;
          // 1..prislusny den je posledni v mesici
          // 0..neni posledni v mesici

  int     Diff( const TDate& adate ) const;
          // vraci pocet dnu do adate

  int     WeekDay() const { return _WeekDay; }
          // 1..pondeli, 2..utery, atd... 6..sobota, 7..nedele
  int     Weekend() const { return _WeekDay>=6; }
          // true..je to vikend, false..neni to vikend

  TDate   EOMonth( int amonths ) const;
          // vraci posledni den mesice o amonths dale (zpet pro negativni hodnoty)
  void    SetToMonthEnd();
  TDate   EDate( int amonths ) const;
  TDate   AddTerm( int anumberofterms, TTermType atype ) const;

  TString DateString() const;
          // returns date string in the default format
  TString DateString( const TString& aformat ) const;
};

#ifndef USE_VCL

class TDateTime: public TDate
{
private:
  int time; // number of miliseconds
  TString format;

public:
  // Used by TDateTime(const TString& src)
  enum TDateTimeFlag {Date, Time, DateTime};

  static TDateTime CurrentDate();
  static TDateTime CurrentTime();
  static TDateTime CurrentDateTime();

  TDateTime():TDate()                                                 { time=0; }
  TDateTime(const TDate& src):TDate(src)                              { time=0; }
  TDateTime(const TDateTime& src):TDate(src.TDate::Serial())          { time=0; }
  TDateTime(double src):TDate()                                       { Set(src); }
  TDateTime(int src):TDate(src)                                       { time=0; }
  TDateTime(const time_t& src);
  TDateTime(int year, int month, int day):TDate(day,month,year)       { time=0; }
  TDateTime(int hour, int min, int sec, int msec);
  TDateTime(int year, int month, int day, int hour, int min, int sec, int msec );
  TDateTime(const TString& src, TDateTimeFlag flag = DateTime);


  void Set( int aserial );
  void Set( double aserial );
  void Set( int year, int month, int day );
  void Set( int hour, int min, int sec, int msec );
  void Set( int year, int month, int day, int hour, int min, int sec, int msec=0 );
  double Serial() const;

  TDateTime& operator =(const TDateTime& rhs);
  TDateTime& operator =(const TDate& rhs);
  TDateTime& operator =(double rhs);
  TDateTime& operator =(int rhs);

  TDateTime& operator +=(const TDate& rhs);
  TDateTime& operator +=(const TDateTime& rhs);
  TDateTime& operator +=(const double rhs);
  TDateTime& operator +=(const int rhs);

  TDateTime& operator -=(const TDate& rhs);
  TDateTime& operator -=(const TDateTime& rhs);
  TDateTime& operator -=(const double rhs);
  TDateTime& operator -=(const int rhs);

  TDateTime& operator ++();
  TDateTime operator ++(int);
  TDateTime& operator --();
  TDateTime operator --(int);

  TDateTime operator +(const TDate& rhs) const;
  TDateTime operator +(const TDateTime& rhs) const;
  TDateTime operator +(const double rhs) const;
  TDateTime operator +(const int rhs) const;

  TDateTime operator -(const TDate& rhs) const;
  TDateTime operator -(const TDateTime& rhs) const;
  TDateTime operator -(const double rhs) const;
  TDateTime operator -(const int rhs) const;

  bool operator ==(const TDate& rhs) const;
  bool operator !=(const TDateTime& rhs) const;
  bool operator >(const TDateTime& rhs) const;
  bool operator <(const TDateTime& rhs) const;
  bool operator >=(const TDateTime& rhs) const;
  bool operator <=(const TDateTime& rhs) const;

  operator TString() const;
  operator double() const;
  operator int() const;

  TString  FormatString()                          { return format; }
  TString  FormatString(const TString& anewformat);
           // Sets a new format format that will be used in
           // DateString, TimeString and DateTimeString routines.
           // Format string is also used in
           // TDateTime::TDateTime( const TString& ) constructor!

  TString  DateString() const;
  TString  TimeString() const;
  TString  DateTimeString() const;
           // if format isn't set, default formats are used (see formats.h)

  int      DayOfWeek() const;

  void     DecodeDate(int* year, int* month, int* day) const;
  void     DecodeTime(int* hour, int* min, int* sec, int* msec) const;

  double   TimeToDouble() const;
};

#endif // USE_VCL

TDate            Today();
// returns current day

TDateTime        Now();
// returns current time and day

int              IsLeap( int ayear );
// rok je prestupny, pokud je delitelny 4 a zaroven
// ( neni delitelny 100 nebo je delitelny 400 )
// tzn. 2000 je prestupny, 2100 nebude...

TDate            EOMonth( const TDate& astart, int amonths );
// vraci konec mesice o amonths mesicu dale

TDate            EDate( const TDate& astart, int amonths );
// vraci den o amonths mesicu dale

// ---------------------------------------------------------------------------

} // namespace utils

#endif // datatimeH


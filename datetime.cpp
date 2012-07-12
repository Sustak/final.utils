
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

#include <math.h>

#include "datetime.h"
#include "utils.h"

namespace utils {

// --------------------------------------------------------------------------

const int days_in_month[] = {31,28,31,30,31, 30, 31, 31, 30, 31, 30, 31};
const int days_in_month_cum[] = {0, 31,59,90,120,151,181,212,243,273,304,334};
const int msecsperday = 86400000;
const double msecsperdayl = 86400000.0;

// --------------------------------------------------------------------------

TDate::TDate()
{
  Clear();
}

TDate::TDate( const tm& adate )
{
  day = adate.tm_mday;
  month = adate.tm_mon+1;
  year = adate.tm_year+1900;
  _Recalculate();
}

TDate::TDate( const time_t& adate )
{
  tm *tblock;
  tblock = localtime( &adate );
  day = tblock->tm_mday;
  month = tblock->tm_mon+1;
  year = tblock->tm_year+1900;
  _Recalculate();
}

TDate::TDate( int aday, int amonth, int ayear )
{
  day = aday;
  month = amonth;
  year = ayear;
  _Recalculate();
}

TDate::TDate( const TDate& adate )
{
  day=adate.day;
  month=adate.month;
  year=adate.year;
  _Recalculate();
}

TDate::TDate( int aserial )
{
  Set( aserial );
}

TDate::TDate( double aserial )
{
  int aintserial = (int) floor(aserial);
  Set( aintserial );
}

int TDate::operator == ( const TDate& adate ) const
{
  return( day==adate.day && month==adate.month && year==adate.year );
}

int TDate::operator != ( const TDate& adate ) const
{
  return( day!=adate.day || month!=adate.month || year!=adate.year );
}

int TDate::operator < ( const TDate& adate ) const
{
  return( Serial() < adate.Serial() );
}

int TDate::operator <= ( const TDate& adate ) const
{
  return( Serial() <= adate.Serial() );
}

int TDate::operator > ( const TDate& adate ) const
{
  return( Serial() > adate.Serial() );
}

int TDate::operator >= ( const TDate& adate ) const
{
  return( Serial() >= adate.Serial() );
}

TDate TDate::operator + ( const int anum ) const
{
  TDate dt;
  dt.Set( Serial()+anum );
  return dt;
}

TDate& TDate::operator ++ ( int )
{
  Set( Serial()+1 );
  return *this;
}

int TDate::operator - ( const TDate& adate ) const
{
  return Serial()-adate.Serial();
}

TDate TDate::operator - ( const int anum ) const
{
  return (*this)+(-anum);
}

TDate& TDate::operator -- ( int )
{
  Set( Serial()-1 );
  return *this;
}

TDate& TDate::operator = ( const TDate& adate )
{
  if( this == &adate ) return *this;
  day = adate.day;
  month = adate.month;
  year = adate.year;
  _Serial = adate._Serial;
  _IsValid = adate._IsValid;
  _IsLeap = adate._IsLeap;
  _IsMonthEnd = adate._IsMonthEnd;
  _WeekDay = adate._WeekDay;
  _NotDefined = adate._NotDefined;
  return *this;
}

void TDate::Validate() const
{
  if( !IsValid() )
  {
    throw TException( "Invalid date: " + DateToString( *this, "DD.MM.YYYY" ) );
  }
}

void TDate::_Recalculate()
{
  _IsValid = _IsLeap = _Serial = _IsMonthEnd = _WeekDay = _NotDefined = 0;

  // IsLeap
  _IsLeap=utils::IsLeap(year);

  // IsValid
  _IsValid=true;
  if( year<1900 )                                   _IsValid=false;
  else
    if( month<1 || month>12 )                       _IsValid=false;
    else
      if( month==2 && IsLeap() )
      {
        if( day<1 || day>29 )                       _IsValid=false;
      }
      else
        if( day<1 || day>days_in_month[month-1] )   _IsValid=false;

  if( _IsValid==false )
    return;

  // IsMonthEnd
  if( IsLeap() && month==2 )
  {
    if( day==29 )
      _IsMonthEnd=true;
    else
      _IsMonthEnd=false;
  }
  else
  {
    if( day==days_in_month[month-1] )
      _IsMonthEnd=true;
    else
      _IsMonthEnd=false;
  }

  // Serial
  int n;
  n = (year-1900)*365 + 2;
     // applix a excel na tomto miste predpokladaji, ze
     // 1.1.1900 je den se seriovym cislem 1.
     // jenomze prvni chybu delaji uz ve chvili, kdy predpokladaji, ze
     // 29.2.1900 existuje, jenomze tomu tak neni.
     // applix suse predstira, ze den se seriovym cislem neexistuje,
     // zatimco excel ho stanovi, jako 29.2.1900...
     //
     // abychom zustali v souladu
     // se seriovymi cisly excelu, nesouhlasi nam prvnich par dni roku 1900,
     // od 1.3.1900 uz vse souhlasi
  n = n + (year-1901)/4;
  n = n - (year-1901)/100;
  n = n + (year-1601)/400;
  n = n + days_in_month_cum[month-1] + day - 1;
  if( IsLeap() && month>2 ) n++;
  _Serial = n;

  // WeekDay
  _WeekDay = (Serial()-2)%7+1;

}

int TDate::IsMonthEnd() const
{
  Validate();
  return _IsMonthEnd;
}

void TDate::Clear() {
  _NotDefined = true;
  _Serial = 0;
  _IsValid = false;
  _IsLeap = false;
  _IsMonthEnd = false;
  _WeekDay = 0;
  day = 0;
  month = 0;
  year = 0;
}

void TDate::Set( int aserial )
{
  if( aserial<0 )
    throw TException( "Dates with negative serial numbers not allowed." );

  if( aserial==0 ) {
    Clear();
    return;
  }

  if( aserial<367 ) year=1900;
  else
  {
    int int_leap;
    TDate dt;

    year = 1900+(aserial-2-((aserial-365)/1460)+(aserial-368)/36524
                          -(aserial+109205)/(146096) ) /365;
    int_leap = utils::IsLeap(year);

    dt=TDate(1,1,year);
    aserial = aserial-dt.Serial()+1;
    for( month=1;
         month<13 && aserial>((month==2&&int_leap?1:0)+days_in_month[month-1]);
         month++ )
      aserial = aserial - days_in_month[month-1] - (month==2&&int_leap?1:0);

    day = aserial;


  }
  _Recalculate();

}

int TDate::Serial() const
{
  if( _NotDefined ) return 0;

  Validate();
  return _Serial;
}

int TDate::Diff( const TDate& adate ) const
{
  // pokud nebude adate nebo soucasny den platny, funkce Serial
  // vyhodi vyjimku, proto neni platnost kontrolovat v teto funkci
  return adate.Serial()-Serial();
}

void TDate::Set( int aday, int amonth, int ayear )
{
  day = aday;
  month = amonth;
  year = ayear;
  _Recalculate();
}

void TDate::Get( int& aday, int& amonth, int& ayear ) const
{
  aday = day;
  amonth = month;
  ayear = year;
}

TDate TDate::EOMonth( int amonths ) const
{
  TDate dt;
  dt = utils::EOMonth( *this, amonths );

  return dt;
}

void TDate::SetToMonthEnd()
{
  day = (utils::IsLeap(year)&&month==2? 29 : days_in_month[month-1]);
  _Recalculate();
}

TDate TDate::EDate( int amonths ) const
{
  TDate dt;
  dt = utils::EDate( *this, amonths );

  return dt;
}

TDate TDate::AddTerm( int anumberofterms, TTermType atype ) const
{
  TDate dt;
  int s = Serial();
  switch( atype )
  {
    case TT_Day:
      dt.Set( s + anumberofterms );
      break;
    case TT_Week:
      dt.Set( s + 7*anumberofterms );
      break;
    case TT_Month:
      dt = EDate( anumberofterms );
      break;
    case TT_Year:
      dt = EDate( 12*anumberofterms );
      break;
    default:
      throw TException( "Type of term not recognized." );
  }
  return dt;
}

TString TDate::DateString() const
{
  return DateToString( *this );
}

TString TDate::DateString( const TString& format ) const
{
  if( format=="" )
    return DateToString( *this );
  else
    return DateToString( *this, format );
}

// --------------------------------------------------------------------------


#ifndef USE_VCL

TDateTime::TDateTime( const time_t& adate ): TDate()
{
  tm *tblock;
  tblock = localtime( &adate );

  Set( tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday,
       tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
}

TDateTime::TDateTime(const TString& src, TDateTimeFlag flag)
{
  switch( flag ) {
    case Date:
         if( format=="" )
           *this = StringToDate( src );
         else
           *this = StringToDate( src, format );
         break;
    case Time:
         if( format=="" )
           *this = StringToTime( src );
         else
           *this = StringToTime( src, format );
         break;
    case DateTime:
         if( format=="" )
           *this = StringToDateTime( src );
         else
           *this = StringToDateTime( src, format );
         break;
    default:
         throw TException( "Unknown value of TDateTimeFlag." );
  }
}

TDateTime::TDateTime(int year, int month, int day, int hour, int min, int sec, int msec )
{
  Set( year, month, day, hour, min, sec, msec );
}

TDateTime::TDateTime(int hour, int min, int sec, int msec): TDate()
{
  Set( hour, min, sec, msec );
}

TDateTime TDateTime::CurrentTime()
{
  time_t now = ::time( NULL );
  tm *tblock;
  tblock = localtime( &now );
  TDateTime ret( tblock->tm_hour, tblock->tm_min, tblock->tm_sec, 0 );

  return ret;
}

TDateTime TDateTime::CurrentDate()
{
  return TDateTime( Today() );
}

TDateTime TDateTime::CurrentDateTime()
{
  return TDateTime( Now() );
}

void TDateTime::Set( int aserial )
{
  TDate::Set(aserial); time = 0;
}

void TDateTime::Set( int year, int month, int day )
{
  TDate::Set(day,month,year);
}

void TDateTime::Set( double aserial )
{
  double adate = floor(aserial);
  time = (int) Round( (aserial - adate) * msecsperdayl, 0 );
  TDate::Set( (int)adate );
}

void TDateTime::Set( int hour, int min, int sec, int msec )
{
  if( hour>23 || min>59 || sec>59 || msec>999 || hour<0 || min<0 || sec<0 || msec<0 )
    throw TException( "Invalid time format (" + TString(hour) + "." +
          TString(min) + "." + TString(sec) + "." + TString(msec) + ")" );

  time = hour;
  time *= 60;
  time += min;
  time *= 60;
  time += sec;
  time *= 1000;
  time += msec;
}

void TDateTime::Set( int year, int month, int day, int hour, int min, int sec, int msec )
{
  Set( year, month, day );
  Set( hour, min, sec, msec );
}

double TDateTime::Serial() const
{
  return TDate::Serial()+((double)time)/msecsperdayl;
}

void TDateTime::DecodeDate(int* year, int* month, int* day) const
{
  TDate::Get( *day, *month, *year );
}

void TDateTime::DecodeTime(int* hour, int* min, int* sec, int* msec) const
{
  int t = time;
  *msec = t % 1000;
  t /= 1000;
  *sec = t % 60;
  t /= 60;
  *min = t % 60;
  t /= 60;
  *hour = t;

  return;
/*  t *= 24.0;
  if( t>=24.0 )
    t = fmod( t, 24.0 );
  *hour = (int) t;
  t -= *hour;
  if( CompareDoubles( t, 1
  t *= 60.0;
  *min = (int) t;
  t -= floor(t);
  t *= 60.0;
  *sec = (int) t;
  t -= floor(t);
  t *= 1000.0;
  *msec = (int) t; */
}

TString TDateTime::FormatString(const TString& anewformat)
{
  TString old = format;
  format = anewformat;
  return old;
}

TString TDateTime::DateString() const
{
  if( format=="" )
    return DateToString( *this );
  else
    return DateToString( *this, format );
}

TString TDateTime::TimeString() const
{
  if( format=="" )
    return TimeToString( *this );
  else
    return TimeToString( *this, format );
}

TString TDateTime::DateTimeString() const
{
  if( format=="" )
    return DateTimeToString( *this );
  else
    return DateTimeToString( *this, format );
}

int TDateTime::DayOfWeek() const
{
  return TDate::WeekDay();
}

double TDateTime::TimeToDouble() const
{
  return ((double)time)/msecsperdayl;
}

bool TDateTime::operator ==(const TDate& rhs) const
{
  return IsEqual(Serial(), rhs.Serial());
}

bool TDateTime::operator !=(const TDateTime& rhs) const
{
  return !IsEqual(Serial(), rhs.Serial());
}

bool TDateTime::operator >(const TDateTime& rhs) const
{
  return CompareDoubles(Serial(), rhs.Serial())>0;
}

bool TDateTime::operator <(const TDateTime& rhs) const
{
  return CompareDoubles(Serial(), rhs.Serial())<0;
}

bool TDateTime::operator >=(const TDateTime& rhs) const
{
  return CompareDoubles(Serial(), rhs.Serial())>=0;
}

bool TDateTime::operator <=(const TDateTime& rhs) const
{
  return CompareDoubles(Serial(), rhs.Serial())<=0;
}

TDateTime::operator double() const
{
  return Serial();
}

TDateTime::operator int() const
{
  return (int)Serial();
}

TDateTime& TDateTime::operator =(const TDateTime& rhs)
{
  TDate::operator=( rhs );
  time = rhs.time;
  format = rhs.format;
  return *this;
}

TDateTime& TDateTime::operator =(const TDate& rhs)
{
  TDate::operator=( rhs );
  time = 0;
  return *this;
}

TDateTime& TDateTime::operator =(double rhs)
{
  Set(rhs);
  return *this;
}

TDateTime& TDateTime::operator =(int rhs)
{
  Set(rhs);
  return *this;
}

TDateTime& TDateTime::operator +=(const TDate& rhs)         { Set(Serial()+rhs.Serial()); return *this; }
TDateTime& TDateTime::operator +=(const TDateTime& rhs)     { Set(Serial()+rhs.Serial()); return *this; }
TDateTime& TDateTime::operator +=(const double rhs)         { Set(Serial()+rhs); return *this; }
TDateTime& TDateTime::operator +=(const int rhs)            { Set(Serial()+rhs); return *this; }

TDateTime& TDateTime::operator -=(const TDate& rhs)         { Set(Serial()-rhs.Serial()); return *this; }
TDateTime& TDateTime::operator -=(const TDateTime& rhs)     { Set(Serial()-rhs.Serial()); return *this; }
TDateTime& TDateTime::operator -=(const double rhs)         { Set(Serial()-rhs); return *this; }
TDateTime& TDateTime::operator -=(const int rhs)            { Set(Serial()-rhs); return *this; }

TDateTime& TDateTime::operator ++()                         { Set(Serial()+1); return *this; }
TDateTime TDateTime::operator ++(int)                       { TDateTime tmp(*this); Set(Serial()-1); return tmp; }
TDateTime& TDateTime::operator --()                         { Set(Serial()+1); return *this; }
TDateTime TDateTime::operator --(int)                       { TDateTime tmp(*this); Set(Serial()-1); return tmp; }

TDateTime TDateTime::operator +(const TDate& rhs) const     { return Serial()+rhs.Serial(); }
TDateTime TDateTime::operator +(const TDateTime& rhs) const { return Serial()+rhs.Serial(); }
TDateTime TDateTime::operator +(const double rhs) const     { return Serial()+rhs; }
TDateTime TDateTime::operator +(const int rhs) const        { return Serial()+rhs; }

TDateTime TDateTime::operator -(const TDate& rhs) const     { return Serial()-rhs.Serial(); }
TDateTime TDateTime::operator -(const TDateTime& rhs) const { return Serial()-rhs.Serial(); }
TDateTime TDateTime::operator -(const double rhs) const     { return Serial()-rhs; }
TDateTime TDateTime::operator -(const int rhs) const        { return Serial()-rhs; }

#endif // USE_VCL

// --------------------------------------------------------------------------

TDate Today()
{
  time_t now = time( NULL );
  return TDate( now );
}

TDateTime Now()
{
  time_t now = time( NULL );
  return TDateTime( now );
}

int IsLeap( int ayear )
{
  if( ayear%4 == 0 )
  {
    if( ayear%100 == 0 )
      if( ayear%400 == 0 )
        return true;
      else
        return false;
    return true;
  }
  return false;
}

TDate EOMonth( const TDate& astart, int amonths )
{
  TDate result;
  int d,m,y;

  astart.Validate();
  astart.Get( d,m,y );

  y = y + amonths/12;
  m = m + amonths%12;

  if( m<1 )
  {
    m += 12;
    y--;
  }
  else
    if( m>12 )
    {
      m -= 12;
      y++;
    }

  if( IsLeap(y) && m==2 )
    d = 29;
  else
    d = days_in_month[ m-1 ];

  result.Set( d,m,y );
  return result;
}

TDate EDate( const TDate& astart, int amonths )
{
  TDate result;
  int d,m,y;

  astart.Validate();
  astart.Get(d,m,y);

  y = y + amonths/12;
  m = m + amonths%12;

  if( m<1 )
  {
    m += 12;
    y--;
  }
  else
    if( m>12 )
    {
      m -= 12;
      y++;
    }


  if( d>days_in_month[ m-1 ] )
    if( IsLeap(y) && m==2 )
      d = 29;
    else
      d = days_in_month[ m-1 ];

  result.Set( d,m,y );
  return result;
}

} // end of namespace utils

// final.utils - Utility functions used by final projects.
// Copyright (c) 2004 - 2012 by Marek Sestak, marek.sestak@gmail.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "formats.h"
#include "maths.h"

#include <math.h>

namespace utils {

//---------------------------------------------------------------------------

TString GetFirstLine( TString &astring )
{
  if( astring.Length()==0 )
    return "";

  TString ret;
  int pos;
  if( (pos=astring.Pos("\n"))==0 )
  {
    ret = astring;
    astring = "";
    return ret;
  }

  ret = astring.SubString( 1, pos-1 );
  if( pos<astring.Length() )
    astring.Delete( 1, pos );
  else
    astring="";

  return ret;
}

TString FirstLine( const TString &astring )
{
  if( astring.Length()==0 )
    return "";

  TString ret;
  int pos;
  if( (pos=astring.Pos("\n"))==0 )
  {
    ret = astring;
    return ret;
  }

  ret = astring.SubString( 1, pos-1 );

  return ret;
}

TString GetNextString( TStringVector &sv )
{
  TString ret;
  if( sv.size()>0 ) {
    ret = sv[0];
    sv.erase( sv.begin() );
  }
  return ret;
}

void StringVectorToString( const TStringVector &sv, TString &string, char adelimiter )
{
  int totallength = 0;
  int n = sv.size();
  int i;

  for( i=0; i<n; i++ )
    totallength += sv[i].Length();
  totallength += n-1;
  // will add n-1 delimiters

  string.SetLength( totallength );

  char *c = string.c_str();
  const TString *s;

  int nrecords = sv.size();
  for( i=0; i<nrecords; i++ ) {
    s = &( sv[i] );
    strcpy( c, s->c_str() );
    c += s->Length();
    if( i<(nrecords-1) ) {
      *c = adelimiter;
      c++;
    }
  }
}

int CountChar( const char* &astr )
{
  const char *c = astr;
  if( *c==0 )
    return 0;

  do
  {
    astr++;
  }
  while( *astr==*c );
  
  return( astr-c );
}

bool IsDigit( char c )
{
  if( c>='0' && c<='9' )
    return true;
  else
    return false;
}

bool IsWhiteSpace( char c )
{
  return ( (c==' ') || (c=='\t') );
}

void SkipWhiteSpace( const char* &astr )
{
  while( (*astr==' ' || *astr=='\t') && *astr!=0 )
    astr++;
}

int GetN( const char* &astr, int n=0 )
{
  int ret=0;
  int j;

  for( j=0; (IsDigit(*astr)) && ((n==0)||(j<n)); j++ )
  {
    ret *= 10;
    ret += *astr-'0';
    astr++;
  }

  if( (n>0) && (j<n) )
    throw TException( "Improper integer format (" + TString(n) + " decimal(s) expected)." );
  if( (n==0) && (j==0) )
    throw TException( "Improper integer format." );

  return ret;
}

TDateTime StringToDate( const TString& astr, const TString& aformat )
{
  int n;
  char c;

  int day, month, year;
  day = month = year /* = hour = minute = sec*/ = -1;

  TString upcasefmt = aformat.UpperCase();
  const char *fmt = upcasefmt.c_str();
  const char *str = astr.c_str();

  while( *fmt!=0 )
  {
    if( IsWhiteSpace( *fmt ) )
    {
      SkipWhiteSpace( fmt );
      SkipWhiteSpace( str );
    }
    else
    {
      if( *str==0 )
        throw TException( "Cannot convert string to date/time format: unexpected end of string." );
      SkipWhiteSpace( str );
      switch( *fmt )
      {
        case 'D':
          if( day!=-1 )
            throw TException( "Invalid date format: day can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            day = GetN( str );
          else
            day = GetN( str, n );
          break;
        case 'M':
          if( month!=-1 )
            throw TException( "Invalid date format: month can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            month = GetN( str );
          else
            month = GetN( str, n );
          break;
        case 'Y':
          if( year!=-1 )
            throw TException( "Invalid date format: year can only be defined once." );
          n = CountChar( fmt );
          if( n==3 )
            throw TException( "Invalid date format: 'YYY' not allowed for years." );
          if( n==1 )
            year = GetN( str );
          else
            year = GetN( str, n );
          if( n==2 )
            if( year>50 )
              year += 1900;
            else
              year += 2000;
          break;
        default:
          if( *str==*fmt )
          {
            str++;
            fmt++;
          }
          else
            throw TException( "Cannot convert string to date/time format." );
      }
    }
  }
  if( day==-1 )
    throw TException( "Invalid date format: day hasn't been defined." );
  if( month==-1 )
    throw TException( "Invalid date format: month hasn't been defined." );
  if( year==-1 )
    throw TException( "Invalid date format: year hasn't been defined." );

  return TDateTime( year, month, day );
}

TDateTime StringToDateTime( const TString& astr, const TString& aformat )
{
  int n;
  char c;

  int day,month,year,hour,minute,sec;
  day = month = year = hour = minute = sec = -1;

  const char *fmt = aformat.c_str();
  const char *str = astr.c_str();
  char fmtc;

  while( *fmt!=0 )
  {
    if( IsWhiteSpace( *fmt ) )
    {
      SkipWhiteSpace( fmt );
      SkipWhiteSpace( str );
    }
    else
    {
      if( *str==0 )
        throw TException( "Cannot convert string to date/time format: unexpected end of string." );
      SkipWhiteSpace( str );
      switch( *fmt )
      {
        case 'D':
        case 'd':
          if( day!=-1 )
            throw TException( "Invalid date format: day can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            day = GetN( str );
          else
            day = GetN( str, n );
          break;
        case 'M':
          if( month!=-1 )
            throw TException( "Invalid date format: month can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            month = GetN( str );
          else
            month = GetN( str, n );
          break;
        case 'Y':
        case 'y':
          if( year!=-1 )
            throw TException( "Invalid date format: year can only be defined once." );
          n = CountChar( fmt );
          if( n==3 )
            throw TException( "Invalid date format: 'YYY' not allowed for years." );
          if( n==1 )
            year = GetN( str );
          else
            year = GetN( str, n );
          if( n==2 )
            if( year>50 )
              year += 1900;
            else
              year += 2000;
          break;
        case 'h':
        case 'H':
          if( hour!=-1 )
            throw TException( "Invalid time format: hour can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            hour = GetN( str );
          else
            hour = GetN( str, n );
          break;
        case 'm':
          if( minute!=-1 )
            throw TException( "Invalid time format: minute can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            minute = GetN( str );
          else
            minute = GetN( str, n );
          break;
        case 's':
        case 'S':
          if( sec!=-1 )
            throw TException( "Invalid time format: second can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            sec = GetN( str );
          else
            sec = GetN( str, n );
          break;
        default:
          if( *str==*fmt )
          {
            str++;
            fmt++;
          }
          else
            throw TException( "Cannot convert string to date/time format." );
      }
    }
  }
  if( day==-1 )
    throw TException( "Invalid date format: day hasn't been defined." );
  if( month==-1 )
    throw TException( "Invalid date format: month hasn't been defined." );
  if( year==-1 )
    throw TException( "Invalid date format: year hasn't been defined." );
  if( hour==-1 )
    hour = 0;
  if( minute==-1 )
    minute = 0;
  if( sec==-1 )
    sec = 0;

  return TDateTime( year, month, day, hour, minute, sec, 0 );
}

TDateTime StringToTime( const TString& astr, const TString& aformat )
{
  int n;
  char c;

  int hour,minute,sec;
  hour = minute = sec = -1;

  TString upcasefmt = aformat.UpperCase();
  const char *fmt = upcasefmt.c_str();
  const char *str = astr.c_str();

  while( *fmt!=0 )
  {
    if( IsWhiteSpace( *fmt ) )
    {
      SkipWhiteSpace( fmt );
      SkipWhiteSpace( str );
    }
    else
    {
      if( *str==0 )
        throw TException( "Cannot convert string to date/time format: unexpected end of string." );
      SkipWhiteSpace( str );
      switch( *fmt )
      {
        case 'H':
          if( hour!=-1 )
            throw TException( "Invalid time format: hour can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            hour = GetN( str );
          else
            hour = GetN( str, n );
          break;
        case 'M':
          if( minute!=-1 )
            throw TException( "Invalid time format: minute can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            minute = GetN( str );
          else
            minute = GetN( str, n );
          break;
        case 'S':
          if( sec!=-1 )
            throw TException( "Invalid time format: second can only be defined once." );
          n = CountChar( fmt );
          if( n==1 )
            sec = GetN( str );
          else
            sec = GetN( str, n );
          break;
        default:
          if( *str==*fmt )
          {
            str++;
            fmt++;
          }
          else
            throw TException( "Cannot convert string to time format." );
      }
    }
  }
  if( hour==-1 )
    hour = 0;
  if( minute==-1 )
    minute = 0;
  if( sec==-1 )
    sec = 0;

  return TDateTime( hour, minute, sec, 0 );
}

TString DateToString( const TDate& adate, const TString& aformat )
{
  TDateTime dt(adate);
  return DateToString( dt, aformat );
}

TString DateToString( const TDateTime& adate, const TString& aformat )
{
  TString ret;
  TString partform;

  TString upcasefmt = aformat.UpperCase();
  const char *fmt = upcasefmt.c_str();
  int day, month, year;
  if( adate.IsValid() )
    adate.DecodeDate( &year, &month, &day );
  else {
    year = 0; month = 0; day = 0;
  }

  char fmtc;

  while( *fmt )
  {
    fmtc = *fmt;
    int n = CountChar( fmt );
    if( n == 1 )
      partform = "%d";
    else
      partform = "%0" + TString(n) + "d";

    switch( fmtc )
    {
      case 'D':
        if( day==-1 )
          throw TException( "Invalid date format: day defined multiple times." );
        ret.cat_sprintf( partform.c_str(), day );
        day = -1;
        break;
      case 'M':
        if( month==-1 )
          throw TException( "Invalid date format: month defined multiple times." );
        ret.cat_sprintf( partform.c_str(), month );
        month = -1;
        break;
      case 'Y':
        if( year==-1 )
          throw TException( "Invalid date format: year defined multiple times." );
        int y;
        if( n==2 )
          y = year%100;
        else {
          if( n==3 )
            throw TException( "Invalid date format: year cannot be defined in three decimals." );
          y = year;
        }
        ret.cat_sprintf( partform.c_str(), y );
        year = -1;
        break;
      default:
        ret += TString( fmtc, n );
    }
//    fmt++;
  }
  return ret;
}

TString TimeToString( const TDateTime& atime, const TString& aformat )
{
  TString ret;
  TString partform;

  TString upcasefmt = aformat.UpperCase();
  const char *fmt = upcasefmt.c_str();
  int hour, min, sec, msec;
  atime.DecodeTime( &hour, &min, &sec, &msec );
  char fmtc;

  while( *fmt )
  {
    fmtc = *fmt;
    int n = CountChar( fmt );
    if( n == 1 )
      partform = "%d";
    else
      partform = "%0" + TString(n) + "d";

    switch( fmtc )
    {
      case 'H':
        if( hour==-1 )
          throw TException( "Invalid time format: hours defined multiple times." );
        ret.cat_sprintf( partform.c_str(), hour );
        hour = -1;
        break;
      case 'M':
        if( min==-1 )
          throw TException( "Invalid time format: minutes defined multiple times." );
        ret.cat_sprintf( partform.c_str(), min );
        min = -1;
        break;
      case 'S':
        if( sec==-1 )
          throw TException( "Invalid time format: seconds defined multiple times." );
        ret.cat_sprintf( partform.c_str(), sec );
        sec = -1;
        break;
      default:
        ret += TString( fmtc, n );
    }
//    fmt++;
  }
  return ret;
}

TString DateTimeToString( const TDateTime& adate, const TString& aformat )
{
  TString ret;
  TString partform;

  int day, month, year;
  if( adate.IsValid() )
    adate.DecodeDate( &year, &month, &day );
  else {
    year = 0; month = 0; day = 0;
  }
  int hour, min, sec, msec;
  adate.DecodeTime( &hour, &min, &sec, &msec );

  const char *fmt = aformat.c_str();
  char fmtc;

  while( *fmt )
  {
    fmtc = *fmt;
    int n = CountChar( fmt );
    if( n == 1 )
      partform = "%d";
    else
      partform = "%0" + TString(n) + "d";

    switch( fmtc )
    {
      case 'D':
      case 'd':
        if( day==-1 )
          throw TException( "Invalid date format: day defined multiple times." );
        ret.cat_sprintf( partform.c_str(), day );
        day = -1;
        break;
      case 'M':
        if( month==-1 )
          throw TException( "Invalid date format: month defined multiple times." );
        ret.cat_sprintf( partform.c_str(), month );
        month = -1;
        break;
      case 'Y':
      case 'y':
        if( year==-1 )
          throw TException( "Invalid date format: year defined multiple times." );
        int y;
        if( n==2 )
          y = year%100;
        else if( n==3 )
          throw TException( "Invalid date format: year cannot be defined in three decimals." );
        else
          y = year;
        ret.cat_sprintf( partform.c_str(), y );
        year = -1;
        break;
      case 'h':
      case 'H':
        if( hour==-1 )
          throw TException( "Invalid time format: hours defined multiple times." );
        ret.cat_sprintf( partform.c_str(), hour );
        hour = -1;
        break;
      case 'm':
        if( min==-1 )
          throw TException( "Invalid time format: minutes defined multiple times." );
        ret.cat_sprintf( partform.c_str(), min );
        month = -1;
        break;
      case 's':
      case 'S':
        if( sec==-1 )
          throw TException( "Invalid time format: seconds defined multiple times." );
        ret.cat_sprintf( partform.c_str(), sec );
        sec = -1;
        break;
      default:
        ret += TString( fmtc, n );
    }
//    fmt++;
  }
  return ret;
}

// --------------------------------------------------------------------------

TString FloatToTextFmt( double value, const TString& str )
{
  // warning! i have absolutely no idea, whether it's all implemented well...
  // please write further tests and continue improving the procedure.
  /* TODO :
     i think an additional feature might be implemented:
     asteriks '*' character after the decimal point could print out the remainder regardless how many decimal places there are
     (so for 0.* and value 1.0 the output would be 1, for 1.23 >> 1.23 etc...) */

  TString ret;
  bool        dq = false;
  bool        sq = false;
  const char* fmt = str.c_str();
  const char* fmtStart;             // start of the string
  const char* fmtEnd;               // points at the last character of the format string in a given section
  const char* fmtStarts[2];         // pointers to first two ';' characters in string str
  int         i = 0;
  int         p;

  if( *fmt==0 ) {
    ret.printf( "%f", value );
    return ret;
  }

  while( *fmt!=0 && i<2 ) {
    switch( *fmt ) {
    case '\'':
         if( !dq ) sq = !sq;
         break;
    case '\"':
         if( !sq ) dq = !dq;
         break;
    default:
         if( !(sq||dq) ) {
           if( *fmt==';' ) {
             fmtStarts[i] = fmt;
             i++;
           }
         }
    }
    fmt++;
  }

  p = CompareDoubles( value, 0.0 );

  switch( p ) {
  case -1:
       if( i>0 ) {  // i>0 means at least one ';' character found
         fmtStart = fmtStarts[0]+1;
         if( i>1 ) // i>1 means at least two ';' characters found
           fmtEnd = fmtStarts[1];
         else
           fmtEnd = str.c_str() + str.Length();
       }
       else {
         fmtStart = str.c_str();
         fmtEnd = fmtStart + str.Length();
       }
       break;
  case 1:
       fmtStart = str.c_str();
       if( i>0 )
         fmtEnd = fmtStarts[0];
       else
         fmtEnd = str.c_str() + str.Length();
       break;
  case 0:
       if( i>1 )
         fmtStart = fmtStarts[1]+1;
       else
         fmtStart = str.c_str();
       fmtEnd = str.c_str() + str.Length();
       break;
  default:
       // uhhh... a trouble... let's return at least something...
       ret.printf( "%Lf", value );
       return ret;
  }

  const char* fmtNumStart = NULL;               // points to the first '?','0' or '#' char in format string
  int         fmtlen = fmtEnd - fmtStart;

  int         len;
  bool        valuenegative = false;
  int         decimalPoint = -1;
  int         fixeddecimals = 0;
  int         floatdecimals = 0;
  int         fixeddecimalstotal = 0;
  int         floatdecimalstotal = 0;
  int         E = -1;
  char        Echar;
  int         Edecimals = 0;
  bool        thousandseparator = false;
  char        pc, nc;

  if( value<0 ) {
    value = -value;
    valuenegative = true;
  }

  dq = false;
  sq = false;
  fmt = fmtStart;
  while( fmt<fmtEnd ) {
    switch( *fmt ) {
    case '\'':
         if( !dq ) sq = !sq;
         fmt++;
         break;
    case '\"':
         if( !sq ) dq = !dq;
         fmt++;
         break;
    default:
         if( !(sq||dq) ) {
           switch( *fmt ) {
           case ',':
                if( !thousandseparator && fmt>fmtStart && E==-1 && decimalPoint==-1) {
                  pc = *(fmt-1);
                  if( pc=='#' || pc=='?' || pc=='0' ) {
                    nc = *(fmt+1);
                    if( pc=='#' || pc=='?' || pc=='0' )
                      thousandseparator = true;
                  }
                }
                fmt++;
                break;
           case '.':
                if( !fmtNumStart ) fmtNumStart = fmt;
                if( decimalPoint==-1 && E==-1 )
                  decimalPoint = fmt-fmtStart;
                fmt++;
                break;
           case 'E':
           case 'e':
                if( E==-1 ) {
                  nc = *(fmt+1);
                  if( nc=='+' || nc=='-' ) {
                    if( !fmtNumStart ) fmtNumStart = fmt;
                    E = fmt-fmtStart;
                    Echar = *fmt;
                    fmt++;
                  }
                }
                fmt++;
                break;
           case '0':
                if( E!=-1 )
                  Edecimals++;
                else if( decimalPoint==-1 ) {
                  if( !fmtNumStart ) fmtNumStart = fmt;
                  fixeddecimals++;
                  fixeddecimalstotal++;
                }
                else {
                  floatdecimalstotal++;
                  floatdecimals = floatdecimalstotal;
                }
                fmt++;
                break;
           case '?':
           case '#':
                if( E!=-1 ) {
                  if( Edecimals>0 ) Edecimals++;
                } else if( decimalPoint==-1 ) {
                  if( !fmtNumStart ) fmtNumStart = fmt;
                  if( fixeddecimals>0 ) fixeddecimals++;
                  fixeddecimalstotal++;
                } else
                  floatdecimalstotal++;
                fmt++;
                break;
           default:
                fmt++;
           } // switch
         } // if( !(dq||sq) )
         else
           fmt++;
    } // switch
  } // while

  if( Edecimals==0 )
    Edecimals = 3;

  TString     num, exp;
  int         numlen;
  int         numpos;
  const char* nump;

  if( E==-1 && (( fabs(value)>1e-15 && fabs(value)<1e+15 ) || p==0) ) {
    if( floatdecimalstotal==0 )
      num.printf( "%0*.0f", fixeddecimals, value );
    else {
      num.printf( "%0*.*f", fixeddecimals+floatdecimalstotal+1, floatdecimalstotal, value );
      numlen = num.Length();
      nump = num.c_str() + numlen - 1;
      i = 0;
      while( i<(floatdecimalstotal-floatdecimals) && *nump=='0' ) {
        i++;
        nump--;
      }
      if( *nump=='.' ) {
        i++;
        nump--;
      }
      num.Delete( numlen-i+1, i );
    }
  }
  else {
    int exponent;
    double man;

    if( p==0 ) {
      exponent = 0;
      man = 0.0;
    }
    else {
      exponent=(int)(floor(log10(value)));
      if( exponent<=fixeddecimals && exponent>=0 ) {
        man = value;
        exponent = 0;
      }
      else {
        man = value * pow( 10.0, -exponent+fixeddecimalstotal-1 );
        exponent -= fixeddecimalstotal-1;

        // sometimes the mantisa equals 9.99999999999995 or something similar
        // let's divide it by 10 and increase the exponent...
        if( fabs( man-10.0 ) <= 1e-15 ) {
          man = 1;
          exponent++;
        }
      }
    }

    if( floatdecimalstotal==0 )
      num.printf( "%0*.0f", fixeddecimals, man );
    else {
      num.printf( "%0*.*f", fixeddecimals+floatdecimalstotal+1, floatdecimalstotal, man );
      numlen = num.Length();
      nump = num.c_str() + numlen - 1;
      i = 0;
      while( i<(floatdecimalstotal-floatdecimals) && *nump=='0' ) {
        i++;
        nump--;
      }
      if( *nump=='.' ) {
        i++;
        nump--;
      }
      num.Delete( numlen-i+1, i );
    }
    if( fmtStart[E+1]=='+' || exponent<0 )
      exp.printf( "%+0*d", Edecimals+1, exponent );
    else
      exp.printf( "%0*d", Edecimals, exponent );
    if( E==-1 )
      num += "e" + exp;
  }

  if( fixeddecimals == 0 ) {
    if( num[1] == '0' )
      num.Delete( 1, 1 );
  }

  // ------------------------------------------------------------------------

  const char* numStart = num.c_str();
  const char* numEnd = numStart + num.Length();
  TString     fix, rest;
  char*       out;

  fix.SetLength( fmtlen + 40 );
  rest.SetLength( fmtlen + 40 );

  numlen = num.Length();
  if( decimalPoint!=-1 ) {
    fmt = fmtStart + decimalPoint - 1;
    numpos = num.Pos(".");
    if( fixeddecimalstotal==0 ) {
      nump = numStart;
      nump--;
    }
    else {
      if( numpos==0 )
        nump = numStart + num.Length() - 1;
      else
        nump = numStart + numpos - 2;
    }
  }
  else {
    if( E!=-1 ) {
      fmt = fmtStart+E-1;
      nump = numStart + numlen - 1;
    }
    else {
      fmt = fmtEnd-1;
      if( fixeddecimals==0 ) {
        nump = numStart;
        nump--;
      }
      else
        nump = numEnd-1;
    }
  }

  if( !fmtNumStart )
    fmtNumStart = fmtStart;
  out = fix.c_str() + fix.Length();
  *out = 0;
  out--;
  dq = false;
  sq = false;

  int  tscounter = 0;
  bool minusprintedout = false;

  while( fmt>=fmtStart ) {

    if( fmt<fmtNumStart ) {
        while( nump>=numStart && *nump>='0' && *nump<='9' ) {
          if( thousandseparator && tscounter%3==0 && tscounter>0 ) {
            *out = ','; /* TODO : DefThousandSeparator */
            out--;
          }
          tscounter++;
          *out = *nump;
          out--;
          nump--;
        }
    }

    switch( *fmt ) {
    case '\'':
         if( !dq ) sq = !sq;
         fmt--;
         break;
    case '\"':
         if( !sq ) dq = !dq;
         fmt--;
         break;
    default:
         if( sq || dq ) {
           *out = *fmt;
           out--;
           fmt--;
         }
         else {
           switch( *fmt ) {
           case ',':
                fmt--;
                break;
           case '0':
           case '?':
           case '#':
                if( nump>=numStart && *nump>='0' && *nump<='9' ) {
                  if( thousandseparator && tscounter%3==0 && tscounter>0 ) {
                    *out = ','; /* TODO : DefThousandSeparator */
                    out--;
                  }
                  tscounter++;
                  *out = *nump;
                  out--;
                  nump--;
                }
                else {
                  if( *fmt=='0' ) {
                    *out = '0';
                    out--;
                  } else if( *fmt=='?' ) {
                    *out = ' ';
                    out--;
                  }
                }
                fmt--;
                break;
           case '-':
                minusprintedout = true;
                *out = *fmt;
                out--;
                fmt--;
                break;
           default:
                *out = *fmt;
                out--;
                fmt--;
           } // switch
         } // else
    }
  }

  while( nump>=numStart ) {
    if( thousandseparator && tscounter%3==0 && tscounter>0 ) {
      *out = ','; /* TODO : DefThousandSeparator */
      out--;
    }
    tscounter++;
    *out = *nump;
    out--;
    nump--;
  }

  if( valuenegative && !minusprintedout && (fixeddecimals+floatdecimalstotal)>0 ) {
    *out = '-';
    out--;
  }

  ret = TString( out+1 );
  out = rest.c_str();

  if( decimalPoint!=-1 || E!=-1 ) {
    if( decimalPoint!=-1 )
      fmt = fmtStart + decimalPoint;
    else
      fmt = fmtStart + E;
    if( numpos==0 )
      nump = numEnd;
    else
      nump = numStart + numpos - 1;

    const char *fmtfloatEnd;

    if( E==-1 )
      fmtfloatEnd = fmtEnd;
    else
      fmtfloatEnd = fmtStart + E - 1;

    bool firstEOut = true;
    dq = false;
    sq = false;
    while( fmt<fmtEnd ) {
      switch( *fmt ) {
      case '\'':
           if( !dq ) sq = !sq;
           fmt++;
           break;
      case '\"':
           if( !sq ) dq = !dq;
           fmt++;
           break;
      default:
           if( sq || dq ) {
             *out = *fmt;
             out++;
             fmt++;
           }
           else {
             switch( *fmt ) {
             case 'e':
             case 'E':
                  *out = *fmt;
                  fmt++;
                  out++;
                  if( firstEOut && (*fmt=='+' || *fmt=='-') ) {
                    firstEOut = false;
                    nump = exp.c_str();
                    if( *nump=='+' || *nump=='-' ) {
                      *out = *nump;
                      out++;
                      nump++;
                    }
                    fmt++;
                  }
                  break;
             case '0':
             case '?':
             case '#':
                  if( *nump>='0' && *nump<='9' ) {
                      *out = *nump;
                      out++;
                      nump++;
                  }
                  else {
                    if( *fmt=='0' ) {
                      *out = '0';
                      out++;
                    } else if( *fmt=='?' ) {
                      *out = ' ';
                      out++;
                    }
                  }
                  fmt++;
                  break;
             case '.':
                  if( *nump!=0 ) {
                    if( *nump == '.' )
                      *nump++;
                    *out = '.';
                    out++;
                  }
                  fmt++;
                  break;

             default:
                  *out = *fmt;
                  out++;
                  fmt++;
             } // switch
           } // else
      } // switch
    } // while

    while( *nump!=0 ) {
      *out = *nump;
      out++;
      nump++;
    }
  }

  *out = 0;
  ret += TString( rest.c_str() );

  return ret;
}

TString NumberToString( double anumber, char athousandsep,
                       bool aalwayssign, int aminintegerplaces, int aprecision )
{
  if( aprecision>=0 )
    anumber = Round( anumber, aprecision );

  bool negative = false;
  if( CompareDoubles( anumber, 0.0 )==-1 )
  {
    anumber = -anumber;
    negative = true;
  }
  bool sign = aalwayssign || negative;

  double integer, fraction;
  fraction = modf( anumber, &integer );

  int i;
  char buff[1000];
  buff[1000] = 0;
  char *pb = &(buff[999]);
  char *endbuf = pb;
  i=0;
  do
  {
    if( i%3==0 && i>0 && athousandsep!=0 )
    {
      *pb = athousandsep;
      pb--;
    }
    int dec = (int) fmod( integer, 10.0 );
    integer = floor( integer/10.0 );
    *pb = '0'+dec;
    pb--;
    i++;
  }  while( integer>=0.1 );
  if( sign )
  {
    if( negative )
      *pb = '-';
    else
      *pb = '+';
  }

  TString ints1( pb, endbuf - pb );
  TString ints2;
  if( aprecision!=0 )
  {
    pb = buff;
    do
    {
      fraction = modf( fraction*10, &integer );
      if( pb==buff )
      {
        *pb = '.';
        pb++;
      }
    } while ( false /* TODO : dodelat formatovani... */ );
  }

  return ints1; /* TODO : toto take neni dobre */
}

TString FloatToScreenString( double anumber )
{
  TString str(anumber);
  int pos = str.Pos( TString(DefDecimalPoint) );
  if( pos==0 )
    pos = str.Length()+1;

  for( pos-=3; pos>=1; pos-=3 )
  {
    if( pos!=1 || (pos==1 && str[1]!='-') )
      str.Insert( " ", pos );
  }

  return str;
}

// ----------------------------------------------------------------------------

#ifdef UTILS_USE_PCRE

TRegExp::TRegExp()
{
  options = PCRE_ANCHORED;
  ovec = NULL;
  code = NULL;
  n = 0;
}

TRegExp::TRegExp( const TString& apattern )
{
  const char *errmsg;
  int errpos;

  options = PCRE_ANCHORED;
  ovec = NULL;
  code = NULL;
  n = 0;

  SetPattern( apattern );
}

TRegExp::~TRegExp()
{
  if( code )
    free( code );

  if( ovec )
    delete ovec;
}

void TRegExp::SetLikePattern( const TString& apattern )
{
  TString pat(apattern);
  TString spec("\\^$.[]|()?*+{}-");

  for( int i=apattern.Length(); i>=1; i-- )
  {
    if( pat[i]=='_' )
      pat[i] = '.';
    else if( pat[i]=='%' )
    {
      pat.Insert( TString("."), i );
      pat[i+1] = '*';
    }
    else if( spec.Pos( pat[i] )>0 )
      pat.Insert( TString("\\"), i );
  }
  SetPattern( pat );
}

void TRegExp::SetPattern( const TString& apattern )
{
  const char *errmsg;
  int errpos;
  pcre *compiled = pcre_compile( apattern.c_str(), options, &errmsg, &errpos, NULL );
  if( !compiled )
  {
    TString errmsg;
    errmsg.sprintf( "Cannot compile regular expression. Error '%s' on position %d.\n", errmsg.c_str(), errpos );
    throw TException( errmsg );
  }
  if( code )
    delete code;
  code = compiled;

  n = pcre_info( code, NULL, NULL );
  if( ovec )
    delete ovec;
  ovec = new int[ (n+1)*3 ];
}

bool TRegExp::Match( const TString& astring )
{
  int result;

  #ifdef __BORLANDC__
  result = pcre_exec( code, NULL, astring.c_str(), astring.Length(), options, ovec, (n+1)*3 );

  // borland uses an outdated version of the pcre library that doesn't use
  // the startoffset parameter...
  //
  // int pcre_exec(const pcre *code, const pcre_extra *extra,
  //     const char *subject, int length, 
  //     int options, int *ovector, int ovecsize);
  
  #else  

  result = pcre_exec( code, NULL, astring.c_str(), astring.Length(), 0, options, ovec, (n+1)*3 );

// int pcre_exec(const pcre *code, const pcre_extra *extra,
//     const char *subject, int length, int startoffset,
//     int options, int *ovector, int ovecsize);
  
  #endif

  if( result>0 )
  {
    if( ovec[0]==0 && ovec[1]==astring.Length() )
      return true;
  }

  return false;
}

#endif

//---------------------------------------------------------------------------

} // end of namespace utils


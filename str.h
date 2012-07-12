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

#ifndef strH
#define strH

#include <stdarg.h>
#include <string.h>
#include <string>

namespace utils {

//
// TString should be used in all operations with strings across all
// libraries. TString should implement funcionality of AnsiString so
// that the code works the same regardless whether the AnsiString is
// available to the compiler (in which case it is typedefed to TString -
// we asume that the AnsiString's implementation is far better than those
// of STL string) or not.
//

#ifdef USE_VCL

#include <vcl.h>
#pragma hdrstop

typedef AnsiString TString;

#else // USE_VCL

class TString
{
private:
  char*           data;
  int             length;

  void            Realloc( unsigned int length );

public:
                  TString(): data(NULL), length(0) {}
                  TString( const TString& s );
                  TString( char c );
                  TString( char c, unsigned int len );
                  TString( const char* c );
                  TString( const char* c, unsigned int len );
                  TString( int a );
                  TString( unsigned int a );
                  TString( double a );
                  TString( long double a );
                  ~TString();

  TString&        operator=( const char* );
  TString&        operator=( const TString& );
//                  operator const char*() const;
//                  operator char*();
  bool            operator==( const TString& str ) const;
  bool            operator==( const char* str ) const;
  bool            operator!=( const TString& str ) const;
  bool            operator!=( const char* str ) const;
  bool            operator<( const TString& str ) const;
  bool            operator<( const char * str ) const;
  bool            operator<=( const TString& str ) const;
  bool            operator<=( const char * str ) const;
  bool            operator>( const TString& str ) const;
  bool            operator>( const char * str ) const;
  bool            operator>=( const TString& str ) const;
  bool            operator>=( const char * str ) const;

  TString         operator+( const TString& str ) const;
  TString&        operator+=( const TString& str );
  char&           operator[]( unsigned int pos );
  const char&     operator[]( unsigned int pos ) const;
  
  friend TString  operator+( const char* lhs, const TString& rhs );

  const char*     c_str() const;
  char*           c_str();

  unsigned int    Length() const                            { return length; }
  TString&        SetLength( unsigned int newlength );
  TString&        Insert( const TString& str, unsigned int index );
  TString&        Delete( unsigned int index, unsigned int count );
  TString         SubString( unsigned int index, unsigned int count ) const;
  int             Pos( const TString& str ) const;

  static TString  IntToHex( int value, int digits );
  int             ToInt() const;
  int             ToIntDef( int def ) const;
  double          ToDouble() const;

  int             printf( const char* format, ... );
  int             printf( const TString &format, ... );
  TString&        sprintf( const char* format, ... );
  TString&        sprintf( const TString &format, ... );
  int             vprintf( const char* format, va_list args );
  int             vprintf( const TString &format, va_list args );

  int             cat_printf( const char* format, ... );
  int             cat_printf( const TString &format, ... );
  TString&        cat_sprintf( const char* format, ... );
  TString&        cat_sprintf( const TString &format, ... );
  int             cat_vprintf( const char* format, va_list args );
  int             cat_vprintf( const TString &format, va_list args );

  TString         UpperCase() const;
  TString         LowerCase() const;
};

std::ostream& operator << (std::ostream& os, const TString& arg);

TString UpperCase( const TString& astr );
TString LowerCase( const TString& astr );

TString IntToHex( int value, int digits );

#endif // USE_VCL

} // end of namespace utils

#endif // strH


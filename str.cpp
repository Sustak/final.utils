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

#include "str.h"
#include "utils.h"

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

namespace utils {

#ifndef USE_VCL

TString::TString( const TString& s )
{
  data = NULL;

  const char *c = s.c_str();
  int len = s.Length();
  Realloc( len );

  if( len>0 )
    strcpy( data, c );
}

TString::TString( int a )
{
  data = NULL;

  char c[64];
  ::sprintf( c,"%d",a);

  int len = strlen(c);
  Realloc( len );

  if( len>0 )
    strcpy( data, c );
}

TString::TString( char c )
{
  data = new char[ 2 ];
  length = 1;

  data[0] = c;
  data[1] = 0;
}

TString::TString( unsigned int a )
{
  data = NULL;

  //long la = a;
  char c[64];

  //ltoa( la, c, 10 );
  ::sprintf(c,"%u",a);

  int len = strlen(c);
  Realloc( len );

  if( len>0 )
    strcpy( data, c );
}

TString::TString( double a )
{
  data = NULL;
  char c[64];
  gcvt( a, 15, c );
  int len = strlen(c);
  Realloc( len );

  if( len>0 )
    strcpy( data, c );
}

TString::TString( long double a )
{
  data = NULL;
  char c[64];
  gcvt( a, 18, c );
  int len = strlen(c);
  Realloc( len );

  if( len>0 )
    strcpy( data, c );
}

TString::TString( const char *astr )
{
  data = NULL;
  int len = strlen( astr );
  Realloc( len );

  if( len>0 )
    ::strcpy( data, astr );
}

TString::TString( const char* c, unsigned int len )
{
  data = NULL;
  if( len==0 ) {
    data = NULL;
    length = 0;
  }
  else {
    Realloc( len );
    if( c ) {
      char *copied = (char*) memccpy( data, c, 0, len );
      if( copied ) {
        copied--;
        memset( copied, ' ', len - ( copied - data ) );
      }
    }
    else
      memset( data, ' ', len );
    data[len] = 0;
  }
}

TString::TString( char c, unsigned int len )
{
  data = NULL;
  if( len==0 ) {
    data = NULL;
  }
  else {
    Realloc( len );
    memset( data, c, len );
    data[len] = 0;
  }
}

TString::~TString()
{
  if( data )
    delete[] data;
}

void TString::Realloc( unsigned int alength )
{
  if( data )
    delete[] data;

  if( alength>0 )
    data = new char[ alength+1 ];
  else
    data = NULL;

  length = alength;
  return;
}

TString& TString::Insert( const TString& str, unsigned int index )
{
  if( !data ) {
    *this = str;
    return *this;
  }
  else {
    int inslen = str.Length();
    if( inslen==0 )
      return *this;

    unsigned int curlen = Length();
    const char *insstr = str.c_str();

    char *newdata = new char[ curlen+inslen+1 ];

    if( index<1 ) {
      memcpy( newdata, insstr, inslen );
      strcpy( newdata+inslen, data );
      delete[] data;
      data = newdata;
      length = curlen+inslen;
      return *this;
    }

    if( index>curlen ) {
      memcpy( newdata, data, curlen );
      strcpy( newdata+curlen, insstr );
      delete[] data;
      data = newdata;
      length = curlen+inslen;
      return *this;
    }

    index--;
    memcpy( newdata, data, index );
    memcpy( newdata+index, insstr, inslen );
    strcpy( newdata+index+inslen, data + index );
    delete[] data;
    data = newdata;
    length = curlen+inslen;
    return *this;

  }
}

TString& TString::Delete( unsigned int index, unsigned int count )
{
  if( !data )
    return *this;

  unsigned int curlen = Length();
  if( index > curlen || count < 1 )
    return *this;

  if( index < 1 )
    index = 1;

  char *newdata = new char[ curlen - count + 1 ];

  index--;
  if( index>0 ) {
    memcpy( newdata, data, index );
  }
  strcpy( newdata+index, data+index+count );

  delete[] data;
  data = newdata;
  length = curlen - count;

  return *this;
}

TString TString::SubString( unsigned int index, unsigned int count ) const
{
  if( !data || Length()==0 )
    return TString();

  if( index<1 )
    index = 1;
  index--;

  unsigned int remlen = Length()-index;
  unsigned int reallen = remlen<count ? remlen : count;

  return TString( data+index, reallen );
}

TString& TString::SetLength( unsigned int newlength )
{
  *this = TString( data, newlength>INT_MAX ? INT_MAX: newlength );
  return *this;
}

int TString::Pos( const TString& str ) const
{
  char *r = strstr( data, str.c_str() );
  if( r )
    return (r-data)+1;
  else
    return 0;
}

TString TString::IntToHex( int value, int digits )
{
  TString str( utils::IntToHex( value, digits ) );
  return TString(str);
}

int TString::ToInt() const
{
  if( !data )
    return 0;

  int i;
  if( sscanf( data, "%d", &i ) == 1 )
    return i;

  throw EConvertError( "Failed to convert string '" + *this + "' to an integer." );
}

int TString::ToIntDef( int def ) const
{
  int i;
  try {
    i = ToInt();
  }
  catch( ... ) {
    return def;
  }
  return i;
}

double TString::ToDouble() const
{
  if( !data )
    return 0.0;

  double d;
  if( sscanf( data, "%le", &d ) == 1 )
    return d;
  if( sscanf( data, "%lf", &d ) == 1 )
    return d;
  if( sscanf( data, "%lg", &d ) == 1 )
    return d;

  throw EConvertError( "Failed to convert string '" + *this + "' to a double." );
}

int TString::vprintf( const char* format, va_list args )
{
  int len = vsnprintf( NULL, 0, format, args );
  Realloc( len );
  return vsprintf( data, format, args );
}

int TString::vprintf( const TString &format, va_list args )
{
  return vprintf( format.c_str(), args );
}

int TString::printf( const TString& format, ... )
{
  va_list ap;
  va_start( ap, format );
  int ret = vprintf( format.c_str(), ap );
  va_end( ap );
  return ret;
}

int TString::printf( const char* format, ... )
{
  va_list ap;
  va_start( ap, format );
  int ret = vprintf( format, ap );
  va_end( ap );
  return ret;
}

TString& TString::sprintf( const TString& format, ... )
{
  va_list ap;
  va_start( ap, format );
  vprintf( format.c_str(), ap );
  va_end( ap );
  return *this;
}

TString& TString::sprintf( const char* format, ... )
{
  va_list ap;
  va_start( ap, format );
  vprintf( format, ap );
  va_end( ap );
  return *this;
}

int TString::cat_vprintf( const char* format, va_list args )
{
  int currentlen = Length();
  int len = vsnprintf( NULL, 0, format, args );
  SetLength( len + currentlen );
  return vsprintf( c_str() + currentlen, format, args );
}

int TString::cat_vprintf( const TString &format, va_list args )
{
  return cat_vprintf( format.c_str(), args );
}

int TString::cat_printf( const TString& format, ... )
{
  va_list ap;
  va_start( ap, format );
  int ret = cat_vprintf( format.c_str(), ap );
  va_end( ap );
  return ret;
}

int TString::cat_printf( const char* format, ... )
{
  va_list ap;
  va_start( ap, format );
  int ret = cat_vprintf( format, ap );
  va_end( ap );
  return ret;
}

TString& TString::cat_sprintf( const TString& format, ... )
{
  va_list ap;
  va_start( ap, format );
  cat_vprintf( format.c_str(), ap );
  va_end( ap );
  return *this;
}

TString& TString::cat_sprintf( const char* format, ... )
{
  va_list ap;
  va_start( ap, format );
  cat_vprintf( format, ap );
  va_end( ap );
  return *this;
}

TString TString::UpperCase() const
{
  TString ret( *this );
  if( ret.Length()>0 ) {
    strupr( ret.c_str() );
  }
  return ret;
}

TString TString::LowerCase() const
{
  TString ret( *this );
  if( ret.Length()>0 ) {
    strlwr( ret.c_str() );
  }
  return ret;
};

TString& TString::operator=( const char* astr )
{
  unsigned int len = strlen( astr );
  Realloc( len );
  if( astr && len>0 )
    strcpy( data, astr );
  return *this;
}

TString& TString::operator=( const TString& astr )
{
  int len = astr.Length();
  Realloc( len );
  if( len>0 )
    strcpy( data, astr.c_str() );
  return *this;
}

TString TString::operator+( const TString& str ) const
{
  TString ret(*this);
  ret += str;
  return ret;
}

TString& TString::operator+=( const TString& str )
{
  int slen = str.Length();
  if( slen==0 )
    return *this;
    
  if( !data )
    *this = str;
  else {
    int len = Length();
    const char *cstr = str.c_str();
    char *newdata = new char[ len+slen+1 ];
    memcpy( newdata, data, len );
    strcpy( newdata+len, cstr );

    if( data )
      delete[] data;
    data = newdata;
    length = len+slen;
  }
  return *this;
}

TString operator+( const char* lhs, const TString& rhs )
{
  TString ret( lhs );
  ret += rhs;
  return ret;
}

inline std::ostream& operator << (std::ostream& os, const TString& arg)
{
  return os << arg.c_str();
}

char& TString::operator[]( unsigned int pos )
{
  if( !data )
    throw ERangeError();

  if( pos<1 || pos>Length() )
    throw ERangeError();

  return data[pos-1];
}

const char& TString::operator[]( unsigned int pos ) const
{
  if( !data )
    throw ERangeError();

  if( pos<1 || pos>Length() )
    throw ERangeError();

  return data[pos-1];
}

const char* TString::c_str() const
{
  if( data )
    return data;
  else
    return "";
}

char* TString::c_str()
{
  if( data )
    return data;
  else
    return "";
}


bool TString::operator==( const TString& str ) const
{
  if( Length()!=str.Length() )
    return false;

  return strcmp( c_str(), str.c_str() ) == 0;
}

bool TString::operator==( const char* str ) const
{
  if( !str )
    if( Length()==0 )
      return true;
    else
      return false;

  return strcmp( c_str(), str ) == 0;
}

bool TString::operator!=( const TString& str ) const
{
  if( Length()!=str.Length() )
    return true;

  return strcmp( c_str(), str.c_str() ) != 0;
}

bool TString::operator!=( const char* str ) const
{
  if( !str )
    if( Length()>0 )
      return true;
    else
      return false;

  return strcmp( c_str(), str ) != 0;
}

bool TString::operator<( const TString& str ) const
{
  return strcmp( c_str(), str.c_str() )<0;
}

bool TString::operator<( const char* str ) const
{
  return strcmp( c_str(), str==NULL ? "" : str ) < 0;
}

bool TString::operator<=( const TString& str ) const
{
  return strcmp( c_str(), str.c_str() ) <= 0;
}

bool TString::operator<=( const char* str ) const
{
  return strcmp( c_str(), str==NULL ? "" : str ) <= 0;
}

bool TString::operator>( const TString& str ) const
{
  return strcmp( c_str(), str.c_str() ) > 0;
}

bool TString::operator>( const char* str ) const
{
  return strcmp( c_str(), str==NULL ? "" : str ) > 0;
}

bool TString::operator>=( const TString& str ) const
{
  return strcmp( c_str(), str.c_str() ) >= 0;
}

bool TString::operator>=( const char* str ) const
{
  return strcmp( c_str(), str==NULL ? "" : str ) >= 0;
}

// --------------------------------------------------------------------------

TString UpperCase( const TString& astr )
{
  return astr.UpperCase();
}

TString LowerCase( const TString& astr )
{
  return astr.LowerCase();
}

TString IntToHex( int value, int digits )
{
  TString fmt( "%." );
  fmt += TString( digits ) + "x";

  TString ret;
  ret.printf( fmt, value );

  return ret;
}

//---------------------------------------------------------------------------

#endif // USE_VCL

} // end of namespace utils


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

#include "csv.h"
#include <stdio.h>

namespace utils {

void ReplaceSub32OrdinalsWithHexForm( TString& astring )
{
  int sl = astring.Length();
  for( int i=1; i<=sl; i++ )
  {
    if( astring[i] == '\\' )
    {
      astring.Insert( "\\", i );
      i++;
      sl++;
    }
    else
    {
      if( ((unsigned char)astring[i]) < 32 )
      {
        astring.Insert( "\\x" + utils::IntToHex((unsigned char)astring[i],2), i );
        astring.Delete( i+4,1 );
        sl += 3;
        i += 3;
      }
    }
  }
}

void ReplaceHexFormWithOrdinals( TString& astring )
{
  int sl = astring.Length();
  for( int i=1; i<=sl; i++  )
  {
    if( astring[i]=='\\' )
      if( i<sl )
        if( astring[i+1]=='\\' )
        {
          astring.Delete( i, 1 );
          sl--;
        }
        else
        {
          if( astring[i+1]!='x' || i+3>sl )
            throw TException( "String '"+astring+"' not in CSV format." );

          char c = HexToInt( astring.c_str() + (i+1), 2 );
          astring[i] = c;
          astring.Delete( i+1, 3 );
          sl -= 3;
        }
  }
}

int HexToInt( const char* ahex, int n )
{
  int ret = 0;
  for( int i=0; i<n; i++ )
  {
    ret *= 16;
    if( *ahex>='0' && *ahex<='9' )
      ret += *ahex-'0';
    else if( *ahex>='A' && *ahex<='F' )
      ret += *ahex-'A'+10;
    else
      throw TException( "Error while converting a hexadecimal number into a character." );
  }
  return ret;
}

TStringVector DecodeCSV( const TString& astring, char adelimiter )
{
  TStringVector sv;
  DecodeCSVString( sv, astring, adelimiter );
  return sv;
}

TStringVector* DecodeCSVString( const TString& astring, char adelimiter )
{
  TStringVector* strings = new TStringVector;
  DecodeCSVString( *strings, astring, adelimiter );
  return strings;
}

void DecodeCSVString( TStringVector &strings, const TString& astring, char adelimiter )
{
  TString str;
  str.SetLength( astring.Length() );

  strings.clear();

  if( astring.Length()==0 )
    return;

  enum { normal, parenthesis } state;
  state = normal;

  const char *beg = astring.c_str();
  const char *c = beg;
  char *destbeg = str.c_str();
  char *dest = destbeg;

  while( *c )
  {
    switch( state )
    {
      case normal:
        switch( *c )
        {
          case '"':
            state = parenthesis;
            c++;
            break;
          default:
            if( *c == adelimiter )
            {
              strings.push_back( TString( destbeg, dest-destbeg ) );
              dest = destbeg;
            }
            else {
              *dest = *c;
              dest++;
            }
            c++;
        }
        break;
      case parenthesis:
        if( *c == '"' )
        {
          c++;
          state = normal;
        }
        else {
          *dest = *c;
          c++;
          dest++;
        }
        break;
      default:
        strings.clear();
/* TODO : Nen� upln� jasn�, zda v p��pad� failu smazat to, co se dosud ud�lalo... */
        throw TException( "Unknown error in DecodeCSVString." );
    }
  }
  strings.push_back( TString( destbeg, dest-destbeg ) );

  unsigned int nstrings = strings.size();
  unsigned int ui;
  for( ui=0; ui<nstrings; ui++ )
    ReplaceHexFormWithOrdinals( strings[ui] );
}

TString CSVString( const TString &astring, char adelimiter )
{
  bool changed = false;
  int pos;
  TString word = astring;

  ReplaceSub32OrdinalsWithHexForm( word );
  changed = 0;
  pos = 1;
  while( pos<=word.Length() )
  {
    if( word[pos]==adelimiter )
    {
      changed = 1;
    }
    else if( word[pos]=='"' )
    {
      word.Insert( "\"", pos );
      pos++;
      changed = 1;
    }
    pos++;
  }
  if( changed )
    word = "\"" + word + "\"";

  return word;
}

void CSVString( const TStringVector &sv, TString &string, char adelimiter )
{
  TString     word;
  int        i;
  int        n = sv.size();
  int        totallength = 0;
//  int        pos = 0;
  char       *pos;

  if( sv.size()==0 ) {
    string = "\"\"";
    return;
  }

  for( unsigned int i=0; i<n; i++ )
  {
    word = CSVString( sv[i], adelimiter );
    totallength += word.Length();
  }
  if( n>0 )
    totallength += n-1;

  string.SetLength( totallength );
  pos = string.c_str();
  for( unsigned int i=0; i<n; i++ )
  {
    word = CSVString( sv[i], adelimiter );
    strcpy( pos, word.c_str() );
    pos += word.Length();
    if( i<(n-1) ) {
      *pos = adelimiter;
      pos++;
    }
  }
}

TString CSVString( const TStringVector &strings, char adelimiter )
{
  TString ret;
  CSVString( strings, ret, adelimiter );
  return ret;
}

TString CSVString( TStringVector* strings, char adelimiter )
{
  if( !strings )
    return "\"\"";

  TString ret;
  CSVString( *strings, ret, adelimiter );
  return ret;
}

/*
TString StringVectorToString( const TStringVector &sv, TString &string, char adelimiter )
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
  TString *s;

  nrecords = sv.size();
  for( i=0; i<nrecords; i++ ) {
    s = &( sv[i] );
    strcpy( c, s->c_str() );
    c += s->Length();
    if( i<(nrecords-1) ) {
      *c = DefDelimiter;
      c++;
    }
  }
}
*/

} // end of namespace utils

//---------------------------------------------------------------------------


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

//---------------------------------------------------------------------------

#include "io.h"
#include <stdio.h>
#include <string>

namespace utils {

//---------------------------------------------------------------------------

bool DoesFileExist( const TString &afilename )
{
  bool ret = false;
  FILE *f = fopen( afilename.c_str(), "r" );
  if( f ) {
    ret = true;
    fclose( f );
  }
  return ret;
}

TString ReadLine( FILE* afile )
{
  TString s;

  if( feof(afile) )
    return s;

  char c = ' ';

  while( !feof(afile) && c!='\n' )
  {
    c = fgetc( afile );
    if( c=='\n' || feof(afile) )
      break;

    if( c!='\r' )
      s += c;
  }

  return s;
}

long GetFileSize(FILE *stream)
{
   long curpos, length;

   curpos = ftell(stream);
   fseek(stream, 0L, SEEK_END);
   length = ftell(stream);
   fseek(stream, curpos, SEEK_SET);
   return length;
}

TStringVector* ReadFile( const TString& afilename )
{
  TStringVector *ret = NULL;
  FILE* f = fopen( afilename.c_str(), "r" );
  if( f==NULL )
    throw TException( "Failed to read file '" + afilename + "'" );

  try {
    int fsize = GetFileSize( f );
    ret = new TStringVector;

    if( fsize>0 ) {

      TString str, line;
      str.SetLength( fsize );
      char *s = str.c_str();
      int read = fread( s, 1, fsize, f );
      fclose( f );
      char *e = s + read;
      *e = 0;

      char *beg = s;
      while( s!=e ) {
        beg = s;
        while( *s!=0 && *s!=10 && *s!=13 )
          s++;
        int linelen = s - beg;
        line = TString( beg, linelen );
        ret->push_back( line );
        if( *s==13 ) s++;
        if( *s==10 ) s++;
      }
    }
  }
  catch( ... ) {
    fclose( f );
    throw;
  }

  return ret;
}

//---------------------------------------------------------------------------

} // end of namespace utils


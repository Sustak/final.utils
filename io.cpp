//---------------------------------------------------------------------------

// Copyright (c) 2004, 2005 by Marek Sestak, marses@volny.cz
// All rights reserved.

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


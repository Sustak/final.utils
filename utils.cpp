//---------------------------------------------------------------------------

// Copyright (c) by Marek Sestak, marses@volny.cz
// All rights reserved.

#include "utils.h"

// #include "str.cpp"
// #include "csv.cpp"
// #include "formats.cpp"
// #include "io.cpp"
// #include "maths.cpp"
// #include "datetime.cpp"
// #include "table.cpp"

#include <iostream.h>

//---------------------------------------------------------------------------

namespace utils {

char DefDelimiter = DefaultDelimiter;
char DefDecimalPoint = DefaultDecimalPoint;
char DefDateSeparator = DefaultDateSeparator;
TString DefDateFormat = DefaultDateFormat;
TString DefTimeFormat = DefaultTimeFormat;
TString DefDateTimeFormat = DefaultDateTimeFormat;
TString DefODBCDateTimeFormat = DefaultODBCDateTimeFormat;

TString DataTypes[] = { TString( STR_INT ), TString( STR_DOUBLE ), TString( STR_BOOL ),
                       TString( STR_STRING ), TString( STR_DATE ),
                       TString( STR_DATETIME ), TString( STR_TIME ) };

int NDataTypes = sizeof( DataTypes ) / sizeof( DataTypes[0] );

TString TrueString( TString(DefaultTrueString).UpperCase() );
TString FalseString( TString(DefaultFalseString).UpperCase() );

TString OKString( TString(DefaultOKString) );
TString ErrorString( TString(DefaultErrorString) );

TString EmptyString( "" );

double DoubleMaxPrec( 1e-307 );

//---------------------------------------------------------------------------

TException::TException()
{
  Message = "General exception.";
}

TException::TException( const TString &amsg )
{
  Message = amsg;
}

//---------------------------------------------------------------------------

//TTable _errors( "timestamp,message", "datetime,string", "" );

//---------------------------------------------------------------------------

void ReportError( const TString &astr )
{
  cerr << "ReportError: " << astr << endl;
//  _errors.
}

//---------------------------------------------------------------------------

} // end of namespace utils


void InitUtils()
{
}

#pragma startup InitUtils

//---------------------------------------------------------------------------


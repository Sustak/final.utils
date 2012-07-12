//------------------------------------------------------------------------------

// Copyright (c) 2003, 2004, 2005 by Marek Sestak, marses@volny.cz
// All rights reserved.

#ifndef utilsH
#define utilsH

#include <vector>
#include <list>
#include <set>
#include <map>
#include <exception>

// #define UTILS_USE_PCRE
// #define UTILS_USE_ODBC

#include "str.h"

// TString should be used in all operations with strings across all
// libraries. TString should implement funcionality of AnsiString so
// that the code works the same regardless whether the AnsiString is
// available to the compiler (in which case it is typedefed to TString -
// we asume that the AnsiString's implementation is far better than those
// of STL string) or not.

namespace utils {

// -----------------------------------------------------------------------------

#ifndef DefaultDelimiter
#define DefaultDelimiter ','
#endif

#ifndef DefaultDecimalPoint
#define DefaultDecimalPoint '.'
#endif

#ifndef DefaultDateSeparator
#define DefaultDateSeparator '.'
#endif

#ifndef DefaultDateFormat
#define DefaultDateFormat "DD.MM.YY"
#endif

#ifndef DefaultTimeFormat
#define DefaultTimeFormat "hh:mm:ss"
#endif

#ifndef DefaultDateTimeFormat
#define DefaultDateTimeFormat "DD.MM.YY hh:mm"
#endif

#ifndef DefaultODBCDateTimeFormat
#define DefaultODBCDateTimeFormat "YYYY-MM-DD hh:mm:ss"
#endif

extern char    DefDelimiter;
extern char    DefDecimalPoint;
extern char    DefDateSeparator;
extern TString DefDateFormat;
extern TString DefTimeFormat;
extern TString DefDateTimeFormat;
extern TString DefODBCDateTimeFormat;

#define TP_INT          0
#define TP_DOUBLE       1
#define TP_BOOL         2
#define TP_STRING       3
#define TP_DATE         4
#define TP_DATETIME     5
#define TP_TIME         6

#define STR_INT         "INT"
#define STR_DOUBLE      "FLOAT"
#define STR_BOOL        "BOOL"
#define STR_STRING      "STRING"
#define STR_DATE        "DATE"
#define STR_DATETIME    "DATETIME"
#define STR_TIME        "TIME"

typedef enum {
        XDB_INT=TP_INT, XDB_DOUBLE=TP_DOUBLE, XDB_BOOL=TP_BOOL,
        XDB_STRING=TP_STRING,
        XDB_DATE=TP_DATE, XDB_DATETIME=TP_DATETIME, XDB_TIME=TP_TIME
} TDataType;

extern TString DataTypes[];
extern int NDataTypes;

#ifndef DefaultTrueString
#define DefaultTrueString  "True"
#endif
#ifndef DefaultFalseString
#define DefaultFalseString "False"
#endif

extern TString TrueString;
extern TString FalseString;

#ifndef DefaultOKString
#define DefaultOKString "OK"
#endif
#ifndef DefaultErrorString
#define DefaultErrorString "ERROR"
#endif

extern TString OKString;
extern TString ErrorString;

extern TString EmptyString;

extern double DoubleMaxPrec;

// -----------------------------------------------------------------------------

#define StdBufSize 1024
// StdBufSize determines the amount of memory allocated each time
// routines need to store temporary variables and the overall length
// cannot be known with certainty

typedef std::vector<int>                        TIntVector;
typedef std::list<int>                          TIntList;
typedef std::set<int>                           TIntSet;
typedef std::vector<TString>                    TStringVector;
typedef std::map<int, TString, std::less<int> > TIntStringMap;

typedef unsigned int        uint;

//------------------------------------------------------------------------------

void ReportError( const TString& aerrormessage );
void Info( const TString& astring );

//------------------------------------------------------------------------------

class TException: public std::exception
{
public:
  TString       Message;

                TException();
                TException( const TString &amsg );
                ~TException() throw() {}
  TString       GetMessage() { return Message; }
};

#ifndef USE_VCL

class EConvertError: public TException
{
public:
                EConvertError(): TException( "Conversion error." ) {}
                EConvertError( const TString& str ): TException( str ) {}
                ~EConvertError() throw() {}
};

#endif

class EOperationFailed: public TException
{
public:
                EOperationFailed(): TException( "Operation failed." ) {}
                EOperationFailed( const TString& str ): TException( str ) {}
                ~EOperationFailed() throw() {}
};

class ERangeError: public TException
{
public:
                ERangeError(): TException( "Index out of range." ) {}
                ERangeError( const TString& str ): TException( str ) {}
                ~ERangeError() throw() {}
};

class EODBCError: public TException
{
public:
                EODBCError(): TException( "General ODBC error." ) {}
                EODBCError( const TString& str ): TException( str ) {}
                ~EODBCError() throw() {}
};

//------------------------------------------------------------------------------

} // end of namespace utils

//------------------------------------------------------------------------------

#include "datetime.h"
#include "csv.h"
#include "formats.h"
#include "io.h"
#include "maths.h"

//------------------------------------------------------------------------------

#endif


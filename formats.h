//---------------------------------------------------------------------------
#ifndef formatsH
#define formatsH

#ifdef UTILS_USE_PCRE
#include <pcre.h>
#endif

#include "utils.h"

namespace utils {

//---------------------------------------------------------------------------

TString GetFirstLine( TString &astring );
TString FirstLine( const TString &astring );

void StringVectorToString( const TStringVector &sv, TString &string, char adelimiter=DefDelimiter );
// StringVectorToString doesn't perform any checks on content of string vector sv!
// This function simply merges all strings contained in sv and inserts
// delimiter characted (e.g. newline char) after each line.
// Problems might arise if strings contained delimiters (for instance newline characters),
// no modifications are made and attempt to decompose string into a string vector
// wouldn't yield the identical copy of the original string vector!

TString GetNextString( TStringVector &sv );
// GetNextString takes the first from the string vector,
// removes it from the vector and returns is...

TString FloatToScreenString( double anumber );
TString FloatToTextFmt( double value, const TString& str );

TDateTime StringToDate( const TString& astr, const TString& aformat=DefDateFormat );
TDateTime StringToDateTime( const TString& astr, const TString& aformat=DefDateTimeFormat );
TDateTime StringToTime( const TString& astr, const TString& aformat=DefTimeFormat );

TString DateToString( const TDate& adate, const TString& aformat=DefDateFormat );
TString DateToString( const TDateTime& adate, const TString& aformat=DefDateFormat );
TString TimeToString( const TDateTime& adate, const TString& aformat=DefTimeFormat );
TString DateTimeToString( const TDateTime& adate, const TString& aformat=DefDateTimeFormat );
// For all the {Date|Time}ToString functions format is case insesitive.
// The following format characters are allowed:
// D ... day
// M ... month
// Y ... year
// h ... hour
// m ... minute
// s ... second
// If a formatting character is repeated more than once, the output number is
// padded with leading zeros.
// Warning: for years, only 'Y', 'YY' and 'YYYY' should be used. For YY it is
// assumed that years are between 1951 and 2050.

TString NumberToString( double anumber, char athousandsep,
                       bool aalwayssign, int aminintegerplaces, int aprecision );

#ifdef UTILS_USE_PCRE
class TRegExp
{
  int           options;
  int           *ovec;
  int           n;
  pcre          *code;
public:
                TRegExp( const TString& apattern );
                TRegExp();
                ~TRegExp();
  void          SetPattern( const TString& apattern );
  void          SetLikePattern( const TString& apattern );
  bool          Match( const TString& astring );
};
#endif

//---------------------------------------------------------------------------

} // end of namespace utils

#endif // formatsH


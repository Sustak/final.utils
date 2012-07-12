//---------------------------------------------------------------------------
#ifndef csvH
#define csvH

// Copyright (c) by Marek Sestak, marses@volny.cz
// All rights reserved.

#include "utils.h"

namespace utils {

TStringVector* DecodeCSVString( const TString& astring, char adelimiter = DefaultDelimiter );
TStringVector DecodeCSV( const TString& astring, char adelimiter = DefaultDelimiter );
void DecodeCSVString( TStringVector &output, const TString& astring, char adelimiter = DefaultDelimiter );
// Decodes a string in the comma-separated-format and separates into
// entries of TStringVector.
// First version dynamically allocates new TStringVector,
// the second one uses TStringVector provided by the caller.

TString CSVString( const TStringVector &strings, char adelimiter = DefaultDelimiter );
TString CSVString( TStringVector* strings, char adelimiter = DefaultDelimiter );
// Returns a string in the comma-separated-format. Entries are separated
// using the 'adelimiter' character, in addition, the following modifications
// are made:
// - if any entry in 'strings' contains the delimiter or a parenthesis
//   character (\"), the whole word is put into parenthesis and parenthesis is
//   doubled (i.e. replaced with \"\")
//   (e.g.   Hello, how are you.       >>>>  \"Hello, how are you.\"
//           And he said: \"Hello.\"   >>>>  \"And he said: \"\"Hello.\"\"\" )
// - characters with ordinary number lower than 32 are replaced with
//   \0xNN (where NN is a hexadecimal representation of the character's
//   ordinary number)
TString CSVString( const TString &astring, char adelimiter = DefaultDelimiter );
void CSVString( const TStringVector &sv, TString &string, char adelimiter );

int HexToInt( const char* ahex, int n );

} // end of namespace utils

//---------------------------------------------------------------------------
#endif

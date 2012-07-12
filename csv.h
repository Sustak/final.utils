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
#ifndef csvH
#define csvH

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

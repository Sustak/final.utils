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
#ifndef ioH
#define ioH

#include "utils.h"
#include <stdio.h>

namespace utils {

//---------------------------------------------------------------------------

long            GetFileSize(FILE *afile);
// Returns number of bytes from the current position to the end of the file.

TString         ReadLine( FILE* afile );
TStringVector*  ReadFile( const TString& afilename );

bool            DoesFileExist( const TString &afilename );

//---------------------------------------------------------------------------

} // end of namespace utils

#endif // ioH


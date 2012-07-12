//---------------------------------------------------------------------------
#ifndef ioH
#define ioH

// Copyright (c) 2004, 2005 by Marek Sestak, marses@volny.cz
// All rights reserved.

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


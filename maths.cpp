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
//
// ----------------------------------------------------------------------------

#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>

#include "maths.h"
#include "utils.h"

namespace utils {

// ----------------------------------------------------------------------------

double Round( double anum, int an )
{
  double zeros;
  double sign;
  if( CompareDoubles( anum, 0.0 )==-1 )
    sign = -1.0;
  else
    sign = 1.0;

  #ifdef __CYGWIN__
  zeros = pow( 10.0, an );
  #else
  zeros = powl( 10.0, an );
  #endif
  anum = anum * zeros * sign;

  #ifdef __CYGWIN__
  double floorval = floor(anum);
  #else
  double floorval = floorl(anum);
  #endif

  if( CompareDoubles( anum-floorval, 0.5 )==-1 )
    return sign*floorval/zeros;
  else
    return sign*(floorval+1.0)/zeros;
}

double RoundDown( double anum, int an )
{
  double zeros;
  double sign;
  if( CompareDoubles( anum, 0.0 )==-1 )
    sign = -1.0;
  else
    sign = 1.0;

  #ifdef __CYGWIN__
  zeros = pow( 10.0, an );
  #else
  zeros = powl( 10.0, an );
  #endif

  anum = anum * zeros * sign;

  #ifdef __CYGWIN__
  double floorval = floor(anum);
  #else
  double floorval = floorl(anum);
  #endif

  if( CompareDoubles((anum-floorval), 1.0)==-1 )
    return sign*floorval/zeros;
  else
    return sign*(floorval+1.0)/zeros;
}

double RoundUp( double anum, int an )
{
  double zeros;
  double sign;
  if( CompareDoubles( anum, 0.0 )==-1 )
    sign = -1.0;
  else
    sign = 1.0;

  #ifdef __CYGWIN__
  zeros = pow( 10.0, an );
  #else
  zeros = powl( 10.0, an );
  #endif

  anum = anum * zeros * sign;

  #ifdef __CYGWIN__
  double ceilval = ceil(anum);
  #else
  double ceilval = ceill(anum);
  #endif

  if( CompareDoubles((ceilval-anum), 1.0)==1 )
    return sign*(ceilval-1.0)/zeros;
  else
    return sign*ceilval/zeros;
}

int Random( int num )
{ 
  return num ? (int)(rand()%(num)) : 0;
}

int CompareDoubles( double a, double b )
{
  int exp1, exp2;
  double man1, man2;

  man1 = frexp( a, &exp1 );
  man2 = frexp( b, &exp2 );

  if( (exp1==exp2) && (fabs( man1-man2 )<DBL_EPSILON) )
    return 0;

  if( a<b )
    return -1;

  return 1;
}

#ifndef __CYGWIN__

int CompareDoubles( long double a, long double b )
{
  int exp1, exp2;
  long double man1, man2;

  man1 = frexpl( a, &exp1 );
  man2 = frexpl( b, &exp2 );

  if( (exp1==exp2) && (fabsl( man1-man2 )<LDBL_EPSILON) )
    return 0;

  if( a<b )
    return -1;

  return 1;
}

#else

int CompareDoubles( long double a, long double b ) 
{
    return CompareDoubles( (double)a, (double)b );
}

#endif

bool IsEqual( double a, double b )
{
  return CompareDoubles( a, b )==0;
}

bool IsZero( double a )
{
  return CompareDoubles( a, 0.0 )==0;
}

//---------------------------------------------------------------------------
// TMatrix implementation

TMatrix::TMatrix( int acols, int arows )
{
  data = 0;
  rows = arows;
  cols = acols;
  AllocateData();
};

TMatrix::TMatrix( const TMatrix& amatrix )
{
  data = 0;
  rows = amatrix.rows;
  cols = amatrix.cols;
  AllocateData();
  CopyCellsFromMatrix( amatrix );
}

TMatrix::~TMatrix()
{
  if( data )
    delete[] data;
}

TMatrix& TMatrix::operator=( const TMatrix& amatrix )
{
  if( cols==amatrix.cols && rows==amatrix.rows )
  {
    memcpy( data, amatrix.data, rows*cols*sizeof(double) );
  }
  else
  {
    cols = amatrix.cols;
    rows = amatrix.rows;
    AllocateData();
    CopyCellsFromMatrix( amatrix );
  }
  return *this;
}

TMatrix operator+( const TMatrix& m1, const TMatrix& m2 )
{
  if( m1.rows!=m2.rows || m1.cols!=m2.cols )
    throw TException( "Matrices' dimensions aren't equal." );
  TMatrix ret(m1);
  for( int ic=1; ic<=m1.cols; ic++ )
    for( int ir=1; ir<=m1.rows; ir++ )
      ret.Cell(ic,ir) += m2.const_Cell(ic,ir);
  return ret;
}

TMatrix operator-( const TMatrix& m1, const TMatrix& m2 )
{
  if( m1.rows!=m2.rows || m1.cols!=m2.cols )
    throw TException( "Matrices' dimensions aren't equal." );
  TMatrix ret(m1);
  for( int ic=1; ic<=m1.cols; ic++ )
    for( int ir=1; ir<=m1.rows; ir++ )
      ret.Cell(ic,ir) -= m2.const_Cell(ic,ir);
  return ret;
}

TMatrix operator*( const TMatrix& m1, const TMatrix& m2 )
{
  if( m1.cols!=m2.rows )
    throw TException( "Cannot multiply matrices, number of columns of the first matrix doesn't equal the number of rows of the second one." );
  int cr = m1.cols;
  TMatrix ret(m2.cols,m1.rows);
  for( int ic=1; ic<=ret.cols; ic++ ) {
    for( int ir=1; ir<=ret.rows; ir++ ) {
      for( int i=1; i<=cr; i++ ) {
        ret.Cell(ic,ir) += m1.const_Cell(i,ir)*m2.const_Cell(ic,i);
      }
    }
  }
  return ret;
}

void TMatrix::AllocateData()
{
  if( rows<=0 )
    throw TException( "Number of rows of a matrix has to be positive." );
  if( cols<=0 )
    throw TException( "Number of rows of a matrix has to be positive." );
  if( data )
    delete[] data;
  data = new double[ rows*cols ];
  memset( data, 0, rows*cols*sizeof(double) );
}

double& TMatrix::Cell( int acol, int arow )
{
  if( !data )
    throw TException( "Attempt to access uninitialized matrix." );
  if( arow<1 || arow>rows )
    throw TException( "Attempt to access row that doesn't exist." );
  if( acol<1 || acol>cols )
    throw TException( "Attempt to access column that doesn't exist." );
  return (data[(arow-1)*cols+(acol-1)]);
}

const double& TMatrix::const_Cell( int acol, int arow ) const
{
  if( !data )
    throw TException( "Attempt to access uninitialized matrix." );
  if( arow<1 || arow>rows )
    throw TException( "Attempt to access row that doesn't exist." );
  if( acol<1 || acol>cols )
    throw TException( "Attempt to access column that doesn't exist." );
  return (data[(arow-1)*cols+(acol-1)]);
}

void TMatrix::SetNCols( int acols )
{
  if( acols == cols )
    return;

  TMatrix newmatrix( acols, rows );
  newmatrix.CopyCellsFromMatrix( *this );
  *this = newmatrix;
}

void TMatrix::SetNRows( int arows )
{
  if( arows == rows )
    return;

  double *newdata = new double[ cols*arows ];
  if( data ) {
    if( arows<rows ) {
      memcpy( newdata, data, cols*arows*sizeof(double) );
    }
    else {
      memcpy( newdata, data, cols*rows*sizeof(double) );
      memset( &(newdata[cols*arows]), 0, cols*(arows-rows) );
    }
    delete[] data;
  }
  else {
    memset( newdata, 0, cols*arows*sizeof(double) );
  }

  data = newdata;
  rows = arows;
}

void TMatrix::CopyCellsFromMatrix( const TMatrix& amatrix )
{
  if( !data )
    throw TException( "Destination matrix has no elements." );
  if( amatrix.data == NULL )
    return;

  if( rows == amatrix.rows && cols == amatrix.cols )
    memcpy( data, amatrix.data, rows*cols*sizeof(double) );
  else
  {
    int nr = rows < amatrix.rows ? rows : amatrix.rows;
    int nc = cols < amatrix.cols ? cols : amatrix.cols;
    if( nr<1 || nc<1 )
      return;
    for( int r=1; r<=nr; r++ ) {
      for( int c=1; c<=nc; c++ ) {
        Cell( c,r ) = amatrix.const_Cell( c,r );
      }
    }
  }
}

TMatrix TMatrix::Transpose()
{
  TMatrix ret( rows, cols );
  for( int r=1; r<=rows; r++ ) {
    for( int c=1; c<=cols; c++ ) {
      ret.Cell( r, c ) = Cell( c, r );
    }
  }
  return ret;
}

void TMatrix::Clear()
{
  if( data )
    memset( data, 0, rows*cols*sizeof(double) );
}

TMatrix TMatrix::Inverse()
{
  if( rows!=cols )
    throw TException( "Inverse matrix can be returned only for matrices that has the same number of rows and columns." );
  if( !data )
    throw TException( "Matrix not defined, cannot return an inverse matrix." );

  int i;
  TMatrix m( cols*2, rows );
  m.CopyCellsFromMatrix( *this );
  for( i=1; i<=m.rows; i++ )
    m.Cell( i+m.rows, i ) = 1;

  try
  {
    m.MakeMatrixTriangular();
    m.MakeTriangularMatrixDiagonal();
  }
  catch( TException& ex )
  {
    throw TException( "Matrix cannot be made inverse because it's singular. (" +
                     ex.Message + ")" );
  }

  return m.SubMatrix( rows+1, 1, 2*rows, rows );
}

TMatrix TMatrix::SubMatrix( int leftcol, int toprow, int rightcol, int bottomrow )
{
  if( leftcol>rightcol )
    throw TException( "Cannot create submatrix, left column's index is greater than right one's." );
  if( toprow>bottomrow )
    throw TException( "Cannot create submatrix, left column's index is greater than right one's." );
  if( leftcol<1 || rightcol>cols || toprow<1 || bottomrow>rows )
    throw TException( "Cannot create submatrix, dimensions don't fit in the original matrix." );

  TMatrix m( rightcol-leftcol+1, bottomrow-toprow+1 );
  for( int ic=1; ic<=m.cols; ic++ ) {
    for( int ir=1; ir<=m.rows; ir++ ) {
      m.Cell(ic,ir) = Cell( leftcol+ic-1, toprow+ir-1 );
    }
  }
  return m;
}

void TMatrix::MakeMatrixTriangular()
{
  for( int ir=1; ir<=rows; ir++ ) {
    if( IsZero(Cell(ir,ir)) ) {
      if( ir==rows )
        throw TException( "Matrix cannot be made triangular because it's singular." );
      int nonzero = FindNonZeroCellBelow(ir+1,ir);
      if( nonzero==-1 )
        throw TException( "Matrix cannot be made triangular because it's singular." );
      SwapRows( ir, nonzero );
    }
    MakeCellsBellowDiagonalZero( ir );
  }
}

void TMatrix::MakeTriangularMatrixDiagonal()
{
  for( int ir=rows; ir>=1; ir-- ) {
    if( IsZero(Cell(ir,ir)) )
      throw TException( "Triangular matrix expected." );
    MultiplyRow( ir, (long double)1.0/(long double)Cell(ir,ir) );
    MakeCellsAboveDiagonalZero( ir );
  }
}

int TMatrix::FindNonZeroCellBelow( int acol, int arow )
{
  if( acol<1 || arow<1 || acol>cols || arow>rows )
    throw TException( "Cell coordinates out of bounds." );
  for( int ir=arow; ir<=rows; ir++ ) {
    if( !IsZero( Cell(acol,ir) ) ) {
      return ir;
    }
  }
  return -1;
}

void TMatrix::SwapRows( int arow1, int arow2 )
{
  if( arow1==arow2 )
    return;

  if( !data )
    throw TException( "Cannot swap rows, matrix not initialized." );
  if( arow1<1 || arow1>rows || arow2<1 || arow2>rows )
    throw TException( "Cannot swap rows, index out of bounds." );
  double *buffer = new double[cols];
  memcpy( buffer, &data[ (arow1-1)*cols ], cols*sizeof(double) );
  memcpy( &data[ (arow1-1)*cols ], &data[ (arow2-1)*cols ], cols*sizeof(double) );
  memcpy( &data[ (arow2-1)*cols ], buffer, cols*sizeof(double) );
  delete[] buffer;
}

void TMatrix::MakeCellsBellowDiagonalZero( int arow )
{
  if( arow<1 || arow>rows )
    throw TException( "Row's index out of bounds." );
  double pivot = Cell(arow,arow);
  if( pivot==0 )
    throw TException( "Zero found on a diagonal, cells bellow cannot be made zero." );
  for( int ir=arow+1; ir<=rows; ir++ )
    AddRowToRow( arow, ir, (long double)-Cell(arow,ir)/pivot );
}

void TMatrix::MakeCellsAboveDiagonalZero( int arow )
{
  if( arow<1 || arow>rows )
    throw TException( "Row's index out of bounds." );
  double pivot = Cell(arow,arow);
  if( pivot==0 )
    throw TException( "Zero found on a diagonal, cells bellow cannot be made zero." );
  for( int ir=arow-1; ir>=1; ir-- )
    AddRowToRow( arow, ir, (long double)-Cell(arow,ir)/pivot );
}

void TMatrix::AddRowToRow( int afrom, int ato, long double amultiple )
{
  for( int ic=1; ic<=cols; ic++ )
    Cell( ic, ato ) += (((long double)amultiple)*(long double)Cell(ic,afrom));
}

void TMatrix::MultiplyRow( int arow, long double amultiple )
{
  for( int ic=1; ic<=cols; ic++ )
    Cell( ic, arow ) = amultiple * (long double) Cell( ic, arow );
}

// ----------------------------------------------------------------------------

TRegression::TRegression( int n, int i )
{
  Y = TMatrix( 1, n );
  X = TMatrix( i, n );
}

TRegression::TRegression( const TMatrix& matrix )
{
  SetYXMatrix( matrix );
}

void TRegression::SetYXMatrix( const TMatrix& matrix )
{
  if( matrix.Cols()<2 )
  {
    throw TException( "Cannot create regression, matrix has to have at least two columns." );
  }
  needrecalc = true;
  int nr = matrix.Rows();
  int nc = matrix.Cols();
  Y = TMatrix( 1, nr );
  X = TMatrix( nc-1, nr );

  for( int ir=1; ir<=nr; ir++ ) {
    SetY( ir, matrix.const_Cell( 1, ir ) );
    for( int ic=1; ic<nc; ic++ ) {
      SetX( ir, ic, matrix.const_Cell( ic+1, ir ) );
    }
  }
}

void TRegression::SetN( int n )
{
  needrecalc = true;
  Y.SetNRows( n );
  X.SetNRows( n );
}

int TRegression::N() const
{
  return X.Rows();
}

int TRegression::I() const
{
  return X.Cols();
}

void TRegression::SetX( int n, int i, double value )
{
  needrecalc = true;
  X.Cell( i, n ) = value;
}

void TRegression::SetY( int n, double value )
{
  needrecalc = true;
  Y.Cell( 1, n ) = value;
}

double TRegression::GetX( int n, int i ) const
{
  return X.const_Cell( i, n );
}

double TRegression::GetY( int n ) const
{
  return Y.const_Cell( 1, n );
}

void TRegression::Recalc()
{
  RecalcRealX();
  RecalcB();
  needrecalc = false;
}

TMatrix TRegression::GetRealX()
{
  if( needrecalc )
    Recalc();
  return _RealX;
}

TMatrix TRegression::GetB()
{
  if( needrecalc )
    RecalcB();
  return B;
}

void TRegression::RecalcRealX()
{
  int ir, ic;
  int nr = N();
  int nc = I()+1;

  _RealX = TMatrix( nc, nr );
  for( ir=1; ir<=nr; ir++ )
    _RealX.Cell(1,ir) = 1.0;
  for( ir=1; ir<=nr; ir++ )
    for( ic=2; ic<=nc; ic++ )
      _RealX.Cell(ic,ir) = X.Cell(ic-1,ir);
}

void TRegression::RecalcB()
{
  TMatrix Xt = _RealX.Transpose();
  B = ((Xt*_RealX).Inverse())*Xt*Y;
}

double TRegression::Regress( double X )
{
  if( needrecalc )
    Recalc();
  return B.Cell(1,1) + B.Cell(1,2)*X;
}
// ----------------------------------------------------------------------------

// --------------------------------------------------------------------------

double ExponentialInterpolation( double alow, double alowvalue,
  double ahigh, double ahighvalue, double avaluedate, double abase )
{
  double r = ahigh-alow;
  double rb = avaluedate-abase;
  return pow(alowvalue,((ahigh-avaluedate)*rb)/(r*(alow-abase))) *
         pow(ahighvalue,((avaluedate-alow)*rb)/(r*(ahigh-abase)));
}

double ExponentialInterpolation( const TDate& alow, double alowvalue,
  const TDate &ahigh, double ahighvalue, const TDate &avaluedate,
  const TDate &abase )
{
  return ExponentialInterpolation( (double)alow.Serial(), alowvalue,
    (double)ahigh.Serial(), ahighvalue, avaluedate,
    (double)abase.Serial() );
}

double ExponentialInterpolation( const TDate& alow, double alowvalue,
  const TDate &ahigh, double ahighvalue, const TDate &avaluedate )
{
  return ExponentialInterpolation( (double)alow.Serial(), alowvalue,
    (double)ahigh.Serial(), ahighvalue, avaluedate, 0.0 );
}

// --------------------------------------------------------------------------

} // end of namespace utils

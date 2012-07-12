// ----------------------------------------------------------------------------
// Copyright (c) by Marek Sestak, marses@volny.cz
// All rights reserved.
//
// ----------------------------------------------------------------------------
// History:
// 01/11/04 - TMatrix implementation finished
//
// TO-DO-LIST:
/* TODO : Implementation of TMatrix::Determinant */
//
#ifndef mathsH
#define mathsH

#include "datetime.h"

#ifdef __CYGWIN__
long double floorl ( long double x );
long double expl ( long double x );
long double frexpl ( long double x, int * exp );
long double fabsl ( long double x );
#endif

namespace utils {

// ----------------------------------------------------------------------------
class TMatrix
{
protected:
  double        *data;
  int           rows, cols;
  void          AllocateData();

public:
                TMatrix() { data = 0; rows = cols = 0; }
                TMatrix( int acols, int arows );
                TMatrix( const TMatrix& amatrix );
                ~TMatrix();

  int           Cols() const { return cols; }
  int           Rows() const { return rows; }
  void          SetNCols( int acols );
  void          SetNRows( int arows );

  TMatrix&      operator=( const TMatrix& amatrix );
  friend TMatrix operator+( const TMatrix& m1, const TMatrix& m2 );
  friend TMatrix operator-( const TMatrix& m1, const TMatrix& m2 );
  friend TMatrix operator*( const TMatrix& m1, const TMatrix& m2 );

  double&       Cell( int acol, int arow );
  const double& const_Cell( int acel, int arow ) const;

  void          CopyCellsFromMatrix( const TMatrix& amatrix );
  TMatrix       Transpose();
  TMatrix       Inverse();
  TMatrix       SubMatrix( int leftcol, int toprow, int rightcol, int bottomrow );

  void          MakeMatrixTriangular();
  void          MakeTriangularMatrixDiagonal();
  void          Clear();

  void          AddRowToRow( int afrom, int ato, long double amultiple=1.0 );
  void          MultiplyRow( int arow, long double amultiple );
  void          MakeCellsBellowDiagonalZero( int arow );
  void          MakeCellsAboveDiagonalZero( int arow );
  int           FindNonZeroCellBelow( int acol, int arow );
  void          SwapRows( int arow1, int arow2 );
};

//---------------------------------------------------------------------------

class TRegression
{
protected:
  TMatrix               X, Y;
  bool                  needrecalc;
  TMatrix               B;
public:
                        TRegression( int n, int i );
                        TRegression( const TMatrix &matrix );

  void                  SetN( int n );
  int                   N() const;
  int                   I() const;
  virtual void          SetYXMatrix( const TMatrix& matrix );
  virtual void          SetX( int n, int i, double value );
  virtual void          SetY( int n, double value );
  virtual double        GetX( int n, int i ) const;
  virtual double        GetY( int n ) const;
  virtual TMatrix       GetB();

  virtual double        Regress( double X );

protected:
  virtual void          Recalc();
  virtual TMatrix       GetRealX();
  TMatrix               _RealX;
  virtual void          RecalcRealX();
  virtual void          RecalcB();
};

//---------------------------------------------------------------------------

double  Round( double anum, int an );
double  RoundDown( double anum, int an );
double  RoundUp( double anum, int an );
int     CompareDoubles( double a, double b );
int     CompareDoubles( long double a, long double b );
bool    IsEqual( double a, double b );
bool    IsZero( double x );
int     Random( int num );

//---------------------------------------------------------------------------

double        ExponentialInterpolation( double alow, double alowvalue,
                        double ahigh, double ahighvalue,
                        double avaluedate, double abase = 0.0 );
double        ExponentialInterpolation( const TDate& alow, double alowvalue,
                        const TDate &ahigh, double ahighvalue,
                        const TDate &avaluedate );
double        ExponentialInterpolation( const TDate& alow, double alowvalue,
                        const TDate &ahigh, double ahighvalue,
                        const TDate &avaluedate, const TDate &abase );
// vsechny tri funkce pracuji stejne (verze s parametry typu TDate prevadi
// vsechny datumy na doubly) a umi krome interpolace i extrapolovat
// parametry: alow, ahigh ............. kratsi a delsi bod na casove ose
//            alowvalue, ahighvalue ... hodnoty korespondujici s alow/ahigh
//            avaluedate .............. bod, ke kteremu hledame interpolovanou/
//                                      extrapolovanou hodnotu
//            abase ................... pokud neni zacatek krivky v bode 0,
//                                      je mozne specifikovat bod, od ktereho
//                                      se meri vzdalenosti

// --------------------------------------------------------------------------

} // end of namespace utils

#endif // mathsH


/*!
*  @file MatrixDense.cpp
*  @brief source of class MatrixDense
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

// C++ packages
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

// MRG packages
#include "MatrixDense.hpp"

// MRG third-party packages

________________________________________________________________________________

//! @internal default constructor
template <class T, class U>
MatrixDense<T, U>::MatrixDense ( void ) {

  // set number of rows
  m_numb_rows    = 0;
  // set number of columns
  m_numb_columns = 0;
  // set pointer to NULL
  m_coef = NULL;

}

________________________________________________________________________________

//! @internal constructor from its size
template <class T, class U>
MatrixDense<T, U>::MatrixDense (
        U numb_rows,
        U numb_columns ) {

  // set number of rows
  m_numb_rows    = numb_rows;
  // set number of columns
  m_numb_columns = numb_columns;
  // allocate elements array
  m_coef = new T[m_numb_rows * m_numb_columns];

}

________________________________________________________________________________

//! @internal default destructor
template <class T, class U>
MatrixDense<T, U>::~MatrixDense ( void ) {

  if ( m_coef != NULL ) {
    m_numb_rows = 0;
    m_numb_columns = 0;
    delete [] m_coef;
    m_coef = NULL;
  }

}

________________________________________________________________________________

//! @internal get the number of rows of the MatrixDense
template <class T, class U>
U MatrixDense<T, U>::GetNumbRows ( void ) const {

  return m_numb_rows;
}

________________________________________________________________________________

//! @internal get the number of columns of the MatrixDense
template <class T, class U>
U MatrixDense<T, U>::GetNumbColumns ( void ) const {

  return m_numb_columns;
}

________________________________________________________________________________

//! @internal get offset of the MatrixDense stored in row-major order
template <class T, class U>
U MatrixDense<T, U>::GetOffset (
        U idx,
        U idy ) const {

  return idx * m_numb_columns + idy;
}

________________________________________________________________________________

//! @internal const get pointer of coefficients
template <class T, class U>
T* MatrixDense<T, U>::GetCoef ( void ) const {

  return m_coef;
}

________________________________________________________________________________

//! @internal const get pointer of coefficients
template <class T, class U>
T* MatrixDense<T, U>::GetCoef (
        U idx ) const {

  return m_coef + idx * m_numb_columns;
}

________________________________________________________________________________

//! @internal get the allocation status of the MatrixDense
template <class T, class U>
bool MatrixDense<T, U>::Status ( void ) const {

  return ( m_coef != NULL && ( m_numb_rows > 0 && m_numb_columns > 0 ) );
}

________________________________________________________________________________

//! @internal explicit destructor
template <class T, class U>
int MatrixDense<T, U>::Allocate (
        U numb_rows,
        U numb_columns ) {

  // if already allocated, deallocate first
  this->Deallocate( );

  // set number of rows
  m_numb_rows    = numb_rows;
  // set number of columns
  m_numb_columns = numb_columns;
  // allocate elements array
  m_coef = new T[m_numb_rows * m_numb_columns];

  return 0;
}

________________________________________________________________________________

//! @internal explicit destructor
template <class T, class U>
int MatrixDense<T, U>::Deallocate ( void ) {

  if ( m_coef != NULL ) {
    m_numb_rows = 0;
    m_numb_columns = 0;
    delete [] m_coef;
    m_coef = NULL;
  }

  return 0;
}

________________________________________________________________________________

//! @internal performs the MatrixDense-vector product y := A * x
template <class T, class U>
int MatrixDense<T, U>::MatrixVectorProduct (
        Vector<T,U>& y,
        const Vector<T,U>& x ) const {

  for ( U i = 0; i < m_numb_rows; i++ ) {
    T res = T(0);
    for ( U j = 0; j < m_numb_columns; j++ ) {
      res += m_coef[this->GetOffset(i, j)] * x(j);
    }
    y(i) = res;
  }

  return 0;
}

________________________________________________________________________________

//! @internal default return error
template <class T, class U>
int MatrixDense<T, U>::MatrixMatrixProduct (
        MatrixDense<T,U>& A,
        const MatrixDense<T,U>& B ) const {

  // this: m x p, B: p x n  => A: m x n
  for ( U i = 0; i < this->GetNumbRows( ); i++ ) {
    for ( U j = 0; j < B.GetNumbColumns( ); j++ ) {
      A(i,j) = T(0);
      for ( U k = 0; k < this->GetNumbColumns( ); k++ ) {
        U i_k = this->GetOffset(i, k);
        A(i,j) = A(i,j) + m_coef[i_k] * B(k,j);
      }
    }
  }

  return 0;
}

________________________________________________________________________________

//! @internal overload operator "="
template <class T, class U>
MatrixDense<T,U>& MatrixDense<T,U>::operator= (
        const MatrixDense<T,U>& copy_m ) {

  // if current object is different with the copy object
  if( this != &copy_m ) {
    // copy elements;
    for ( U i = 0; i < m_numb_rows; i++ ) {
      for ( U j = 0; j < m_numb_columns; j++ ) {
        U i_j = this->GetOffset(i, j);
        m_coef[i_j] = copy_m.m_coef[i_j];
      }
    }
  }

  return *this;
}

________________________________________________________________________________

//! @internal overload operator "()"
template <class T, class U>
T& MatrixDense<T,U>::operator() (
        const U idx,
        const U idy ) {

  return m_coef[idx * m_numb_columns + idy];
}

________________________________________________________________________________

//! @internal overload operator "()" const
template <class T, class U>
T MatrixDense<T,U>::operator() (
        const U idx,
        const U idy ) const {

  return m_coef[idx * m_numb_columns + idy];
}

________________________________________________________________________________

//! @internal print MatrixDense on standard output
template <class T, class U>
int MatrixDense<T, U>::WriteToStdout (
        const char separator ) const {

  // -- write dimensions
  std::cout << m_numb_rows << " " << m_numb_columns << std::endl;
  // -- write elements
  for ( U i = 0; i < m_numb_rows; i++ ) {
    for ( U j = 0; j < m_numb_columns - 1; j++ ) {
      std::cout << m_coef[this->GetOffset(i, j)] << " ";
    }
    T last_coef = m_coef[this->GetOffset(i, m_numb_columns-1)];
    std::cout << last_coef << separator;
  }

  return 0;
}

________________________________________________________________________________

//! @internal read MatrixDense from a csv file
template <class T, class U>
int MatrixDense<T, U>::ReadFromFileCsv (
        const char* file_name ) {

  iomrg::printf( ".r. Reading csv file (dense): %s \n", file_name );

  // -- open file
  std::ifstream file(file_name, std::ios::in);

  // read mrg information
  std::string current_line;

  // -- read new line
  std::getline( file, current_line );
  std::stringstream ss_current_line(current_line);
  // first line
  U numb_rows;
  U numb_columns;
  ss_current_line >> numb_rows >> numb_columns;
  // update number of rows
  m_numb_rows = numb_rows;
  // update number of columns
  m_numb_columns = numb_columns;

  // -- read elements
  // allocate elements array
  m_coef = new T[m_numb_rows * m_numb_columns];

  // read elements
  for (U i = 0; i < m_numb_rows * m_numb_columns; i++) {
    file >> m_coef[i];
  }

  // -- close file
  file.close();

  return 0;
}

________________________________________________________________________________

//! @internal write MatrixDense into a csv file
template <class T, class U>
int MatrixDense<T, U>::WriteToFileCsv (
        const char* file_name,
        const char separator_column,
        const char separator_row ) const {

  iomrg::printf( ".w. Writing csv file (dense): %s \n", file_name );

  // -- open file
  std::ofstream file(file_name, std::ios::out | std::ios::trunc);

  // -- write dimensions
  file << m_numb_rows << " " << m_numb_columns << std::endl;
  // -- write elements
  for ( U i = 0; i < m_numb_rows; i++ ) {
    if ( separator_column != '\0' ) {
      for ( U j = 0; j < m_numb_columns - 1; j++ ) {
        file << m_coef[this->GetOffset(i, j)] << separator_column;
      }
    } else {
      for ( U j = 0; j < m_numb_columns - 1; j++ ) {
        file << m_coef[this->GetOffset(i, j)];
      }
    }
    T last_coef = m_coef[this->GetOffset(i, m_numb_columns-1)];
    file << last_coef << separator_row;
  }

  // -- close file
  file.close();

  return 0;
}

________________________________________________________________________________

//! @internal read an image matrix from a csv file
template <class T, class U>
int MatrixDense<T, U>::ReadImageMatrixFromFileCsv (
        const char* file_name ) {

  iomrg::printf( ".r. Reading image csv file (dense): %s \n", file_name );

  // -- open file
  std::ifstream file(file_name, std::ios::in);

  // read mrg information
  std::string current_line;

  // -- read new line
  std::getline( file, current_line );
  std::stringstream ss_current_line(current_line);
  // first line
  U numb_rows;
  U numb_columns;
  ss_current_line >> numb_rows >> numb_columns;
  // update number of rows
  m_numb_rows = numb_rows;
  // update number of columns
  m_numb_columns = numb_columns;

  // -- read elements
  // allocate elements array
  m_coef = new T[m_numb_rows * m_numb_columns];

  // read elements
  for (U i = 0; i < m_numb_rows; i++) {
    file >> current_line; //m_coef[i];
    for (U j = 0; j < m_numb_columns; j++) {
      char str[2];
      str[0] = current_line[j];
      str[1] = '\n';
      std::stringstream ss_c( str );
      ss_c >> m_coef[this->GetOffset(i,j)];
    }
  }

  // -- close file
  file.close();

  return 0;
}

________________________________________________________________________________

//! instantiate the class
INSTANTIATE_CLASS(MatrixDense)

________________________________________________________________________________

/*!
*  @file Vector.cpp
*  @brief source of class Vector
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

// basic packages
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

// project packages
#include "Vector.hpp"

// third-party packages


________________________________________________________________________________

//! @internal default constructor
template <class T, class U>
Vector<T, U>::Vector ( void ) {

  // set size
  m_size = 0;
  // set pointer to NULL
  m_coef = NULL;

}

________________________________________________________________________________

//! @internal construct a vector from its size
template <class T, class U>
Vector<T, U>::Vector (
        const U size ) {

  // set size
  m_size = size;
  // allocate elements array
  m_coef = new T[size];

}

________________________________________________________________________________

//! @internal default destructor
template <class T, class U>
Vector<T, U>::~Vector ( void ) {

  if ( m_coef != NULL ) {
    m_size = 0;
    delete [] m_coef;
    m_coef = NULL;
  }

}

________________________________________________________________________________

//! @internal get the size of the vector
template <class T, class U>
U Vector<T, U>::GetSize ( void ) const {

  return m_size;
}

________________________________________________________________________________

//! @internal get the pointer to the elements of the vector
template <class T, class U>
T* Vector<T, U>::GetCoef ( void ) const {

  return m_coef;
}

________________________________________________________________________________

//! @internal set data of the vector
template <class T, class U>
int Vector<T, U>::SetCoef (
        const T* coef ) {

  // copy elements
  for ( U i = 0; i < m_size; i++ ) {
    m_coef[i] = coef[i];
  }

  return 0;
}

________________________________________________________________________________

//! @internal get the allocation status of the vector
template <class T, class U>
bool Vector<T, U>::Status ( void ) const {

  return ( m_coef != NULL && m_size > 0 );
}

________________________________________________________________________________

//! @internal explicit allocation
template <class T, class U>
int Vector<T, U>::Allocate (
        const U size ) {

  // if already allocated, deallocate first
  this->Deallocate( );

  // set size
  m_size = size;
  // allocate elements array
  m_coef = new T[size];

  return 0;
}

________________________________________________________________________________

//! @internal explicit destructor
template <class T, class U>
int Vector<T, U>::Deallocate ( void ) {

  if ( m_coef != NULL ) {
    m_size = 0;
    delete [] m_coef;
    m_coef = NULL;
  }

  return 0;
}

________________________________________________________________________________

//! @internal overload operator "="
template <class T, class U>
Vector<T,U>& Vector<T, U>::operator= (
        const Vector<T,U>& copy_v ) {

  // if current object is different with the copy object
  if( this != &copy_v ) {
    // copy elements;
    for ( U i = 0; i < copy_v.GetSize( ); i++ ) {
      m_coef[i] = copy_v.m_coef[i];
    }
  }

  return *this;
}

________________________________________________________________________________

//! @internal overload operator "( )"
template <class T, class U>
T& Vector<T, U>::operator() (
        const U idx ) {

  return m_coef[idx];
}

________________________________________________________________________________

//! @internal overload operator "( )" const
template <class T, class U>
T Vector<T, U>::operator() (
        const U idx ) const {

  return m_coef[idx];
}

________________________________________________________________________________

//! @internal overload operator "[]"
template <class T, class U>
T& Vector<T, U>::operator[] (
        const U idx ) {

  return m_coef[idx];
}

________________________________________________________________________________

//! @internal overload operator "[]" const
template <class T, class U>
T Vector<T, U>::operator[] (
        const U idx ) const {

  return m_coef[idx];
}

________________________________________________________________________________

//! @internal print vector on standard output
template <class T, class U>
int Vector<T, U>::WriteToStdout (
        const char separator,
        const U idx_begin,
        const U idx_end ) const {

  // size of the vector
  U size = m_size;

  U _idx_begin = 0;
  U _idx_end = size - 1;
  if ( idx_end != 0 ) {
    size = idx_end - idx_begin + 1;
    _idx_begin = idx_begin;
    _idx_end = idx_end;
  }

  // -- write dimension
  std::cout << size << std::endl;
  // -- write elements
  for ( U i = _idx_begin; i < _idx_end; i++ ) {
    std::cout << m_coef[i] << separator;
  }
  std::cout << m_coef[_idx_end] << std::endl;

  return 0;
}

________________________________________________________________________________

//! @internal read vector from file
template <class T, class U>
int Vector<T, U>::ReadFromFileCsv (
        const char* file_name ) {

  iomrg::printf( ".r. Reading csv file (vector): %s \n", file_name );

  // -- open file
  std::ifstream file(file_name, std::ios::in);

  // read mrg information
  std::string current_line;

  // -- read new line
  std::getline( file, current_line );
  std::stringstream ss_current_line(current_line);
  // first line
  U size;
  ss_current_line >> size;
  // -- update size
  m_size = size;

  // -- read elements
  // allocate elements array
  m_coef = new T[m_size];
  // read elements
  for (U i = 0; i < m_size; i++) {
    file >> m_coef[i];
  }

  // -- close file
  file.close();

  return 0;
}

________________________________________________________________________________

//! @internal write vector into file
template <class T, class U>
int Vector<T, U>::WriteToFileCsv (
        const char* file_name,
        const char separator,
        const U idx_begin,
        const U idx_end ) const {

  iomrg::printf( ".w. Writing csv file (vector): %s \n", file_name );

  // -- open file
  std::ofstream file(file_name, std::ios::out | std::ios::trunc);

  // dimension of the array
  U size = m_size;

  U _idx_begin = 0;
  U _idx_end = size - 1;
  if ( idx_end != 0 ) {
    size = idx_end - idx_begin + 1;
    _idx_begin = idx_begin;
    _idx_end = idx_end;
  }

  // -- write dimension
  file << size << std::endl;

  // -- write elements
  if ( separator != '\0' ) {
    for ( U i = _idx_begin; i < _idx_end; i++ ) {
      file << m_coef[i] << separator;
    }
  } else {
    for ( U i = _idx_begin; i < _idx_end; i++ ) {
      file << m_coef[i];
    }
  }
  file << m_coef[_idx_end] << std::endl;

  // -- close file
  file.close();

  return 0;
}

________________________________________________________________________________

//! instantiate the class
INSTANTIATE_CLASS(Vector)

________________________________________________________________________________

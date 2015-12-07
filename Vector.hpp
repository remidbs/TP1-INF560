/*!
*  @file Vector.hpp
*  @brief header of class Vector
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

#ifndef GUARD_VECTOR_HPP_
#define GUARD_VECTOR_HPP_

// basic packages

// project packages
#include "dllmrg.hpp"

// third-party packages


//! @class Vector
//! @brief basic vector operations
template <class T, class U=int>
class Vector {

  protected:

    // -------------------------------------------------------------------------
    // -- vector
    // -------------------------------------------------------------------------

    //! size of the vector
    U m_size;
    //! elements of the vector
    T* m_coef;

  public:

    // -------------------------------------------------------------------------
    // -- Constructor and Destructor of the class
    // -------------------------------------------------------------------------

    //! @brief default constructor
    Vector ( void ) ;

    //! @brief construct a vector from its size
    //! @param [in] size = number of elements of the vector
    explicit Vector (
        const U size ) ;

    //! @brief default destructor
    ~Vector ( void ) ;

 public:

    // -------------------------------------------------------------------------
    // -- API of the class
    // -------------------------------------------------------------------------

    //! @brief get the size of the vector
    //! @return size of the vector
    U GetSize ( void ) const ;

    //! @brief get the pointer to the elements of the vector
    //! @return pointer to the elements of the vector
    T* GetCoef ( void ) const ;

    //! @brief set data of the vector
    //! @param [in] coef = new data
    int SetCoef (
        const T* coef ) ;

  public:

    // -------------------------------------------------------------------------
    // -- Low level utility function of the class
    // -------------------------------------------------------------------------

    //! @brief get the allocation status of the vector
    //! @return (true if allocated)
    bool Status ( void ) const ;

    //! @brief explicit vector allocation
    //! @param [in] size = number of elements of the vector
    //! @remarks allocate a vector of size size
    //! @return error code
    int Allocate (
        const U size ) ;

    //! @brief explicit vector destructor
    //! @remarks destroy the vector
    //! @return error code
    int Deallocate ( void ) ;

  public:

    // -------------------------------------------------------------------------
    // -- Operator of the class
    // -------------------------------------------------------------------------

    //! @brief overload operator "="
    //! @param [in] copy_v = the vector to be copied into *this
    //! @remarks \f$ (*this) := copy_v \f$
    //! @return (*this)
    Vector<T,U>& operator= (
        const Vector<T,U>& copy_v ) ;

    //! @brief overload operator "()"
    //! @param [in] idx = vector index
    //! @remarks \f$ 0 \le idx < m\_size \f$
    //! @return element at index idx
    T& operator() (
        const U idx ) ;

    //! @brief overload operator "()" const
    //! @param [in] idx = vector index
    //! @remarks \f$ 0 \le idx < m\_size \f$
    //! @return element at index idx
    T operator() (
        const U idx ) const ;

    //! @brief overload operator "[]"
    //! @param [in] idx = vector index
    //! @remarks \f$ 0 \le idx < m\_size \f$
    //! @return element at index idx
    T& operator[] (
        const U idx ) ;

    //! @brief overload operator "[]" const
    //! @param [in] idx = vector index
    //! @remarks \f$ 0 \le idx < m\_size \f$
    //! @return element at index idx
    T operator[] (
        const U idx ) const ;

  public:

    // -------------------------------------------------------------------------
    // -- Input/Output of the class
    // -------------------------------------------------------------------------

    //! @brief print vector on standard output
    //! @param [in] separator = format separator
    //! @param [in] idx_begin = start index to print
    //! @param [in] idx_end = end index to print
    //! @return error code
    int WriteToStdout (
        const char separator = ' ',
        const U idx_begin = 0,
        const U idx_end = 0 ) const ;

    //! @brief read vector from a csv ascii file
    //! @param [in] file_name = name of the file
    //! @return error code
    int ReadFromFileCsv (
        const char* file_name ) ;

    //! @brief write vector into a csv ascii file
    //! @param [in] file_name = name of the file
    //! @param [in] separator = format separator
    //! @return error code
    int WriteToFileCsv (
        const char* file_name,
        const char separator = ' ',
        const U idx_begin = 0,
        const U idx_end = 0 ) const ;

}; // class Vector {


#endif // #ifndef GUARD_VECTOR_HPP_

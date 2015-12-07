/*!
*  @file MatrixDense.hpp
*  @brief header of class MatrixDense
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

#ifndef GUARD_MATRIXDENSE_HPP_
#define GUARD_MATRIXDENSE_HPP_

// basic packages

// project packages
#include "dllmrg.hpp"
#include "Vector.hpp"

// third-party packages


//! @class MatrixDense
//! @brief basic matrix operations
template <class T, class U=int>
class MatrixDense {

  protected:

    // -------------------------------------------------------------------------
    // -- matrix
    // -------------------------------------------------------------------------

    //! size of the dimension one of the matrix
    U m_numb_rows;
    //! size of the dimension two of the matrix
    U m_numb_columns;
    //! elements of the matrix
    T* m_coef;

  public:

    // -------------------------------------------------------------------------
    // -- Constructor and Destructor of the class
    // -------------------------------------------------------------------------

    //! @brief default constructor
    MatrixDense ( void ) ;

    //! @brief constructor from its size
    //! @param [in] numb_rows = number of elements for the first dimension
    //! @param [in] numb_columns = number of elements for the second dimension
    MatrixDense (
        U numb_rows,
        U numb_columns ) ;

    //! @brief default destructor
    ~MatrixDense ( void ) ;

 public:

    // -------------------------------------------------------------------------
    // -- API of the class
    // -------------------------------------------------------------------------

    //! @brief get the number of rows of the matrix
    //! @return number of rows of the matrix
    U GetNumbRows ( void ) const ;

    //! @brief get the number of columns of the matrix
    //! @return number of columns of the matrix
    U GetNumbColumns ( void ) const ;

    //! @brief get offset of the matrix stored in row-major order
    //! @param [in] idx = row number
    //! @param [in] idy = column number
    //! @return offset of element (idx, idy)
    U GetOffset (
        U idx,
        U idy ) const ;

    //! @brief const get pointer of coefficients
    //! @param [in] idx = index of the idx-th line of the matrix
    //! @note if idx != -1, matrix is in 2d
    //! @return pointer to coefficients
    T* GetCoef ( void ) const ;

    //! @brief const get pointer of coefficients
    //! @param [in] idx = index of the idx-th line of the matrix
    //! @note if idx != -1, matrix is in 2d
    //! @return pointer to coefficients
    T* GetCoef (
        U idx ) const ;

  public:

    // -------------------------------------------------------------------------
    // -- Low level utility function of the class
    // -------------------------------------------------------------------------

    //! @brief get the allocation status of the matrix
    //! @return true if allocated
    bool Status ( void ) const ;

    //! @brief explicit matrix allocation
    //! @param [in] numb_rows = number of elements for the first dimension
    //! @param [in] numb_columns = number of elements for the second dimension
    //! @remarks allocate a matrix of size numb_rows x numb_columns with nnz
    //! @return error code
    int Allocate (
        U numb_rows,
        U numb_columns ) ;

    //! @brief explicit matrix destructor
    //! @remarks destroy the matrix
    int Deallocate ( void ) ;

  public:

    //! @brief perform the matrix-vector product y := A * x
    //! @param [in,out] y = output vector
    //! @param [in] x = input vector
    //! @return error code
    int MatrixVectorProduct (
        Vector<T,U>& y,
        const Vector<T,U>& x ) const ;

    //! @brief perform the matrix-matrix product A := A * B
    //! @param [in,out] A = output matrix
    //! @param [in] B = input matrix
    //! @return error code
    int MatrixMatrixProduct (
        MatrixDense<T,U>& A,
        const MatrixDense<T,U>& B ) const ;

  public:

    // -------------------------------------------------------------------------
    // -- Operator of the class
    // -------------------------------------------------------------------------

    //! @brief overload operator "="
    //! @param [in] copy_matrix = the matrix to be copied with *this
    MatrixDense& operator= (
        const MatrixDense& copy_matrix ) ;

    //! @brief overload operator "()"
    //! @param [in] idx = matrix index (fisrt dimension)
    //! @param [in] idy = matrix index (second dimension)
    //! @remarks
    //! @li \f$ 0 \le idx < m\_numb\_rows \f$
    //! @li \f$ 0 \le idy < m\_numb\_columns \f$
    //! @return m_coef[idx][idy]
    T& operator() (
        const U idx,
        const U idy ) ;

    //! @brief overload operator "()" const
    //! @param [in] idx = matrix index (fisrt dimension)
    //! @param [in] idy = matrix index (second dimension)
    //! @remarks
    //! @li \f$ 0 \le idx < m\_numb\_rows \f$
    //! @li \f$ 0 \le idy < m\_numb\_columns \f$
    //! @return m_coef[idx][idy]
    T operator() (
        const U idx,
        const U idy ) const ;


  public:

    // -------------------------------------------------------------------------
    // -- Input/Output of the class
    // -------------------------------------------------------------------------

    //! @brief print matrix on standard output
    //! @param [in] separator = format separator
    //! @return error code
    //! @param [in] separator = format separator
    //! @return error code
    int WriteToStdout (
        const char separator = '\n' ) const ;

    //! @brief read matrix from a csv file
    //! @param [in] file_name = name of the file
    //! @return error code
    int ReadFromFileCsv (
        const char* file_name ) ;

    //! @brief write matrix into a csv file
    //! @param [in] file_name = name of the file
    //! @param [in] separator = format separator
    //! @return error code
    int WriteToFileCsv (
        const char* file_name,
        const char separator_column = ' ',
        const char separator_row = '\n' ) const ;

    //! @brief read an image matrix from a csv file
    //! @param [in] file_name = name of the file
    //! @remarks image matrix generated from:
    //! size=20
    //! echo "$size $size" > lX.csv
    //! jp2a -i --chars="0123456789" --height=$size --width=$size lX.jpg >> lX.csv
    //! @return error code
    int ReadImageMatrixFromFileCsv (
        const char* file_name ) ;


} ; // class MatrixDense {


#endif // #ifdef GUARD_MATRIXDENSE_HPP_

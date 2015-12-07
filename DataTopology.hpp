/*!
*  @file DataTopology.hpp
*  @brief manage data distribution
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

#ifndef GUARD_DATATOPOLOGY_HPP_
#define GUARD_DATATOPOLOGY_HPP_

// basic packages
#include <mpi.h>

// project packages
#include "dllmrg.hpp"
#include "Vector.hpp"
#include "MatrixDense.hpp"

// third-party packages


//! @namespace DataTopology
namespace DataTopology {

// -----------------------------------------------------------------------------
// -- band distribution - topology
// -----------------------------------------------------------------------------

//! @brief index position of a given band
//! @param [in] proc_numb = processor number
//! @param [in] numb_procs = number of processors
//! @param [in] size = global size
//! @return index position of a given band
int BandIndexPos (
        int proc_numb,
        int numb_procs,
        int size ) ;

//! @brief size of a given band
//! @param [in] proc_numb = processor number
//! @param [in] numb_procs = number of processors
//! @param [in] size = global size
//! @return size of a given band
int BandSize (
        int proc_numb,
        int numb_procs,
        int size ) ;

//! @brief band list start and position
//! @param [in,out] band_list_start = band list start position
//! @param [in,out] band_list_size = band list size
//! @param [in] size = global size
//! @param [in] numb_procs = number of processors
//! @param [in] shift = shift start and size
//! @return error code
int BandTopology (
        int*& band_list_start,
        int*& band_list_size,
        int size,
        int numb_procs,
        int shift = 1 ) ;

//! @brief creation of two-dimensional grid communicator and communicators
//         for each row and each column of the grid
//! @param [in,out] mpi_comm_rows = grid rows communicator
//! @param [in,out] mpi_comm_columns = grid columns communicator
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int GridCartesianComm (
        MPI_Comm& mpi_comm_rows,
        MPI_Comm& mpi_comm_columns,
        MPI_Comm& mpi_comm ) ;

// -----------------------------------------------------------------------------
// -- Vector : BAND
// -----------------------------------------------------------------------------

//! @brief distribute vector upon processors (band)
//! @param [in,out] x_local = local vector
//! @param [in] x = global vector
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int DistributeVectorBand (
        Vector<double,int>& x_local,
        const Vector<double,int>& x,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief assemble vector upon processors (band)
//! @param [in,out] x_global = global vector
//! @param [in] x = local vector
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int AssembleVectorBand (
        Vector<double,int>& x_global,
        const Vector<double,int>& x,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief build the 'band_numb'-th band of a vector (band)
//! @param [in,out] x_local = local vector
//! @param [in] x = global vector
//! @param [in] numb_procs = number of processors
//! @param [in] proc_numb = processor number
//! @return error code
int BuildVectorBand (
        Vector<double,int>& x_local,
        const Vector<double,int>& x,
        const int numb_procs,
        const int proc_numb ) ;

// -----------------------------------------------------------------------------
// -- Matrix: BAND-ROW
// -----------------------------------------------------------------------------

//! @brief distribute matrix upon processors (band row)
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int DistributeMatrixBandRow (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief assemble matrix upon processors (band row)
//! @param [in,out] A_global = global matrix
//! @param [in] A = local matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int AssembleMatrixBandRow (
        MatrixDense<double,int>& A_global,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief build the 'band_numb'-th band (row) of a matrix
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] numb_procs = number of processors
//! @param [in] proc_numb = processor number
//! @return error code
int BuildMatrixBandRow (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        const int numb_procs,
        const int proc_numb ) ;

// -----------------------------------------------------------------------------
// -- Matrix: BAND-COLUMN
// -----------------------------------------------------------------------------

//! @brief distribute matrix upon processors (band column)
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int DistributeMatrixBandColumn (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief assemble matrix upon processors (band column)
//! @param [in,out] A_global = global matrix
//! @param [in] A = local matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm = MPI communicator
//! @return error code
int AssembleMatrixBandColumn (
        MatrixDense<double,int>& A_global,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm ) ;

//! @brief build the 'band_numb'-th band (column) of a matrix
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] numb_procs = number of processors
//! @param [in] proc_numb = processor number
int BuildMatrixBandColumn (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        const int numb_procs,
        const int proc_numb ) ;

// -----------------------------------------------------------------------------
// -- Vector : BLOCK
// -----------------------------------------------------------------------------

//! @brief distribute vector upon processors (block)
//! @param [in,out] x_local = local vector
//! @param [in] x = global vector
//! @param [in] root = root processor
//! @param [in] root = root processor
//! @param [in] mpi_comm_rows = grid rows communicator
//! @param [in] mpi_comm_columns = grid columns communicator
//! @return error code
int DistributeVectorBlock (
        Vector<double,int>& x_local,
        const Vector<double,int>& x,
        int root,
        MPI_Comm& mpi_comm_rows,
        MPI_Comm& mpi_comm_columns ) ;

//! @brief build the 'block_numb'-th band of a vector (block)
//! @param [in,out] x_local = local vector
//! @param [in] x = global vector
//! @param [in] numb_procs_i = number of processors (i-)
//! @param [in] numb_procs_j = number of processors (j-)
//! @param [in] proc_numb_i = processor number (i-)
//! @param [in] proc_numb_j = processor number (j-)
//! @return error code
int BuildVectorBlock (
        Vector<double,int>& x_local,
        const Vector<double,int>& x,
        const int numb_procs_i,
        const int numb_procs_j,
        const int proc_numb_i,
        const int proc_numb_j ) ;

// -----------------------------------------------------------------------------
// -- Matrix: BLOCK
// -----------------------------------------------------------------------------

//! @brief distribute matrix upon processors (block)
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm_rows = grid rows communicator
//! @param [in] mpi_comm_columns = grid columns communicator
//! @remarks checkerboard matrix decomposition
//! @return error code
int DistributeMatrixBlock (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm_rows,
        MPI_Comm& mpi_comm_columns ) ;

//! @brief assemble matrix upon processors (block)
//! @param [in,out] A_global = global matrix
//! @param [in] A = local matrix
//! @param [in] root = root processor
//! @param [in] mpi_comm_rows = grid rows communicator
//! @param [in] mpi_comm_columns = grid columns communicator
//! @remarks checkerboard matrix decomposition
//! @return error code
int AssembleMatrixBlock (
        MatrixDense<double,int>& A_global,
        const MatrixDense<double,int>& A,
        int root,
        MPI_Comm& mpi_comm_rows,
        MPI_Comm& mpi_comm_columns ) ;

//! @brief build the 'band_numb'-th block of a matrix
//! @param [in,out] A_local = local matrix
//! @param [in] A = global matrix
//! @param [in] numb_procs_i = number of processors (i-)
//! @param [in] numb_procs_j = number of processors (j-)
//! @param [in] proc_numb_i = processor number (i-)
//! @param [in] proc_numb_j = processor number (j-)
//! @remarks proc_numb follows row major order
//! @remarks proc_numb = proc_numb_i * numb_procs_j + proc_numb_j
//! @note number of processors must be perfect square
//! @return error code
int BuildMatrixMatrixBlock (
        MatrixDense<double,int>& A_local,
        const MatrixDense<double,int>& A,
        const int numb_procs_i,
        const int numb_procs_j,
        const int proc_numb_i,
        const int proc_numb_j ) ;

// -----------------------------------------------------------------------------
// -- Read Local
// -----------------------------------------------------------------------------

//! @brief get the filename of given processor among others
//! @param [in] file_group_name = file groupname
//! @param [in] file_group_type = file grouptype
//! @param [in] proc_numb = processor number
//! @param [in] numb_procs_i = number of processors (i-)
//! @param [in] numb_procs_j = number of processors (j-)
//! @return error code
const char* GetProcFilename (
        const char* file_group_name,
        const char* file_group_type,
        const int proc_numb,
        const int numb_procs_i,
        const int numb_procs_j = 0 ) ;

//! @brief read local vector
//! @param [in,out] x_local = local matrix
//! @param [in] file_group_name = file groupname
//! @param [in] file_group_type = file grouptype
//! @param [in] numb_procs = number of processors
//! @param [in] proc_numb = processor number
//! @return error code
int ReadLocalVectorFromFile (
        Vector<double,int>& A_local,
        const char* file_group_name,
        const char* file_group_type,
        const int numb_procs,
        const int proc_numb ) ;

//! @brief read local matrix
//! @param [in,out] A_local = local matrix
//! @param [in] file_group_name = file groupname
//! @param [in] file_group_type = file grouptype
//! @param [in] numb_procs = number of processors
//! @param [in] proc_numb = processor number
//! @param [in] opt_image_matrix = matrix csv or matrix image
//! @return error code
int ReadLocalMatrixFromFile (
        MatrixDense<double,int>& A_local,
        const char* file_group_name,
        const char* file_group_type,
        const int numb_procs,
        const int proc_numb,
        const bool opt_image_matrix = false ) ;

} // namespace DataTopology {


#endif // GUARD_DATATOPOLOGY_HPP_

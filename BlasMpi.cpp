/*!
*  @file BlasMpi.cpp
*  @internal source of class BlasMpi
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

// basic packages

// project packages
#include "BlasMpi.hpp"
#include "Vector.hpp"
#include "MatrixDense.hpp"
#include "DataTopology.hpp"

// third-party packages


//! @namespace BlasMpi
namespace BlasMpi {

    ________________________________________________________________________________

//! @internal compute matrix-vector product y:= A *x
    int MatrixVectorProductBandRow(
            Vector<double, int> &y,
            const MatrixDense<double, int> &A,
            const Vector<double, int> &x,
            MPI_Comm &mpi_comm) {
        int rank, nproc;
        MPI_Comm_rank(mpi_comm,&rank);
        MPI_Comm_size(mpi_comm,&nproc);

        Vector<double,int> x_temp(y.GetSize());

        int* recvcounts = new int[nproc];
        int* shifts = new int[nproc];
        int* ones = new int[nproc];
        int* idty = new int[nproc];
        for(int i = 0; i < nproc; i++){
            ones[i] = 1;
            idty[i] = i;
        }
        int size = x.GetSize();
        MPI_Allgatherv(&size,1,MPI_INT,recvcounts,ones,idty,MPI_INT,mpi_comm);
        for(int i = 0; i < nproc; i++){
            if(i == 0)
                shifts[i] = 0;
            else
                shifts[i] = shifts[i-1]+recvcounts[i-1];
        }



        MPI_Allgatherv(x.GetCoef(),x.GetSize(),MPI_DOUBLE,x_temp.GetCoef(),recvcounts,shifts,MPI_DOUBLE,mpi_comm);
        A.MatrixVectorProduct(y, x_temp);


        return 0;
    }

    ________________________________________________________________________________

//! @internal compute matrix-vector product y:= A *x
    int MatrixVectorProductBandColumn(
            Vector<double, int> &y_global,
            const MatrixDense<double, int> &A,
            const Vector<double, int> &x,
            int root,
            MPI_Comm &mpi_comm) {
        A.MatrixVectorProduct(y_global,x);


        return 0;
    }

    ________________________________________________________________________________

//! @internal compute matrix-vector product y:= A *x
    int MatrixVectorProductBlock(
            Vector<double, int> &y,
            const MatrixDense<double, int> &A,
            const Vector<double, int> &x,
            int root,
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns) {


        return 0;
    }

    ________________________________________________________________________________

//! @internal compute matrix-matrix product C := A * B
    int MatrixMatrixProductBlock(
            MatrixDense<double, int> &C,
            const MatrixDense<double, int> &A,
            const MatrixDense<double, int> &B,
            int root,
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns) {

        return 0;
    }

    ________________________________________________________________________________


} // namespace BlasMpi {

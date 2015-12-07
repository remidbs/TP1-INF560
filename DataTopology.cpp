/*!
*  @file DataTopology.cpp
*  @internal manage data distribution
*  @author Abal-Kassim Cheik Ahamed, Frédéric Magoulès, Sonia Toubaline
*  @date Tue Nov 24 16:16:48 CET 2015
*  @version 1.0
*  @remarks
*/

// basic packages

// project packages
#include <iostream>
#include "DataTopology.hpp"
#include "Vector.hpp"
#include "MatrixDense.hpp"

// third-party packages


//! @namespace DataTopology
namespace DataTopology {

    ________________________________________________________________________________


// -----------------------------------------------------------------------------
// -- band distribution - topology
// -----------------------------------------------------------------------------

//! @internal index position of a given band
    int BandPos(
            int proc_numb,
            int numb_procs,
            int size) {

        return (proc_numb * size) / numb_procs;
    }

    ________________________________________________________________________________

//! @internal size of a given band
    int BandSize(
            int proc_numb,
            int numb_procs,
            int size) {

        int band_size = BandPos(proc_numb + 1, numb_procs, size) -
                        BandPos(proc_numb, numb_procs, size);

        return band_size;
    }

    ________________________________________________________________________________


//! @internal band list start and position
    int BandTopology(
            int *&band_list_start,
            int *&band_list_size,
            int size,
            int numb_procs,
            int shift) {

        // -- band starts (displacement)
        band_list_start = new int[numb_procs];
        // -- band sizes (count)
        band_list_size = new int[numb_procs];

        for (int k = 0; k < numb_procs; k++) {
            // -- start position
            band_list_start[k] = BandPos(k, numb_procs, size) * shift;
            // -- size of the block
            band_list_size[k] = BandSize(k, numb_procs, size) * shift;
        }

        return 0;
    }

    ________________________________________________________________________________

//! @internal creation of two-dimensional grid communicator and communicators
//         for each row and each column of the grid
    int GridCartesianComm(
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns,
            MPI_Comm &mpi_comm) {

        // -- number of processors
        int numb_procs;
        // -- get number of processes
        MPI_Comm_size(mpi_comm, &numb_procs);

        // -- number of cartesian dimensions (integer)
        const int numb_dims = 2;

        // -- get suggestion on best work divison
        // integer array of size numb_dims specifying the number of nodes in each dimension
        int grid_dims[numb_dims];
        grid_dims[0] = 0;
        grid_dims[1] = 0;
        MPI_Dims_create(numb_procs, numb_dims, grid_dims);

        // -- create a new communicator with the layout of a cartesian grid
        MPI_Comm mpi_comm_cart;
        // logical array of size numb_dims specifying whether the grid is periodic or not in each dimension
        int periods[numb_dims];
        periods[0] = 0;
        periods[1] = 0;
        MPI_Cart_create(mpi_comm, numb_dims, grid_dims, periods, 1, &mpi_comm_cart);

        // -- process number (process proc_numb)
        int grid_proc_numb;
        // -- get current process proc_numb
        MPI_Comm_rank(mpi_comm_cart, &grid_proc_numb);

        // -- get local coordinates on grid
        // cartesian coordinates of specified process
        int grid_coords[numb_dims];
        grid_coords[0] = 0;
        grid_coords[1] = 0;
        MPI_Cart_coords(mpi_comm_cart, grid_proc_numb, numb_dims, grid_coords);

        // -- create communicators that work across entire rows or columns
        //  3 x 2 grid
        // (0,0)=0 (1,0)=3
        // (0,1)=1 (1,1)=4
        // (0,2)=2 (1,2)=5
        MPI_Comm_split(mpi_comm_cart, grid_coords[0], grid_coords[1],
                       &mpi_comm_rows);
        MPI_Comm_split(mpi_comm_cart, grid_coords[1], grid_coords[0],
                       &mpi_comm_columns);

        return 0;
    }

    ________________________________________________________________________________

// -----------------------------------------------------------------------------
// -- Vector: BAND
// -----------------------------------------------------------------------------

//! @internal distribute vector upon processors
    int DistributeVectorBand(
            Vector<double, int> &x_local,
            const Vector<double, int> &x,
            int root,
            MPI_Comm &mpi_comm) {

        int rank, nproc;
        MPI_Comm_rank(mpi_comm, &rank);
        MPI_Comm_size(mpi_comm, &nproc);

        int vector_size;
        if(rank == 0)
            vector_size = x.GetSize();
        MPI_Bcast(&vector_size,1,MPI_INT,root,mpi_comm);

        int sendcounts[nproc];
        int displs[nproc];
        for (int i = 0; i < nproc; i++) {
            sendcounts[i] = BandSize(rank, nproc, vector_size);
        }

        for(int i = 0; i < nproc; i++){
            if(i == 0)
                displs[i] = 0;
            else
                displs[i] = displs[i-1]+sendcounts[i-1];
        }


        x_local.Allocate(sendcounts[rank]);

        MPI_Scatterv(x.GetCoef(),sendcounts,displs,MPI_DOUBLE, x_local.GetCoef(), sendcounts[rank], MPI_DOUBLE, root, mpi_comm);




        return 0;
    }

    ________________________________________________________________________________

//! @internal assemble vector upon processors
    int AssembleVectorBand(
            Vector<double, int> &x_global,
            const Vector<double, int> &x,
            int root,
            MPI_Comm &mpi_comm) {
        int rank, nproc;
        MPI_Comm_rank(mpi_comm, &rank);
        MPI_Comm_size(mpi_comm, &nproc);

        int* recvcounts = new int[nproc];
        int* shifts = new int[nproc];
        int* ones = new int[nproc];
        int* idty = new int[nproc];
        for(int i = 0; i < nproc; i++){
            ones[i] = 1;
            idty[i] = i;
        }
        int size = x.GetSize();
        int total_size = 0;
        MPI_Gatherv(&size,1,MPI_INT,recvcounts,ones,idty,MPI_INT,root,mpi_comm);
        for(int i = 0; i < nproc; i++){
            if(i == 0)
                shifts[i] = 0;
            else
                shifts[i] = shifts[i-1]+recvcounts[i-1];
            total_size += recvcounts[i];
        }




        x_global.Allocate(total_size);
        MPI_Gatherv(x.GetCoef(),x.GetSize(),MPI_DOUBLE,x_global.GetCoef(),recvcounts,shifts,MPI_DOUBLE,root,mpi_comm);


        return 0;
    }

    ________________________________________________________________________________

//! @internal build the 'band_numb'-th band
    template<class T, class U>
    int BuildVectorBand(
            Vector<double, int> &x_local,
            const Vector<double, int> &x,
            const int numb_procs,
            const int proc_numb) {

        // -- size of the global vector
        int size = x.GetSize();

        // -- compute the number of rows per band
        int size_local = BandSize(proc_numb, numb_procs, size);
        // -- allocate and fill out the local vector
        x_local.Allocate(size_local);
        for (U i = 0; i < size_local; i++) {
            // local (i) to global indice (l2g_i)
            int l2g_i = BandPos(proc_numb, numb_procs, size) + i;
            x_local(i) = x(l2g_i);
        }

        return 0;
    }

    ________________________________________________________________________________


// -----------------------------------------------------------------------------
// -- Matrix: BAND-ROW
// -----------------------------------------------------------------------------

//! @internal distribute matrix upon processors (band row)
    int DistributeMatrixBandRow(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm) {

        int rank, nproc;
        MPI_Comm_rank(mpi_comm, &rank);
        MPI_Comm_size(mpi_comm, &nproc);

        int rows, cols;
        if (rank == root) {
            rows = A.GetNumbRows();
            cols = A.GetNumbColumns();
        }


        MPI_Bcast(&rows, 1, MPI_INT, root, mpi_comm);
        MPI_Bcast(&cols, 1, MPI_INT, root, mpi_comm);


        int sendcounts[nproc];
        int shifts[nproc];
        for (int i = 0; i < nproc; i++) {
            sendcounts[i] = BandSize(rank, nproc, rows) * cols;
        }

        for(int i = 0; i < nproc; i++){
            if(i == 0)
                shifts[i] = 0;
            else
                shifts[i] = shifts[i-1]+sendcounts[i-1];
        }


        A_local.Allocate(sendcounts[rank]/cols,cols);




        MPI_Scatterv(A.GetCoef(), sendcounts, shifts, MPI_DOUBLE, A_local.GetCoef(), sendcounts[rank], MPI_DOUBLE, root,
                     mpi_comm);

        return 0;
    }

    ________________________________________________________________________________

//! @internal assemble matrix upon processors (band row)
    int AssembleMatrixBandRow(
            MatrixDense<double, int> &A_global,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm) {


        return 0;
    }

    ________________________________________________________________________________

//! @internal build the 'band_numb'-th band (row) of a matrix
    int BuildMatrixBandRow(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            const int numb_procs,
            const int proc_numb) {

        return 0;
    }

    ________________________________________________________________________________


// -----------------------------------------------------------------------------
// -- Matrix: BAND-COLUMN
// -----------------------------------------------------------------------------


//! @internal distribute matrix upon processors (band column)
    int DistributeMatrixBandColumn(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm) {

        int rank, nproc;
        MPI_Comm_rank(mpi_comm, &rank);
        MPI_Comm_size(mpi_comm, &nproc);

        int rows, cols;
        if (rank == root) {
            rows = A.GetNumbRows();
            cols = A.GetNumbColumns();
        }


        MPI_Bcast(&rows, 1, MPI_INT, root, mpi_comm);
        MPI_Bcast(&cols, 1, MPI_INT, root, mpi_comm);


        int sendcounts[nproc];
        int shifts[nproc];
        for (int i = 0; i < nproc; i++) {
            sendcounts[i] = BandSize(rank, nproc, rows);
        }

        for(int i = 0; i < nproc; i++){
            if(i == 0)
                shifts[i] = 0;
            else
                shifts[i] = shifts[i-1]+sendcounts[i-1];
        }


        A_local.Allocate(rows,sendcounts[rank]);



        for(int i = 0; i < rows; i++)
            MPI_Scatterv(A.GetCoef(i), sendcounts, shifts, MPI_DOUBLE, A_local.GetCoef(i), sendcounts[rank], MPI_DOUBLE, root,
                     mpi_comm);

        return 0;
    }

    ________________________________________________________________________________

//! @internal assemble matrix upon processors (band column)
    int AssembleMatrixBandColumn(
            MatrixDense<double, int> &A_global,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm) {

        return 0;
    }

    ________________________________________________________________________________

//! @internal build the 'band_numb'-th band (column) of a matrix
    int BuildMatrixBandColumn(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            const int numb_procs,
            const int proc_numb) {

        return 0;
    }

    ________________________________________________________________________________


// -----------------------------------------------------------------------------
// -- Vector : BLOCK
// -----------------------------------------------------------------------------

//! @internal distribute vector upon processors (block)
    int DistributeVectorBlock(
            Vector<double, int> &x_local,
            const Vector<double, int> &x,
            int root,
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns) {

        return 0;
    }

    ________________________________________________________________________________

//! @internal build the 'block_numb'-th band of a vector (block)
    int BuildVectorBlock(
            Vector<double, int> &x_local,
            const Vector<double, int> &x,
            const int numb_procs_i,
            const int numb_procs_j,
            const int proc_numb_i,
            const int proc_numb_j) {

        // -- size of the global vector
        int size = x.GetSize();

        // -- compute the number of block in j-
        int size_local = BandSize(proc_numb_j, numb_procs_j, size);
        // -- allocate and fill out the local vector
        x_local.Allocate(size_local);
        for (int j = 0; j < size_local; j++) {
            // local (j) to global indice (l2g_j)
            int l2g_j = BandPos(proc_numb_j, numb_procs_j, size) + j;
            x_local(j) = x(l2g_j);
        }

        return 0;
    }

    ________________________________________________________________________________

// -----------------------------------------------------------------------------
// -- Matrix: BAND-ROW
// -----------------------------------------------------------------------------

//! @internal distribute matrix upon processors (block)
//! @remarks number of processors must be perfect square
    int DistributeMatrixBlock(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns) {



        return 0;
    }

    ________________________________________________________________________________

//! @internal assemble matrix upon processors (block)
//! @remarks number of processors must be perfect square
    int AssembleMatrixBlock(
            MatrixDense<double, int> &A_global,
            const MatrixDense<double, int> &A,
            int root,
            MPI_Comm &mpi_comm_rows,
            MPI_Comm &mpi_comm_columns) {


        return 0;
    }

    ________________________________________________________________________________

//! @internal build the 'band_numb'-th block of a matrix
//! @remarks proc_numb follows row major order
//! @remarks proc_numb = proc_numb_i * numb_procs_j + proc_numb_j
//! @note number of processors must be perfect square
    int BuildMatrixMatrixBlock(
            MatrixDense<double, int> &A_local,
            const MatrixDense<double, int> &A,
            const int numb_procs_i,
            const int numb_procs_j,
            const int proc_numb_i,
            const int proc_numb_j) {

        return 0;
    }

// -----------------------------------------------------------------------------
// -- Read Local
// -----------------------------------------------------------------------------

    ________________________________________________________________________________

//! @internal get the filename of given processor among others
    const char *GetProcFilename(
            const char *file_group_name,
            const char *file_group_type,
            const int proc_numb,
            const int numb_procs_i,
            const int numb_procs_j) {

// -- format file name
        std::string numb_procs_str = "000";

        int numb_procs = numb_procs_i;
        if (numb_procs_j != 0) {
            numb_procs = numb_procs_i * numb_procs_j;
        }

// sample: numb_procs = 9
        std::stringstream ss_numb_procs;
// sample: ss_numb_procs = "9"
        ss_numb_procs << numb_procs;

// sample: numb_procs_i = 3
        std::stringstream ss_numb_procs_i;
// sample: ss_numb_procs_i = "3"
        ss_numb_procs_i << numb_procs_i;

// sample: numb_procs_j = 3
        std::stringstream ss_numb_procs_j;
// sample: ss_numb_procs_j = "3"
        ss_numb_procs_j << numb_procs_j;

// -- format file name
// sample: proc_numb = 1
        std::stringstream ss_proc_numb;
// sample: ss_proc_numb = "1"
        ss_proc_numb << proc_numb;
// sample: l_ns = 1
        int l_ns = std::string(ss_proc_numb.str()).size();
// sample: prefix_str = "3_000001"
        std::string prefix_str = ss_numb_procs.str() + "_";

        if (numb_procs_j != 0) {
            prefix_str += ss_numb_procs_i.str() + "x" + ss_numb_procs_j.str() + "_";
        }
        prefix_str += numb_procs_str.substr(l_ns) + ss_proc_numb.str();

// output filename
        std::string output_file_name = std::string(file_group_name)
                                       + "_" + prefix_str + "."
                                       + std::string(file_group_type);

        return strdup(output_file_name.c_str());
    }

    ________________________________________________________________________________

//! @internal read local vector
    int ReadLocalVectorFromFile(
            Vector<double, int> &x_local,
            const char *file_group_name,
            const char *file_group_type,
            const int numb_procs,
            const int proc_numb) {

        // -- format file name
        const char *output_file_name = NULL;
        output_file_name = GetProcFilename(file_group_name, file_group_type,
                                           numb_procs, proc_numb);

        // -- read vector
        x_local.ReadFromFileCsv(output_file_name);

        return 0;
    }

    ________________________________________________________________________________

//! @internal read local matrix
    int ReadLocalMatrixFromFile(
            MatrixDense<double, int> &A_local,
            const char *file_group_name,
            const char *file_group_type,
            const int numb_procs,
            const int proc_numb,
            const bool opt_image_matrix) {

        // -- format file name
        const char *output_file_name = NULL;
        output_file_name = GetProcFilename(file_group_name, file_group_type,
                                           numb_procs, proc_numb);

        // -- read matrix
        if (opt_image_matrix) {
            // -- read image matrix from file csv
            A_local.ReadImageMatrixFromFileCsv(output_file_name);
        } else {
            // -- read image matrix from file csv
            A_local.ReadFromFileCsv(output_file_name);
        }

        return 0;
    }

    ________________________________________________________________________________

} // namespace DataTopology {

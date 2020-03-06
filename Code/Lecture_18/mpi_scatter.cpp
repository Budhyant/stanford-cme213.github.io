#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int num_procs, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // We assume we have a simple 3D cubic grid
    if (num_procs != 8)
    {
        if (my_rank == 0)
        {
            cerr << "Number of processes must be equal to 8." << endl;
            cerr << "usage: mpirun -np <# procs> " << endl;
        }
        MPI_Finalize();
        exit(1);
    }

    int q = static_cast<int>(cbrt(num_procs));
    assert(q == 2);

    int dims[3] = {q, q, q};    // Dimension of 3D grid of processes
    int periods[3] = {1, 1, 1}; // Whether or not our grid is periodic (wrap-around)

    // Create a Cartesian topology

    MPI_Comm comm_3d;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &comm_3d);

    int my3drank;
    MPI_Comm_rank(comm_3d, &my3drank); // new rank in Cartesian grid

    int coords[3];
    MPI_Cart_coords(comm_3d, my3drank, 3, coords); // coordinates in topology

    int keep_dims[3] = {1, 1, 0};
    MPI_Comm comm_ij;
    MPI_Cart_sub(comm_3d, keep_dims, &comm_ij);

    int data[16], data_local[4];

    for (int i = 0; i < 16; ++i)
        data[i] = i;

    MPI_Datatype blocktype;        // MPI user data type corresponding to one block in the matrix
    MPI_Datatype blk_type_resized; // Extent of data type modified; used as the extent unit by displs

    // Data type for a block of size 2 x 2 and with matrix size 4 x 4
    const int block_dim = 2;
    const int matrix_dim = 4;
    const int block_size = 4; // block_dim x block_dim
    MPI_Type_vector(block_dim, block_dim, matrix_dim, MPI_INT, &blocktype);
    // Changing the extent and making it equal to block_size
    MPI_Type_create_resized(blocktype, 0, sizeof(int) * block_dim, &blk_type_resized);
    MPI_Type_commit(&blk_type_resized);

    // Given this new datatype, we need to compute the start index of the
    // submatrix to send to each rank.
    int *displs = new int[num_procs];
    int *counts = new int[num_procs];
    for (int i = 0; i < q; i++)
        for (int j = 0; j < q; j++)
        {
            displs[i * q + j] = i * matrix_dim + j; // Index of block to send; in units of block_size
            counts[i * q + j] = 1;                  // We send only one block to each rank
        }

    // Scatterv data from (0,0,0) to (i,j,0)
    if (coords[2] == 0)
    {
        // int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
        //                  MPI_Datatype sendtype, void *recvbuf, int recvcount,
        //                  MPI_Datatype recvtype, int root, MPI_Comm comm)
        // TODO

        printf("(%1d,%1d,%1d) data = {%2d,%2d,%2d,%2d}\n", coords[0], coords[1], coords[2],
               data_local[0], data_local[1], data_local[2], data_local[3]);

        assert(data_local[0] == coords[0] * block_dim * matrix_dim + coords[1] * block_dim);
        assert(data_local[1] == data_local[0] + 1);
        assert(data_local[2] == data_local[0] + matrix_dim);
        assert(data_local[3] == data_local[2] + 1);
    }

    MPI_Finalize();

    return 0;
}

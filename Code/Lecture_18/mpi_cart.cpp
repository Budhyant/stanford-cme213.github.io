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
    int q = static_cast<int>(cbrt(num_procs));
    if (q * q * q != num_procs)
    {
        if (my_rank == 0)
        {
            cerr << "Number of processes must be a perfect cube." << endl;
            cerr << "usage: mpirun -np <# procs> " << endl;
        }
        MPI_Finalize();
        exit(1);
    }

    int dims[3] = {q, q, q}; // Dimension of 3D grid of processes
    int periods[3] = {1, 1, 1};
    // Whether or not our grid is periodic (wrap-around)
    // 1 means periodic; 0 means non-periodic

    // Create a Cartesian topology

    MPI_Comm comm_3d;

    // int MPI_Cart_create(MPI_Comm comm_old, int ndims, const int dims[],
    //                     const int periods[], int reorder, MPI_Comm * comm_cart)
    // ndims: size of dims
    // reorder = 0 means keep the original rank ordering
    // comm_cart: new communicator
    // TODO

    // int MPI_Comm_rank( MPI_Comm comm, int *rank)
    int my3drank;
    // TODO

    printf("[%1d] rank = %1d\n", my_rank, my3drank);

    assert(my_rank == my3drank);

    // MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[])
    int coords[3];
    // TODO

    printf("[%1d] coords = (%1d, %1d, %1d)\n", my_rank, coords[0], coords[1], coords[2]);

    // Coordinates of next process along the k axis
    int neighbor[3] = {coords[0], coords[1], coords[2] + 1};
    int rank_neighbor = 0;
    // int MPI_Cart_rank(MPI_Comm comm, const int coords[], int *rank)
    // TODO

    printf("[%1d] rank neighbor = %1d\n", my_rank, rank_neighbor);

    if (coords[2] != q - 1)
    {
        assert(rank_neighbor == my_rank + 1);
    }
    else
    {
        assert(rank_neighbor == my_rank - q + 1);
    }

    MPI_Finalize();

    return 0;
}

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

    int dims[3] = {q, q, q};    // Dimension of 3D grid of processes
    int periods[3] = {1, 1, 1}; // Whether or not our grid is periodic (wrap-around)

    // Create a Cartesian topology

    MPI_Comm comm_3d;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &comm_3d);

    int my3drank;
    MPI_Comm_rank(comm_3d, &my3drank); // new rank in Cartesian grid

    int coords[3];
    MPI_Cart_coords(comm_3d, my3drank, 3, coords); // coordinates in topology

    int keep_dims[3] = {1, 0, 0};
    MPI_Comm comm_i;
    MPI_Cart_sub(comm_3d, keep_dims, &comm_i);

    int data = coords[0];
    int data_result;

    int rank_root = 0;

    // int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
    //                MPI_Op op, int root, MPI_Comm comm)
    MPI_Reduce(&data, &data_result, 1, MPI_INT, MPI_SUM, 0, comm_i);

    if (coords[0] == 0)
    {
        printf("(%1d,%1d,%1d) data = %2d\n", coords[0], coords[1], coords[2], data_result);
        assert(data_result == (q * (q - 1)) / 2);
    }

    MPI_Finalize();

    return 0;
}

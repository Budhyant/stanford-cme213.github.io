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

    int data = -1;

    if (coords[0] == 0)
    {
        data = coords[1] * q + coords[2];
    }

    int keep_dims[3] = {1, 0, 0};
    // int MPI_Cart_sub(MPI_Comm comm, const int keep_dims[], MPI_Comm * newcomm)
    // keep_dims = 1 if kept in subgrid; 0 otherwise
    MPI_Comm comm_i;
    // TODO

    int rank_root = 0;

    // int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
    // Send buffer from root to all processes in comm
    // "void *buffer" is data
    // TODO

    printf("(%1d,%1d,%1d) data = %2d\n", coords[0], coords[1], coords[2], data);

    assert(data == coords[1] * q + coords[2]);

    MPI_Finalize();

    return 0;
}

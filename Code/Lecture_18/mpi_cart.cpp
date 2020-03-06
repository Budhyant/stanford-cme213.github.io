#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

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
    int periods[3] = {0, 0, 0}; // Whether or not our grid is periodic (wrap-around)

    // Create a Cartesian topology

    MPI_Comm comm_3d;

    // int MPI_Cart_create(MPI_Comm comm_old, int ndims, const int dims[],
    //                     const int periods[], int reorder, MPI_Comm * comm_cart)
    // TODO

    // int MPI_Comm_rank( MPI_Comm comm, int *rank )
    int my3drank;
    // TODO

    printf("[%1d] rank = %1d\n", my_rank, my3drank);

    // MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int coords[]
    int coords[3];
    // TODO
    // coordinates in topology

    printf("[%1d] coords = (%1d, %1d, %1d)\n", my_rank, coords[0], coords[1], coords[2]);

    MPI_Finalize();

    return 0;
}
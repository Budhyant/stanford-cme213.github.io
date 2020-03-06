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

    if (coords[1] != 0 && coords[2] == 0)
    {
        // Sending from (i,j,0) to (i,j,j)
        // i = coords[0]
        // j = coords[1]
        // k = coords[2]

        data = coords[0] * q + coords[1];

        // Coordinates of receiving process
        int recv_coords[3] = {0, 0, 0}; // TODO
        int recv_rank;
        MPI_Cart_rank(comm_3d, recv_coords, &recv_rank);

        // Send
        // int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
        // "const void *buf" is data
        // dest is recv_rank
        // TODO
    }
    else if (coords[1] == coords[2] && coords[2] != 0)
    {
        // Receiving from (i,j,0)
        // i = coords[0]
        // j = coords[1]
        // k = coords[2]

        // Coordinates of sending process
        int send_coords[3] = {0, 0, 0}; // TODO
        int send_rank;
        MPI_Cart_rank(comm_3d, send_coords, &send_rank);

        // Send
        // int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
        //              MPI_Comm comm, MPI_Status *status)
        // "const void *buf" is data
        // source is send_rank
        // You can use MPI_STATUS_IGNORE for "MPI_Status *status"
        // TODO

        printf("(%1d,%1d,%1d) data = %2d\n", coords[0], coords[1], coords[2], data);

        assert(data == coords[0] * q + coords[1]);
    }

    MPI_Finalize();

    return 0;
}

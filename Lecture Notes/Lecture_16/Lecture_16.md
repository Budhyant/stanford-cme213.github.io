class: center, middle

# CME 213, ME 339 - Winter 2020

## Eric Darve, ICME

![:width 40%](Stanford.jpg)

“Where is the ‘any’ key?”
— Homer Simpson, in response to the message, “Press any key”

---
class: center, middle

# Distributed memory computing using MPI

---
class: center, middle

Shared memory is a good model for a small number of processes.

---
class: center, middle

When dealing with a large number of processors, we need to view the memory as being distributed.

---
class: center, middle

What this means:

Processors can no longer directly read and write to another processor's memory

---
class: center, middle

Instead processors exchange messages.

Programmed by the user explicitly.

Send + Receive

---
class: center, middle

This can be done using MPI.

MPI is the standard for distributed memory computing. 

Message Passing Interface

---
class: center, middle

# Flynn’s taxonomy

SIMT: one instruction is dispatched to multiple threads. 

Warps on a GPU

---
class: center, middle

 SIMD: same instruction run by different processing units using different data

Vector processing units

![:width 30%](SIMD.svg)

---
class: center, middle

 MIMD: multiple instructions, multiple data; multiple threads running different functions

Vector processing units

![:width 30%](MIMD.svg)

---
class: center, middle

 SPMD: this is out topic for today. One program running on different processors. 
 
Processors communicate through a network by exchanging messages or data in an explicit manner.

---
class: center, middle

![:width 70%](2020-02-28-08-26-10.png)

---
class: center, middle

![:width 30%](2020-02-28-08-28-27.png)

---
class: middle

# Where can I get MPI?

- OpenMPI: [www.open-mpi.org](https://www.open-mpi.org/) (what we use for the GCP VM)
- MVAPICH: [mvapich.cse.ohio-state.edu](http://mvapich.cse.ohio-state.edu/)
- MPICH: [https://www.mpich.org/](www.mpich.org)

---
class: center, middle

# What computer can I use it with?

You can test MPI using a multicore computer.

Each process runs on its own core.

You can run this on GCP or your laptop.

---
class: center, middle

# GCP and cores

GCP gives you access to virtual machines.

virtual CPU (vCPU)

A vCPU is implemented as a single hardware hyper-thread on one of the available CPU Platforms.

---
class: center, middle

This means that when you request cores, you don’t really know what cores you get and where they are.

They could be physically on the same processor/board (the most likely), but they may be on different motherboards.

However, when querying the computer, it will look like you have a single processor with many cores and shared memory.

---
class: middle

# Compiling


Compile with:<br>
`mpic++`

Header:<br>
`mpi.h`

---
class: middle

Running is more complicated than usual.

You need to start multiple processes on multiple computers.

You need to make sure all processes are killed or terminated at the end.

`mpirun`

---
class: center, middle

![:width 90%](2020-02-28-08-55-50.png)

---
class: center, middle

# Transfer layer

[mpirun](https://www.open-mpi.org/doc/current/man1/mpirun.1.php) &mdash; Open MPI doc

Many **transport layers** (software/hardware used to exchange data) can be used

Examples: InfiniBand, Libfabric, Portals4, PSM2, shared memory, TCP sockets, ...

---
class: center, middle

**openib** is the option to use the OpenFabrics network, which is not available on GCP

---
class: middle

MCA: Modular Component Architecture; allows to specify options in `mpirun`

`ompi_info --params all all`

to get all options

- **BTL:** byte transfer layer (point-to-point byte movement))
- MTL: Matching transport layer (MPI-2 one-sided communications)
- PML: MPI point-to-point management layer (fragmenting, reassembly, top-layer protocols)

---
class: middle

Options for BTL: `tcp`, `vader`, `openib`, `sm`, `self`

`vader` and `sm` are shared memory byte transfer layers

`openib` high-performance (OpenFabrics Alliance software stack); InfiniBand and iWARP networks

`tcp` default; works on all networks

---
class: center, middle

So...

![:width 20%](2020-02-28-09-27-38.png)

---
class: middle

![:width 100%](2020-02-28-09-25-32.png)

---
class: center, middle

`-n` or `-np` is the number of processes to run

---
class: middle

```
  MPI_Init(&argc, &argv);

  int numprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Do something based on rank

  int len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(hostname, &len);

  MPI_Finalize();
```

---
class: center, middle

# Computing $\pi$ using MPI

---
class: center, middle

![:width 90%](2020-02-28-09-46-20.png)

---
class: center, middle

```
if (rank != MASTER)
{
    int tag = i;
    int rc = MPI_Send(&my_pi, 1, MPI_DOUBLE,
                    MASTER, tag, MPI_COMM_WORLD);

    if (rc != MPI_SUCCESS)
    printf("%d: Send failure on round %d\n", rank, tag);
}
else
{
    ...
}
```
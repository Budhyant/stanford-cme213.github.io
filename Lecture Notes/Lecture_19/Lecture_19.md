class: center, middle

# CME 213, ME 339 - Winter 2020

## Eric Darve, ICME

![:width 40%](Stanford.jpg)

“Make everything as simple as possible, but not simpler.”
— Albert Einstein

---
class: center, middle

# MPI process mapping

---
class: center, middle

Mapping is important for performance

---
class: center, middle

Lots of interprocess data exchange<br>&rarr; processes should be close

Lots of memory accesses<br>&rarr; processes should be far

---
class: center, middle

Example 1: 2-package quad-core Xeon

Pre-Nehalem, with 2 dual-core dies into each package

---
class: center, middle

![:width 90%](2020-03-10-11-34-46.png)

---
class: center, middle

Example 2: PPC64-based system with 32 cores (each with 2 hardware threads)

The architecture can get complicated real fast!

---
class: center, middle

![:width 40%](ppc64-full-with-smt.png)

---
class: middle

Mapping relies on two concepts:

1. Mapping: assign each to process to which hardware component
2. Binding: restrict the motio of process between hardware components

---
class: center, middle

# Binding

OS is responsible for assigning a hardware thread to each MPI process.

How do you control the placement of process threads?

`-bind-to object`

---
class: center, middle

This determines how the OS can migrate a process. 

Does the process stay with the same hardware thread or is it allowed to migrate to another thread (say on the same socket)?

---
class: center, middle

# `bind-to`

Get all options using `[mpirun -help]`

---
class: center, middle

Option | Hardware element
--- | ---
`hwthread` | bind to hardware thread
`core` | bind to core
`l1cache` | bind to process on L1 cache domain
`l2cache` | bind to process on L2 cache domain
`l3cache` | bind to process on L3 cache domain

---
class: center, middle

# Continued

Option | Hardware element
--- | ---
`socket` | bind to socket
`numa` | bind to NUMA domain
`board` | bind to motherboard

---
class: center, middle

# `map-by`

`map-by object`

Skip over object between bindings.

Options:<br>
slot, hwthread, core, L1cache, L2cache, L3cache, socket, numa, board, node

---
class: center, middle

Example usage:

`mpirun -bind-to core -map-by core -np 4 ./a.out`

---
class: center, middle

# Example output of GCP

`mpi8` instance

---
class: middle

```
$ mpirun --report-bindings --oversubscribe -mca btl ^openib -n 6 \
         --bind-to hwthread --map-by hwthread ./mpi_hello
[mpi8:03807] MCW rank 0 bound to socket 0[core 0[hwt 0]]: [B./../../..]
[mpi8:03807] MCW rank 1 bound to socket 0[core 0[hwt 1]]: [.B/../../..]
[mpi8:03807] MCW rank 2 bound to socket 0[core 1[hwt 0]]: [../B./../..]
[mpi8:03807] MCW rank 3 bound to socket 0[core 1[hwt 1]]: [../.B/../..]
[mpi8:03807] MCW rank 4 bound to socket 0[core 2[hwt 0]]: [../../B./..]
[mpi8:03807] MCW rank 5 bound to socket 0[core 2[hwt 1]]: [../../.B/..]
```

---
class: middle

```
$ mpirun --report-bindings --oversubscribe -mca btl ^openib -n 6 \
         --bind-to hwthread --map-by core ./mpi_hello
[mpi8:03830] MCW rank 0 bound to socket 0[core 0[hwt 0]]: [B./../../..]
[mpi8:03830] MCW rank 1 bound to socket 0[core 1[hwt 0]]: [../B./../..]
[mpi8:03830] MCW rank 2 bound to socket 0[core 2[hwt 0]]: [../../B./..]
[mpi8:03830] MCW rank 3 bound to socket 0[core 3[hwt 0]]: [../../../B.]
[mpi8:03830] MCW rank 4 bound to socket 0[core 0[hwt 1]]: [.B/../../..]
[mpi8:03830] MCW rank 5 bound to socket 0[core 1[hwt 1]]: [../.B/../..]
```

---
class: center, middle

# More information

[Mapping, Ranking, and Binding](https://www.open-mpi.org/doc/v4.0/man1/mpirun.1.php#sect9)

---
class: center, middle

VMs on GCP are virtual so architecture is hidden

It can be queried using:

`lstopo`

`hwloc-info`

---
class: middle

```
$ lstopo
Machine (7165MB)
  Package L#0 + L3 L#0 (55MB)
    L2 L#0 (256KB) + L1d L#0 (32KB) + L1i L#0 (32KB) + Core L#0
      PU L#0 (P#0)
      PU L#1 (P#4)
    L2 L#1 (256KB) + L1d L#1 (32KB) + L1i L#1 (32KB) + Core L#1
      PU L#2 (P#1)
      PU L#3 (P#5)
    L2 L#2 (256KB) + L1d L#2 (32KB) + L1i L#2 (32KB) + Core L#2
      PU L#4 (P#2)
      PU L#5 (P#6)
    L2 L#3 (256KB) + L1d L#3 (32KB) + L1i L#3 (32KB) + Core L#3
      PU L#6 (P#3)
      PU L#7 (P#7)
  HostBridge L#0
    PCI 1af4:1000
```

---
class: middle

```
$ hwloc-info
depth 0:	1 Machine (type #1)
 depth 1:	1 Package (type #3)
  depth 2:	1 L3Cache (type #4)
   depth 3:	4 L2Cache (type #4)
    depth 4:	4 L1dCache (type #4)
     depth 5:	4 L1iCache (type #4)
      depth 6:	4 Core (type #5)
       depth 7:	8 PU (type #6)
Special depth -3:	1 Bridge (type #9)
Special depth -4:	1 PCI Device (type #10)
```

---
class: center, middle

# More information

[Hwloc tutorial slides](https://www-lb.open-mpi.org/projects/hwloc/tutorials/20160606-PATC-hwloc-tutorial.pdf)

[Open-mpi hwloc documentation page](https://www-lb.open-mpi.org/projects/hwloc/)

[Hwloc/lstopo examples](https://www.open-mpi.org/projects/hwloc/doc/v2.1.0/a00328.php#cli_examples)

---
class: middle

# MPI communications

Two strategies:

1. Buffered: send/receive appear to complete immediately
2. Non-buffered: saves memory but requires waiting

---
class: center, middle

![:width 90%](2020-03-10-11-58-47.png)

---
class: center, middle

![:width 70%](2020-03-10-11-59-51.png)

---
class: center, middle

# Summary

---
class: middle

# `Send`

If MPI uses a separate system buffer, the data in `smessage` (user buffer space) is copied (fast); then the main thread resumes.

If MPI does not use a separate system buffer, the main thread must wait until the communication over the network is complete.

---
class: middle

# `Recv`

If communication happens before the call, the data is stored in an MPI system buffer, and then simply copied into the user provided rmessage when `recv()` is called.

The user cannot decide whether a buffer is used or not; the MPI library makes that decision based on the resources available and other factors.

---
class: center, middle

![:width 70%](2020-03-10-12-29-51.png)

---
class: center, middle

# Send/Recv deadlocks

---
class: center, middle

Very easy to achieve with Send/Recv

Send and Recv are both blocking

Process will wait until communication completes

---
class: center, middle

Process 0 | Process 1 | Deadlock
--- | --- | ---
Recv()<br>Send() | Recv()<br>Send() | .red[Always]
Send()<br>Recv() | Send()<br>Recv() | Depends on whether a buffer is used or not
Send()<br>Recv() | Recv()<br>Send() | .green[Secure]

---
class: center, middle

Let's demonstrate these implementations on a ring communication example

---
class: center, middle

![](2020-03-10-13-48-49.png)

---
class: center, middle

# Deadlock

---
class: middle

```
...
MPI_Recv(&number_recv, 1, MPI_INT, rank_sender,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
MPI_Send(&number_send, 1, MPI_INT, rank_receiver,
            0, MPI_COMM_WORLD);
...            
```      

---
class: middle

# Uncertain case

```
...
MPI_Send(&number_send, 1, MPI_INT, rank_receiver,
            0, MPI_COMM_WORLD);
MPI_Recv(&number_recv, 1, MPI_INT, rank_sender,
            0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
...            
```

---
class: center, middle

# Correct implementation

---
class: middle

```
if (rank % 2 == 0) {
    MPI_Send(&number_send, 1, MPI_INT, rank_receiver,
                0, MPI_COMM_WORLD);
} else {
    MPI_Recv(&number_recv, 1, MPI_INT, rank_sender,
                0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

if (rank % 2 == 1){
    MPI_Send(&number_send, 1, MPI_INT, rank_receiver,
                0, MPI_COMM_WORLD);
} else {
    MPI_Recv(&number_recv, 1, MPI_INT, rank_sender,
                0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
```

---
class: center, middle

# Non-blocking MPI communications

deadlock, buffering, synchronous communications, user-specified buffering, and ready send

---
class: center, middle

# MPI send modes

Send Modes | MPI function | Completion Condition
--- | --- | ---
Standard send | `MPI_Send` | Message sent (receiver state unknown)
Buffered send | `MPI_Bsend` | Always completes, irrespective of the receiver
Ready send | `MPI_Rsend` | May be used only when the a matching receive has already been posted
Synchronous send | `MPI_Ssend` | Only completes when the receive has completed
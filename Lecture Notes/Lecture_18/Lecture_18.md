class: center, middle

# CME 213, ME 339 - Winter 2020

## Eric Darve, ICME

![:width 40%](Stanford.jpg)

“When you are stuck in a traffic jam with a Porsche, all you do is burn more gas in idle. Scalability is about building wider roads, not about building faster cars.”
— Steve Swartz

---
class: center, middle

# Homework 5: matrix-matrix products

---
class: center, middle

$$c\_{ij} = \sum\_{k=1}^n a\_{ik} b\_{kj}$$

---
class: center, middle

![](2020-03-06-08-11-50.png)

???

Product is computed using block operations

---
class: center, middle

# Process $(i,j,k)$

![](2020-03-06-08-16-08.png)

---
class: center, middle

# Data distribution

Several steps are required

Assume process 0 has matrix $A$ and $B$

---
class: middle

1. Process 0 sends $a\_{ij}$ and $b\_{ij}$ to process $(i,j,0)$
2. Process $(i,j,0)$ sends $a\_{ij}$ to $(i,j,j)$, and $b\_{ij}$ to $(i,j,i)$
3. $(i,j,j)$ broadcasts $a\_{ij}$ to $(i,\*,j)$, and $(i,j,i)$ broadcasts $b\_{ij}$ to $(\*,j,i)$

Process $(i,j,k)$ computes $a\_{ik} b\_{kj}$.

---
class: middle

MPI functions that will be needed:

1. That's a scatter. `MPI_Scatterv`
2. That's a send/recv. `MPI_Send` and `MPI_Recv`
3. That's a broadcast. `MPI_Bcast`

---
class: center, middle

Scatter and broadcast are collective communications.

But only a subset of processes are involved in the communication.

---
class: middle

1. 0 &rarr; $(i,j,0)$
2. P2P
3. $(i,j,j)$ &rarr; $(i,\*,j)$, $(i,j,i)$ &rarr; $(\*,j,i)$

---
class: middle

We will need two concepts:

1. MPI topology
2. MPI groups and communicators

---
class: center, middle

# MPI topology

We need a grid topology.

Arranges all processes logically on a 3D grid $(i,j,k)$.
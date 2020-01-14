class: center, middle

# CME 213, ME 339 - Winter 2020</br>Introduction to parallel computing using MPI, openMP, and CUDA

## Eric Darve, ICME, Stanford yoy

![:width 40%](Stanford.jpg)

"Computers are useless.  They can only give you answers."
(Pablo Picasso)

---
class: middle

# Homework 1

Pre-requisite homework

Topics: 

- derived classes
- polymorphism
- standard library
- testing

---
class: middle

# Submission

1. Copy your files to cardinal
2. Run a Python script with code

Grading is done on gradescope

.center[![:width 70%](hw_rats.jpeg)]

---
class: center, middle

Deadline is **Friday January 17**

---
class: center, middle

# Why parallel computing?

![:width 30%](2020-01-08-12-58-05.png)

---
class: center, middle

Parallel computing is omni-present

Any type of non-trivial computing requires parallel computing

---

.two-column[![:width 80%](G_Moore.jpeg)]
.two-column[- Gordon Moore 1965: "the number of transistors on a chip shall double every 18–24 months."
- Accompanied by an increase in clock speed
]

---
class: center, middle

# Intel microprocessor trends

![:width 70%](micro_trend.png)

---

# But

Increase in transistor density is limited by:
- Leakage current increases
- Power consumption increases
- Heat generated increases

---
class: center, middle

Memory access time has not been reduced at a rate comparable to the processing speed

&#8595;

Go parallel!

Multiple cores on a processor

---
class: center, middle

# Multicore

.two-column[![:width 80%](Sea_Giant_MG.jpg)]
.two-column[One/few 

but 

fast core(s)
]

---
class: center, middle

# Manycore

.two-column[![:width 80%](ants.jpg)]
.two-column[ .left[- Many, but slower cores
- GPUs]
]

---
class: center, middle

# Core increase; frequency plateau

.two-column[![:width 100%](2020-01-08-08-22-25.png)
Historical data on 1403 Intel microprocessors]

.two-column[![:width 100%](2020-01-08-08-22-42.png)
Historical data on 566 NVIDIA GPUs]

---
class: center, middle

# Memory wall; bandwidth and latency

.two-column[![:width 100%](2020-01-08-08-26-38.png)
Intel microprocessors]

.two-column[![:width 100%](2020-01-08-08-27-02.png)
NVIDIA GPUs]

---
class: center, middle

More info at

.smaller[https://pure.tue.nl/ws/portalfiles/portal/3942529/771987.pdf]

---
class: center, middle

# Parallel computing everywhere!

.two-column[![:width 100%](2020-01-07-15-38-14.png)]
.two-column[![:width 100%](summit.png)]

---
class: center, middle

# Multi and many core processors

.two-column[![:width 70%](2020-01-07-15-45-25.png)

Intel Ice Lake 10 nm]
.two-column[![:width 80%](2020-01-07-15-48-28.png)
NVIDIA Turing TU102 architecture]

<!-- https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units -->

---
class: center, middle

![:width 80%](cluster.png)

---
class: middle

.two-column[![:width 100%](cloud.png)]
.two-column[</br></br>![:width 100%](cloud_google.png)]

---
class: center, middle

![:width 70%](2020-01-07-16-18-51.png)
![:width 100%](2020-01-07-16-18-15.png)

---
class: center, middle

# Top 500 Supercomputers

---
class: center, middle

![:width 100%](2020-01-08-08-32-06.png)

---
class: center, middle

Green 500</br>
![:width 80%](2020-01-08-08-34-54.png)


---
class: center, middle

Vendor shares</br>
![:width 100%](2020-01-08-08-39-48.png)

---
class: center, middle

![:width 45%](2020-01-08-08-41-41.png)

---
class: center, middle

More at

https://www.top500.org/

---
class: center, middle

# Example of a Parallel Computation

---
class: center, middle

# Parallel programs often look very different sequential programs

![:width 50%](2020-01-08-13-01-28.png)

---
class: center, middle

.two-column[![:width 25%](2020-01-08-08-47-38.png)</br>Sequential]

.two-column[![:width 90%](2020-01-08-08-47-47.png)</br>Parallel]

---
class: middle

# Example: program to sum numbers

```
for (int i = 0; i < n; ++i)
{
    x = ComputeNextValue();
    sum += x;
}
```

---
class: center, middle

We have $p$ cores that can compute and exchange data

---
class: center, middle

Can we accelerate our calculation by splitting the work among the cores?

---
class: middle

<!-- Add schematic with splitting of vector input into chunks  -->

```
int r; /* thread number */
int b; /* number of entries processed */
int my_first_i = r * b;
int my_last_i = (r + 1) * b;
for (int my_i = my_first_i; my_i < my_last_i; 
     my_i++) {
    my_x = ComputeNextValue();
    my_sum += my_x;
}
```

---
class: center, middle

Not that simple

Each core has computed a partial sum

All these partial sums need to summed up together

---
class: center, middle

Simplest approach: 

have one "master" thread do all the work

---
class: middle

```
if (r == 0) /* master thread */
{
    int sum = my_sum;
    for (int ro = 1; ro < p; ++ro)
    {
        int sum_ro;
        ReceiveFrom(&sum_ro, ro);
        sum += sum_ro;
    }
}
else /* worker thread */
{
    SendTo(&my_sum, 0);
}
```

---
class: center, middle

That may not be enough

If we have many cores, this final sum may take a lot of time

![:width 40%](2020-01-08-10-55-38.png)

---
class: center, middle

![:width 80%](tree_sum.png)

---
class: center, middle

This simple example illustrates the fact that it is difficult for a compiler to parallelize a program.

Instead the programmer must often re-write his code having in mind that multiple cores will be computing in parallel.

---
class: center, middle

The purpose of this class is to teach you the most common parallel languages used in science and engineering.

---
class: center, middle

# Shared Memory Processor

---
class: middle

# Schematic

- A number of processors or cores
- A shared physical memory (global memory)
- An interconnection network to connect the processors with the memory

---
class: center, middle

![:width 80%](shared_mem.png)

---
class: center, middle

# Shared memory NUMA

In many cases, the program views the memory as a single addressable space.

In reality, the memory is physically distributed.

---
class: center, middle

NUMA non-uniform memory access

Why? Faster access to memory

But, special hardware required to move data between memory banks

---
class: center, middle

![:width 80%](numa.png)

---
class: center, middle

# Bulldozer server (AMD)

![:width 70%](ccnuma_bulldozer.png)</br>
.smaller[ccNUMA uses inter-processor communication between cache controllers to keep a consistent memory image when more than one cache stores the same memory location]

<!-- hwloc's lstopo tool -->

<!-- https://en.wikipedia.org/wiki/Non-uniform_memory_access -->

---
class: center, middle

# Motherboard with 2 CPU sockets

![:width 80%](2020-01-08-11-15-32.png)

---
class: center, middle

# Performance tip on multicore

memory is key to developing high-performance multicore applications

memory is hierarchical and complex

![:width 20%](2020-01-08-11-17-05.png)

---
class: center, middle

![:width 80%](2020-01-08-11-18-49.png)

---
class: center, middle

![:width 100%](2020-01-08-11-21-24.png)

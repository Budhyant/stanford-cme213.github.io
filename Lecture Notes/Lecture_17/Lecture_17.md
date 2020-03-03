class: center, middle

# CME 213, ME 339 - Winter 2020

## Eric Darve, ICME

![:width 40%](Stanford.jpg)

“Programs must be written for people to read, and only incidentally for machines to execute.”
— Abelson and Sussman

---
class: center, middle

Let's learn about MPI communicator and groups with linear algebra

---
class: center, middle

Matrix-vector product

$$ b = A x $$

---
class: center, middle

![](2020-03-03-09-07-15.png)

---
class: center, middle

Strategy 1: partition across rows

---
class: center, middle

Step 1: replicate $b$ across all processes

```
MPI_Allgather()
```

---
class: center, middle

Step 2: local product; no communication

---
class: middle

```
/* Gather entire vector b on each processor using Allgather */
MPI_Allgather(&bloc[0], nlocal, MPI_FLOAT, &b[0], nlocal, MPI_FLOAT,
            MPI_COMM_WORLD);
// sending nlocal and receiving nlocal from any other process

/* Perform the matrix-vector multiplication involving the
locally stored submatrix. */
for(int i=0; i<nlocal; i++) {
    for(int j=0; j<n; j++) {
        x[i] += a[i*n+j]*b[j];
    }
}
```
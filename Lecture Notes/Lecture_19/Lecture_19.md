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

# Send/Recv deadlocks

---
class: center, middle

# Non-blocking MPI communications

deadlock, buffering, synchronous communications, user-specified buffering, and ready send

---
class: center, middle

# MPI send modes

Send Modes | MPI function | Completion Condition
--- | --- | ---
Synchronous send | `MPI_Ssend` | Only completes when the receive has completed
Buffered send | `MPI_Bsend` | Always completes, irrespective of the receiver
Standard send | `MPI_Send` | message sent (receiver state unknown)
Ready send | `MPI_Rsend` | May be used only when the a matching receive has already been posted
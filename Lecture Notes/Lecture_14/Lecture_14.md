class: center, middle

# CME 213, ME 339 - Winter 2020

## Eric Darve, ICME

![:width 40%](Stanford.jpg)

“Physics is the universe's operating system.”
(Steven R Garman)

---
class: center, middle

# Final project

---
class: center, middle

# Goal

Implementing a neural network in order to recognize hand-written digits

---
class: center, middle

# Logistics

Turn in | Date | Grade
--- | ---
Preliminary report + code | Friday March 6th 11 PM | 20%
Final report (4 pages) + code | Wednesday March 18th 11 PM | 80%

---
class: center, middle

# Preliminary report

Focus is on correctness

---
class: center, middle

# Final report

Profiling and analysis, performance, quality of report

What are the performance bottlenecks in your code? 

How can they be addressed?

---
class: center, middle

# Correctness

Discuss your strategy to test your code

Test outputs for valid inputs

Make sure you distinguish roundoff errors from genuine bugs

---
class: center, middle

![:width 50%](mnist_train0.jpg)

---
class: center, middle

![:width 50%](mnist_train9.jpg)

---
class: center, middle

![:width 50%](2020-02-20-10-37-19.png)

---
class: center, middle

Input layer: image

Hidden layer: `-n num`; variable size

Output layer: `softmax` vector with 10 digits

---
class: center, middle

# Softmax

$$\text{softmax}(z)\_j = \frac{\exp z\_j}{\sum_{i=1}^{10} \exp z\_i}$$

Interpreted as a probability

---
class: center, middle

Each layer is a matrix multiplication and a non-linear function

$$z = W x + b$$

$$a = \sigma(z)$$

---
class: center, middle

![:width 40%](2020-02-20-10-57-42.png)

---
class: center, middle

# How do you train a network?

Many methods but most are based on gradient descent

---
class: center, middle

# Error function

$$J(p) = \frac{1}{N} \sum\_{i=1}^N \text{error}^{(i)}(y\_i,\hat{y}\_i)$$

$p$: Weights and biases of network = all parameters

---
class: center, middle

# Gradient update

$$p \leftarrow p - \alpha \nabla_p J$$

Gradient is computed by repeated application of the chain rule

Backpropagation

---
class: center, middle

![:width 50%](2020-02-20-10-37-19.png)

---
class: center, middle

# Stochastic gradient descent

$$J\_r(p) = \frac{1}{N} \sum\_{i \in \text{random subset}} \text{error}^{(i)}(y\_i,\hat{y}\_i)$$

$$p \leftarrow p - \alpha \nabla\_p J\_r$$

---
class: center, middle

If we use a small subset, this allows more updates to the DNN coefficients

&rArr; more accurate

---
class: center, middle

Randomness of subset selection allows avoiding local minima

&rArr; better convergence

---
class: center, middle

# Sequence of operations

Forward pass = left to right; DNN prediction; compare with label

Backward propagation = right to left; chain rule; compute gradient and update DNN

Iterate until convergence

---
class: center, middle

![:width 50%](2020-02-20-10-37-19.png)

---
class: middle

# Core building blocks to implement

- Matrix-matrix products
- Non-linear activation functions

---
class: center, middle

[https://playground.tensorflow.org](https://playground.tensorflow.org)

---
class: center, middle

![:width 100%](2020-02-20-11-43-25.png)

---
class: center, middle

![:width 90%](2020-02-20-11-45-08.png)

---
class: center, middle

![:width 90%](2020-02-20-11-53-12.png)

---
class: center, middle

![](2020-02-20-11-55-40.png)

---
class: center, middle

# Regularization

$$J(p) = \frac{1}{N} \sum\_{i=1}^N \text{error}^{(i)}(y\_i,\hat{y}\_i) + \frac{1}{2} \, \lambda \, || p ||\_2^2$$

$p$: weights and biases of the network

---
class: center, middle

$$z = W x + b$$

$$a = \sigma(z)$$

Regularization makes the DNN more linear

---
class: center, middle

How can we figure out how much regularization is needed?

---
class: center, middle

Training set: used to minimize loss; involved in defining the gradient

Test set: used to test model; how accurate is it? Avoids overfitting

---
class: center, middle

![:width 80%](1400px-Traintest.png)

???

- green fit: regularized
- orange: overfitted

---
class: center, middle

Training | Test | Diagnostic
--- | --- | ---
![:width 20%](good.jpg) | ![:width 20%](bad.png) | Overfitting; &uarr; $\lambda$
![:width 20%](bad.png) | ![:width 20%](bad.png) | Too much regularization; &darr; $\lambda$ 
![:width 20%](good.jpg) | ![:width 20%](good.jpg) | Regularization is good

---
class: middle

- Training set: optimize DNN parameters
- Test set: optimize regularization
- Validation set: [optional] can be used to adjust the hyperparameters of the network

---
class: center, middle

# Steps to get started on the project

---
class: middle

1. Download script: [create_vm_final_project.sh](https://github.com/stanford-cme213/stanford-cme213.github.io/blob/master/Code/create_vm_final_project.sh)
2. Run script to create VM
3. Log on VM `gpu4` and run<br>`$ download_starter_code`<br>
This downloads the starter code.
4. You can start working. Check [GCP billing](https://console.cloud.google.com/billing) to see your remaining credits.

---
class: middle

Two main tasks in the project

1. Implement a GEMM algorithm
2. Implement the MPI algorithm for distributed memory

---
class: center, middle

# GEMM

How to increase the arithmetic intensity?

---
class: center, middle

![:width 80%](cutlass.png)

---
class: center, middle

$$c\_{ij} = \sum\_k a\_{ik} b\_{kj}$$

$$c\_{ij} \leftarrow c\_{ij} + a\_{ik} b\_{kj}$$

---
class: center, middle

$$c\_{ij} \leftarrow c\_{ij} + a\_{ik} b\_{kj}$$

Block size: $b$

Memory traffic: $2b$

Flops: $b^2$

High arithmetic intensity ![:width 10%](good.jpg)

---
class: center, middle

![:width 80%](cutlass.png)

---
class: center, middle

# MPI, distributed memory algorithm

Topic of upcoming lectures

High-level discussion of approaches

---
class: center, middle

![:width 90%](2020-02-21-10-52-06.png)

---
class: center, middle

# Data parallelism 

![:width 80%](2020-02-21-10-55-43.png)

---
class: center, middle

# Communication

$$J(p) = \frac{1}{N} \sum\_{i=1}^N \text{error}^{(i)}(y\_i,\hat{y}\_i)$$

Sum is required over all input images to compute gradient

---
class: center, middle

Parallel reduction to get $\nabla\_p J$

= Reduction for all DNN coefficients across all nodes

---
class: center, middle

![:width 100%](2020-02-21-10-55-43.png)

---
class: center, middle

Time for MPI communication is fairly significant.

A better implementation exists!

---
class: center, middle

# Model parallelism 

Much more complicated to understand but implementation has same complexity as previous approach

---
class: center, middle

![](2020-02-21-11-45-36.png)

---
class: center, middle

Reduction is required at the end to get the output labels $y$

---
class: center, middle

# Backpropagation

You have not seen the details yet. So, it will be hard to follow.

The take-home message is that no communication is required between nodes.

---
class: center, middle

![](2020-02-21-11-47-17.png)

---
class: center, middle

Warning!

Equations in previous slide were simplified for clarity

---
class: center, middle

No communication is required during the backpropagation

This implementation will be much more efficient
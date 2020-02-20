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

![:width 50%](2020-02-20-10-57-42.png)
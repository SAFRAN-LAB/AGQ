Adaptive Gaussian Quadrature in 1D and 2D
===

Adaptive Gaussian quadrature in 1D and 2D. Gauss-Legendre quadrature is used on each panel. Each panel is adaptively sub-divided till convergence of the quadrature on each panel is reached.

---
AGQ on interval:
---

<img style="float: middle" src = "https://raw.githubusercontent.com/sivaramambikasaran/AGQ/master/Domain1D.png" width=750>

To run the AGQ in 1D on an interval:

	make -f makefile_1D.mk

	./exec/quadrature1D

The driver file is ./examples/test1D.cpp.

---
AGQ on rectangle:
---

<img style="float: middle" src = "https://raw.githubusercontent.com/sivaramambikasaran/AGQ/master/Domain2D.png" width=750>

To run the AGQ in 2D on a rectangle:

	make -f makefile_2D.mk

	./exec/quadrature2D

The driver file is ./examples/test2D.cpp.
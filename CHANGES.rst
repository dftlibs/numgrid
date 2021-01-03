

Changelog
=========


Version 2.0.0
-------------

Released on 2021-01-03.

The API changed for easier maintenance and simpler use:

- No initialization or deallocation necessary.
- One-step instead of two steps (since the radial grid generation time is
  negligible compared to space partitioning, it did not make sense anymore to
  separate these steps and introduce a state).
- ``alpha_min`` is given as dictionary which saves an argument and simplifies
  explaining the API.
- The library now provides Rust and Python bindings. It used to provide C and
  Fortran bindings. The C/Fortran code lives on on the `cpp-version branch
  <https://github.com/dftlibs/numgrid/tree/cpp-version>`__.  I might bring the
  C interfaces back into the Rust code if there is sufficient interest/need.
- Note that the API will probably change again as soon as support for more
  quadratures is added (see `issue 43
  <https://github.com/dftlibs/numgrid/issues/43>`__).


Version 1.1.2
-------------

Released on 2020-08-14.

- Unpin version dependencies for Numpy and CFFI.


Version 1.1.1
-------------

Released on 2020-04-09.

- Build warning removed.
- Add .zenodo.json.


Version 1.1.0
-------------

Released on 2019-05-03.

- Do not append "numgrid" to install prefix within CMake.
- Install both shared and static libraries.
- Fortran support by default OFF.
- Python interface allows to specify the basis set instead of explicit exponent ranges
  (this uses https://github.com/MolSSI-BSE/basis_set_exchange).


Version 1.0.2
-------------

Released on 2018-10-24.


Grid memory management is now client-side
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This was done to simplify memory management and avoid memory leaks and
strange effects. The client can now query the number of grid points
before computing the grid for a certain atom type. Sounds cumbersome but
is not a problem in practice. For the Python interface this is not a
problem at all since it takes care of that.


Compute one center at a time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Motivation was to simplify code and to make it possible to pre-compute a
grid for a certain atom/basis type. This also means that the code can be
optimized and parallelized on the client side.


Full basis set does not need to be provided
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Great simplification. All that is needed now is the steepest exponent
and a set of smallest exponents for each angular momentum.


Separate arrays for x, y, z, and weights
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

They can be recombined on the client side but it makes it easier to
understand how the grid information is stored in memory.


Version 0.5.0
-------------

Released on 2016-12-26.

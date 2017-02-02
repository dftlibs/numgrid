program test

   use numgrid
   use, intrinsic :: iso_c_binding, only: c_ptr

   implicit none

   real(8)              :: radial_precision
   integer              :: min_num_angular_points
   integer              :: max_num_angular_points
   integer              :: num_centers
   real(8), allocatable :: center_coordinates(:)
   integer, allocatable :: center_elements(:)
   integer              :: num_outer_centers
   real(8), allocatable :: outer_center_coordinates(:)
   integer, allocatable :: outer_center_elements(:)
   integer              :: num_shells
   integer, allocatable :: shell_centers(:)
   integer, allocatable :: shell_l_quantum_numbers(:)
   integer, allocatable :: shell_num_primitives(:)
   real(8), allocatable :: primitive_exponents(:)
   integer              :: num_points
   integer, parameter   :: io_unit = 13
   real(8)              :: ref(4)
   integer              :: i, j, k
   real(8)              :: error
   real(8), pointer     :: grid(:)
   type(c_ptr)          :: context

   radial_precision = 1.0d-12
   min_num_angular_points = 86
   max_num_angular_points = 302

   num_centers = 3

   allocate(center_coordinates(num_centers*3))

   center_coordinates(1) = 0.0000d+00
   center_coordinates(2) = 0.0000d+00
   center_coordinates(3) = 0.0000d+00
   center_coordinates(4) = 5.5168d-01
   center_coordinates(5) = 7.7340d-01
   center_coordinates(6) = 0.0000d+00
   center_coordinates(7) = 5.5168d-01
   center_coordinates(8) = -7.734d-01
   center_coordinates(9) = 0.0000d+00

   allocate(center_elements(num_centers))

   center_elements(1) = 8
   center_elements(2) = 1
   center_elements(3) = 1

   num_outer_centers = 0

   num_shells = 12

   allocate(shell_centers(num_shells))

   shell_centers( 1) = 1
   shell_centers( 2) = 1
   shell_centers( 3) = 1
   shell_centers( 4) = 1
   shell_centers( 5) = 1
   shell_centers( 6) = 1
   shell_centers( 7) = 2
   shell_centers( 8) = 2
   shell_centers( 9) = 2
   shell_centers(10) = 3
   shell_centers(11) = 3
   shell_centers(12) = 3

   allocate(shell_l_quantum_numbers(num_shells))

   shell_l_quantum_numbers( 1) = 0
   shell_l_quantum_numbers( 2) = 0
   shell_l_quantum_numbers( 3) = 0
   shell_l_quantum_numbers( 4) = 1
   shell_l_quantum_numbers( 5) = 1
   shell_l_quantum_numbers( 6) = 2
   shell_l_quantum_numbers( 7) = 0
   shell_l_quantum_numbers( 8) = 0
   shell_l_quantum_numbers( 9) = 1
   shell_l_quantum_numbers(10) = 0
   shell_l_quantum_numbers(11) = 0
   shell_l_quantum_numbers(12) = 1

   allocate(shell_num_primitives(num_shells))

   shell_num_primitives( 1) = 9
   shell_num_primitives( 2) = 9
   shell_num_primitives( 3) = 1
   shell_num_primitives( 4) = 4
   shell_num_primitives( 5) = 1
   shell_num_primitives( 6) = 1
   shell_num_primitives( 7) = 4
   shell_num_primitives( 8) = 1
   shell_num_primitives( 9) = 1
   shell_num_primitives(10) = 4
   shell_num_primitives(11) = 1
   shell_num_primitives(12) = 1

   allocate(primitive_exponents(37))

   primitive_exponents( 1) = 1.172d+04
   primitive_exponents( 2) = 1.759d+03
   primitive_exponents( 3) = 4.008d+02
   primitive_exponents( 4) = 1.137d+02
   primitive_exponents( 5) = 3.703d+01
   primitive_exponents( 6) = 1.327d+01
   primitive_exponents( 7) = 5.025d+00
   primitive_exponents( 8) = 1.013d+00
   primitive_exponents( 9) = 3.023d-01
   primitive_exponents(10) = 1.172d+04
   primitive_exponents(11) = 1.759d+03
   primitive_exponents(12) = 4.008d+02
   primitive_exponents(13) = 1.137d+02
   primitive_exponents(14) = 3.703d+01
   primitive_exponents(15) = 1.327d+01
   primitive_exponents(16) = 5.025d+00
   primitive_exponents(17) = 1.013d+00
   primitive_exponents(18) = 3.023d-01
   primitive_exponents(19) = 3.023d-01
   primitive_exponents(20) = 1.770d+01
   primitive_exponents(21) = 3.854d+00
   primitive_exponents(22) = 1.046d+00
   primitive_exponents(23) = 2.753d-01
   primitive_exponents(24) = 2.753d-01
   primitive_exponents(25) = 1.185d+00
   primitive_exponents(26) = 1.301d+01
   primitive_exponents(27) = 1.962d+00
   primitive_exponents(28) = 4.446d-01
   primitive_exponents(29) = 1.220d-01
   primitive_exponents(30) = 1.220d-01
   primitive_exponents(31) = 7.270d-01
   primitive_exponents(32) = 1.301d+01
   primitive_exponents(33) = 1.962d+00
   primitive_exponents(34) = 4.446d-01
   primitive_exponents(35) = 1.220d-01
   primitive_exponents(36) = 1.220d-01
   primitive_exponents(37) = 7.270d-01

   context = numgrid_new_context()

   call numgrid_generate_grid(context,                  &
                      radial_precision,         &
                      min_num_angular_points,   &
                      max_num_angular_points,   &
                      num_centers,              &
                      center_coordinates,       &
                      center_elements,          &
                      num_outer_centers,        &
                      outer_center_coordinates, &
                      outer_center_elements,    &
                      num_shells,               &
                      shell_centers,            &
                      shell_l_quantum_numbers,  &
                      shell_num_primitives,     &
                      primitive_exponents)

   num_points = numgrid_get_num_points(context)

   if (num_points /= 46220) stop 1

   grid => numgrid_get_grid(context)

   open(unit=io_unit, file='../test/referece_grid.txt', access='sequential', action='read')
   k = 1
   do i = 1, num_points
      read(io_unit, *) ref(1), ref(2), ref(3), ref(4)
      do j = 1, 4
         error = grid(k) - ref(j)
         if (dabs(ref(j)) > 1.0e-15) error = error/ref(j)
         if (dabs(error) > 1.0e-5) stop 1
         k = k + 1
      end do
   end do
   close(io_unit)

   deallocate(center_coordinates)
   deallocate(center_elements)
   deallocate(shell_centers)
   deallocate(shell_l_quantum_numbers)
   deallocate(shell_num_primitives)
   deallocate(primitive_exponents)

   call numgrid_free_context(context)

end program

module numgrid

   use, intrinsic :: iso_c_binding, only: c_ptr, c_f_pointer, c_double, c_int

   implicit none

   private

   public new_context
   public free_context
   public generate_grid
   public get_num_points
   public get_grid

   interface new_context
      function new_context() result(context) bind (C)
         import :: c_ptr
         type(c_ptr) :: context
      end function
   end interface

   interface free_context
      subroutine free_context(context) bind (C)
         import :: c_ptr
         type(c_ptr), value :: context
      end subroutine
   end interface

   interface generate_grid
      subroutine generate_grid(context,                  &
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
                               primitive_exponents) bind (C)
         import :: c_ptr, c_double, c_int
         type(c_ptr), value                :: context
         real(c_double), intent(in), value :: radial_precision
         integer(c_int), intent(in), value :: min_num_angular_points
         integer(c_int), intent(in), value :: max_num_angular_points
         integer(c_int), intent(in), value :: num_centers
         real(c_double), intent(in)        :: center_coordinates(*)
         integer(c_int), intent(in)        :: center_elements(*)
         integer(c_int), intent(in), value :: num_outer_centers
         real(c_double), intent(in)        :: outer_center_coordinates(*)
         integer(c_int), intent(in)        :: outer_center_elements(*)
         integer(c_int), intent(in), value :: num_shells
         integer(c_int), intent(in)        :: shell_centers(*)
         integer(c_int), intent(in)        :: shell_l_quantum_numbers(*)
         integer(c_int), intent(in)        :: shell_num_primitives(*)
         real(c_double), intent(in)        :: primitive_exponents(*)
      end subroutine
   end interface

   interface get_num_points
      function get_num_points(context) result(num_points) bind (C)
         import :: c_ptr, c_int
         type(c_ptr), value :: context
         integer(c_int)     :: num_points
      end function
   end interface

contains

   function get_grid(context) result(grid)

      type(c_ptr), value :: context
      real(8), pointer   :: grid(:)

      integer          :: n
      type(c_ptr)      :: c_p
      real(8), pointer :: f_p(:)

      interface
         function c_get_grid(context) result(grid) bind(C, name='get_grid')
            import :: c_ptr
            type(c_ptr), value :: context
            type(c_ptr)        :: grid
         end function
      end interface

      ! we need to do this because c_f_pointer needs
      ! to know the length
      n = get_num_points(context)

      c_p = c_get_grid(context)
      call c_f_pointer(c_p, f_p, [n*4])
      grid => f_p

   end function

end module

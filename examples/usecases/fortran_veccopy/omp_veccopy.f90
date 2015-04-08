subroutine vector_square(n, a, b)
      integer i, n, b(n), a(n)
!$omp parallel do 
      do i=1,n
          b(i) = a(i) * a(i)
      enddo
!$omp end parallel do
end subroutine vector_square

program main
      integer, parameter :: n=256
      integer i, b(n), a(n)
      do i=1,n
          a(i) = i
          b(i) = 0
      enddo 
      call vector_square(n, a, b)
      print *, ' Array B is ', b
end program main 

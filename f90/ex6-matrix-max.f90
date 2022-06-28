program main
    use omp_lib
    implicit none
    integer, dimension (:, :), allocatable :: A
    integer :: i, j, n, max_val
    real :: u
    real(8) :: t0, t1
    character(32) :: arg

    if(command_argument_count() .eq. 1) then
        call get_command_argument(1, arg)
        read (arg, *) n
    else
        call get_command_argument(0, arg)
        call usage(arg)
        stop 1
    endif

    call random_seed()

    allocate(A(n, n))
    do i=1,n
        do j=1,n
            call random_number(u)
            A(i, j) = floor(u*huge(n))
        end do
    end do

    t0 = omp_get_wtime()
    ! max()/maxval() are intrinsic functions
    max_val = kmax(A)
    t1 = omp_get_wtime()

    print *, "max(A) = ", max_val
    print *, "Total time taken: ", t1-t0

    deallocate(A)

contains
    pure integer function kmax(A)
        integer, dimension(:, :), intent (in) :: A
        integer :: i, j, n, m
        n = size(A, 1)
        m = size(A, 2)
        kmax = A(1, 1)
        do i=1,n
            do j=1,m
                if(A(i, j) .gt. kmax) then
                    kmax = A(i, j)
                endif
            end do
        end do
    end function

end program

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine

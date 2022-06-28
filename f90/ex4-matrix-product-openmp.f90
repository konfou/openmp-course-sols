program main
    use omp_lib
    implicit none

    interface
        subroutine mat_prod(A, B, C)
            integer, dimension(:, :) :: A, B, C
        end subroutine
    end interface

    integer, dimension (:, :), allocatable :: A, B, C
    integer :: i, j, n
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

    allocate(A(n, n))
    allocate(B(n, n))
    allocate(C(n, n))
    do i=1,n
        do j=1,n
            A(i, j) = mod(i*j, 100)
            B(i, j) = mod(i*j, 100)
            C(i, j) = 0
        end do
    end do

    t0 = omp_get_wtime()
    call mat_prod(A,B,C)
    t1 = omp_get_wtime()

    print *, "Total time taken: ", t1-t0

    deallocate(A)
    deallocate(B)
    deallocate(C)
end program

subroutine mat_prod(A, B, C)
    integer, dimension(:, :) :: A, B, C
    integer :: i, j, k, n, m
    n = size(A, 1)
    m = size(B, 2)
    !$omp parallel do default(none) shared(n,m,A,B,C) private(i,j,k)
    do i=1,n
        do k=1,n
            do j=1,m
                C(i, j) = C(i, j) + A(i, k) * B(k, j)
            end do
        end do
    end do
    !$omp end parallel do
end subroutine

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine

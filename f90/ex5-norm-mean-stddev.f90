program main
    use omp_lib
    implicit none
    real, dimension (:, :), allocatable :: A
    integer :: i, j, n
    real :: avg, stddev
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

    A = normrnd(n, n, 0.0, 1.0)

    t0 = omp_get_wtime()
    avg = mean(A)
    stddev = std(avg, A)
    t1 = omp_get_wtime()

    print *, "mean(A) = ", avg
    print *, "std(A) = ", stddev
    print *, "Total time taken: ", t1-t0

    deallocate(A)

contains
    function normrnd(n, m, mu, sigma) result(A)
        integer, intent (in) :: n, m
        real, intent (in) :: mu, sigma
        real :: u1, u2, z0, A(n, m)
        do i=1,n
            do j=1,m
                call random_number(u1)
                call random_number(u2)
                z0 = sqrt(-2 * log(u1)) * cos(6.28 * u2)
                A(i, j) = mu + sigma * z0
            end do
        end do
    end function

    real function mean(A)
        real, dimension(:, :), intent(in) :: A
        real :: msum = 0
        integer :: i, j, n, m
        n = size(A, 1)
        m = size(A, 2)
        do i=1,n
            do j=1,m
                msum = msum + A(i, j)
            end do
        end do
        mean = msum / (n * m)
    end function

    real function std(avg, A)
        real, intent(in) :: avg
        real, dimension(:, :), intent(in) :: A
        real :: msum = 0
        integer :: i, j, n, m
        n = size(A, 1)
        m = size(A, 2)
        do i=1,n
            do j=1,m
                msum = msum + (A(i, j) - avg) * (A(i, j) - avg)
            end do
        end do
        std = sqrt(msum / (n * m - 1))
    end function

end program

subroutine usage(arg)
    character(32) :: arg
    print *, "Usage: ", arg(1:len_trim(arg)), " <length>"
end subroutine

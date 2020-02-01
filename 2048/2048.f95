module game2048
    implicit none
    integer(kind=4),parameter :: size=4, full_size=16
    integer(kind=4) :: status(size, size) = 0

contains
    subroutine game_start()
        implicit none
        real :: r
        integer :: x, y, choose
        call random_seed()
        call random_number(r)
        x = ceiling(r*4)
        call random_number(r)
        y = ceiling(r*4)
        status = 0
        status(x, y) = 2
        write(*,"(A20)") "This is a 2048 game;"
        write(*,"(A54)") "use 'w'(up), 's'(down), 'a'(left), 'd'(right) to play."
        write(*,"(A11)", advance="NO") "Ready?[Y/n]"
        call getchar(choose)
        if (choose /= ichar('Y') .and. choose /= ichar('y')) stop
        call clean_console()
        call print_frame()
        do while (step())
            continue
        end do
    end subroutine game_start
    
    subroutine print_frame()
        implicit none
        character(len=30) :: line
        integer :: i, j
        line = repeat('-', 30)
        call set_cursor_potition(0, 0)
        write(*,"(A30)") line
        do i=1,size
            do j=1,size
                if (status(i,j)/=0) then
                    write(*,"('| ',I4,1X)", advance="NO") status(i,j)
                else
                    write(*,"(A1,6X)", advance="NO") "|"
                end if
            end do
            write(*,"(' |',/,A30)") line
        end do
    end subroutine print_frame
    
    function shift2048(change)
        implicit none
        integer(kind=4),intent(in) :: change(size)
        integer(kind=4) :: shift2048(size)
        integer(kind=4) :: temp(size)
        integer :: read_idx, write_idx, i
        temp = 0
        temp = pack(change, change/=0, temp)
        read_idx = count(temp /= 0)
        shift2048 = 0
        write_idx = 1
        i = 1
        do while(i <= read_idx)
            if(i < size .and. temp(i)==temp(i+1)) then
                shift2048(write_idx) = temp(i) * 2
                write_idx = write_idx + 1
                i  = i + 2
            else
                shift2048(write_idx) = temp(i)
                write_idx = write_idx + 1
                i = i + 1
            end if
        end do
        return
    end function shift2048
    
    subroutine change_status(direction)
        implicit none
        character(len=*),intent(in) :: direction
        integer :: i
        integer(kind=4) :: temp(size)
        select case(direction)
        case("UP")
            do i=1,size
                temp = shift2048(status(:,i))
                status(:,i) = temp
            end do
        case("DOWN")
            do i=1,size
                temp = shift2048(status(size:1:-1, i))
                status(size:1:-1, i) = temp
            end do
        case("LEFT")
            do i=1,size
                temp = shift2048(status(i,:))
                status(i,:) = temp
            end do
        case("RIGHT")
            do i=1,size
                temp = shift2048(status(i,size:1:-1))
                status(i, size:1:-1) = temp
            end do
        case default
            continue
        end select
        return
    end subroutine change_status
    
    subroutine add_new()
        implicit none
        integer(kind=4) :: temp(full_size)
        integer :: zero_counts, i, rand_num, p
        real :: rnum
        zero_counts = count(status==0)
        if (zero_counts == 0) return
        call random_seed()
        call random_number(rnum)
        rand_num = floor(rnum*zero_counts)
        temp = reshape(status, shape(temp))
        do i=1, full_size
            zero_counts = zero_counts - count((/temp(i)==0/))
            if(zero_counts == rand_num) then
                call random_number(rnum)
                select case(maxval(temp))
                case(64)
                    p = floor(rnum*8.0)
                case(128)
                    p = floor(rnum*5.0)
                case(256)
                    p = floor(rnum*3.0)
                case(512)
                    p = floor(rnum*2.0)
                case(1024)
                    p = floor(rnum*1.5)
                case default
                    p = 1
                end select
                if (p==0) then
                    temp(i) = 4
                else
                    temp(i) = 2
                endif
                exit
            end if
        end do
        status = reshape(temp, shape(status))
    end subroutine add_new

    logical function step()
        implicit none    
        integer(kind=4) :: ch
        call getch(ch)
        select case(ch)
        case(ichar('w'),72,65)
            call change_status("UP")
        case(ichar('s'),80,66)
            call change_status("DOWN")
        case(ichar('a'),75,68)
            call change_status("LEFT")
        case(ichar('d'),77,67)
            call change_status("RIGHT")
        case(ichar('q'),ichar('Q'))
            write(*,"(A9)") "You quit!"
            step = .false.
            return
        case default
            step = .true.
            return
        end select
        select case(check_status())
        case(0)
            continue
        case(1)
            call print_frame()
            write(*,"(A10)") "You win!!!"
            step = .false.
            return
        case(-1)
            call print_frame()
            write(*,"(A9)") "You fail!"
            step = .false.
            return
        case default
            call print_frame()
            write(*,"(A5)") "error"
            step = .false.
            return
        end select
        call add_new()
        call print_frame()
        step = .true.
        return
    end function step
    
    integer function check_status()
        implicit none
        integer :: i, j
        if (any(status == 2048)) then
            check_status = 1
            return
        end if
        if (any(status == 0)) then
            check_status = 0
            return
        end if
        do i=1,size
            do j=1,size-1
                if(status(i,j) == status(i,j+1)) then
                    check_status = 0
                    return
                end if
            end do
        end do
        do j=1,size
            do i=1,size-1
                if(status(i,j) == status(i+1,j)) then
                    check_status = 0
                    return
                end if
            end do
        end do
        check_status = -1
        return
    end function check_status
end module game2048

program game
    use game2048
    implicit none
    call game_start()
end program game
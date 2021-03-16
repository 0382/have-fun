using Printf
const N2048 = 4
const NS2048 = N2048^2
const status = zeros(Int, 4, 4)

const greet_string = """
This is a 2048 game;
use 'w'(up), 's'(down), 'a'(left), 'd'(right) to play.
Ready? [Y/N]"""

include("console.jl")

function start_2048()
    print(greet_string)
    choose = getch()
    if choose != 'Y' % Cint && choose != 'y' % Cint
        return
    end
    status[mod1(rand(Int), NS2048)] = 2
    clean_console()
    print_frame()
    while step2048() end
end

function print_frame()
    set_cursor_position(0, 0)
    line = repeat('-', 30)
    println(line)
    for i = 1:N2048
        for j = 1:N2048
            print("| ")
            if status[i, j] != 0
                @printf("%4d", status[i,j])
            else
                print("    ")
            end
            print(" ")
        end
        println(" |")
        println(line)
    end
end

function shift2048(vec::Vector{Int})::Vector{Int}
    N = size(vec, 1)
    temp = zeros(Int, N)
    shift = zeros(Int, N)
    mask = vec .!= 0
    read_idx = count(mask)
    temp[1:read_idx] .= vec[mask]
    write_idx = 1
    i = 1
    while i <= read_idx
        if i < N && temp[i] == temp[i + 1]
            shift[write_idx] = temp[i] * 2
            write_idx += 1
            i += 2
        else
            shift[write_idx] = temp[i]
            write_idx += 1
            i += 1
        end
    end
    return shift
end

function change_status(direction::Symbol)
    if direction == :up
        for i = 1:N2048
            status[:, i] .= shift2048(status[:, i])
        end
    elseif direction == :down
        for i = 1:N2048
            status[end:-1:1, i] .= shift2048(status[end:-1:1, i])
        end
    elseif direction == :left
        for i = 1:N2048
            status[i, :] .= shift2048(status[i, :])
        end
    elseif direction == :right
        for i = 1:N2048
            status[i, end:-1:1] .= shift2048(status[i, end:-1:1])
        end
    end
    # println("after shift")
    # print_frame()
end

function add_new()
    zero_counts = count(status .!= 0)
    zero_counts == 0 && return
    rand_num = mod(rand(Int), zero_counts)
    for i = 1:NS2048
        zero_counts -= status[i] == 0
        if zero_counts == rand_num
            r = rand(Float64)
            max_s = maximum(status)
            tab = Dict(
                    64 => floor(Int, r * 8.),
                    128 => floor(Int, r * 5.),
                    256 => floor(Int, r * 3.),
                    512 => floor(Int, r * 2.),
                    1024 => floor(Int, r * 1.5)
                )
            p = one(Int)
            if max_s in keys(tab)
                p = tab[max_s]
            end
            if p == 0
                status[i] = 4
            else
                status[i] = 2
            end
            return
        end
    end
end

function step2048()
    ch = getch()
    if ch in ['w' % Int, 72, 65]
        change_status(:up)
    elseif ch in ['s' % Int, 80, 66]
        change_status(:down)
    elseif ch in ['a' % Int, 75, 68]
        change_status(:left)
    elseif ch in ['d' % Int, 77, 67]
        change_status(:right)
    elseif ch in ['q' % Int, 'Q' % Int]
        print("You quit!")
        return false
    else
        return true
    end
    check = check2048()
    if check == 1
        print_frame()
        print("You win!!!")
        return false
    elseif check == -1
        print_frame()
        print("You fail!")
        return false
    end
    add_new()
    print_frame()
    return true
end

function check2048()::Int
    if any(status .== 2048)
        return 1
    elseif any(status .== 0)
        return 0
    elseif any(status[:, 1:end - 1] .== status[:, 2:end])
        return 0
    elseif any(status[1:end - 1, :] .== status[2:end, :])
        return 0
    else
        return -1
    end
end

start_2048()
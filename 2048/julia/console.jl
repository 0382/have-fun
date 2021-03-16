# bind console api for julia

# for linux
const STDIN_FILENO = 0
const ICANON = 0x02
const ECHO = 0x08
const TCSANOW = 0

# for windows
const STD_OUTPUT_HANDLE = reinterpret(Culong, Clong(-11))
const HANDLE = Ptr{Cvoid}

mutable struct termios
    c_iflag::Cuint		#  input mode flags
    c_oflag::Cuint		#  output mode flags
    c_cflag::Cuint		#  control mode flags
    c_lflag::Cuint		#  local mode flags
    c_line::Cuchar		#  line discipline
    c_cc::NTuple{32,Cuchar}	#  control characters
    c_ispeed::Cint		#  input speed
    c_ospeed::Cint		#  output speed
end

mutable struct COORD
    x::Cshort
    y::Cshort
end

termios() = termios(0, 0, 0, 0, 0, ntuple(_ -> Cuchar(0), 32), 0, 0)

function getch_linux()::Cint
    oldt = termios()
    ccall(:tcgetattr, Cint, (Cint, Ref{termios}), STDIN_FILENO, Ref(oldt))
    newt = oldt
    newt.c_lflag &= ~(ICANON | ECHO)
    ccall(:tcsetattr, Cint, (Cint, Cint, Ref{termios}), STDIN_FILENO, TCSANOW, Ref(newt))
    ch = ccall(:getchar, Cint, ())
    ccall(:tcsetattr, Cint, (Cint, Cint, Ref{termios}), STDIN_FILENO, TCSANOW, Ref(oldt))
    return ch
end

getch()::Cint = begin
    @static if Sys.iswindows()
        return ccall(:_getch, Cint, ())
    else
        return getch_linux()
    end
end

function clean_console()
    @static if Sys.iswindows()
        ccall(:system, Cint, (Cstring,), "cls")
    else
        print("\033[2J")
    end
end

function set_cursor_position(x::Int, y::Int)
    @static if Sys.iswindows()
        handle = ccall(:GetStdHandle, HANDLE, (UInt32,), STD_OUTPUT_HANDLE)
        pos = COORD(x % Cshort, y % Cshort)
        ccall(:SetConsoleCursorPosition, Cint, (HANDLE, COORD), handle, pos)
    else
        @printf("\033[%d;%dH", y + 1, x + 1)
    end
end

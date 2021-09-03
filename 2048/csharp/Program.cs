using System;

namespace learn_dotnet
{
    enum Direction { Up, Down, Left, Right }

    class Game2048
    {
        private int[] status;
        public Game2048()
        {
            status = new int[16];
            for (int i = 0; i < status.GetLength(0); ++i)
            {
                status[i] = 0;
            }
            Random rnd = new Random();
            status[rnd.Next(16)] = 2;
        }

        public void start()
        {
            Console.WriteLine("This is a 2048 game;");
            Console.WriteLine("use 'w'(up), 's'(down), 'a'(left), 'd'(right) to play.");
            Console.WriteLine("Ready?[Y/n]");
            ConsoleKey ch = Console.ReadKey(true).Key;
            if (ch != ConsoleKey.Y)
            {
                return;
            }
            clean_console();
            print_frame();
            while (step()) ;
        }

        private bool step()
        {
            ConsoleKey ch = Console.ReadKey(true).Key;
            if (ch == ConsoleKey.W || ch == ConsoleKey.UpArrow)
            {
                change_status(Direction.Up);
            }
            else if (ch == ConsoleKey.S || ch == ConsoleKey.DownArrow)
            {
                change_status(Direction.Down);
            }
            else if (ch == ConsoleKey.A || ch == ConsoleKey.LeftArrow)
            {
                change_status(Direction.Left);
            }
            else if (ch == ConsoleKey.D || ch == ConsoleKey.RightArrow)
            {
                change_status(Direction.Right);
            }
            else if (ch == ConsoleKey.Q)
            {
                Console.WriteLine("You quit!");
                return false;
            }
            else
            {
                return true;
            }
            switch (check_status())
            {
                case 0:
                    break;
                case 1:
                    print_frame();
                    Console.WriteLine("You win!!!");
                    return false;
                case -1:
                    print_frame();
                    Console.WriteLine("You fail!");
                    return false;
                default:
                    Console.WriteLine("invalid status code");
                    return false;
            }
            add_new_number();
            print_frame();
            return true;
        }

        private void print_frame()
        {
            set_cursor_pos(0, 0);
            const string line = "-----------------------------";
            Console.WriteLine(line);
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    Console.Write("| ");
                    if (status[i * 4 + j] != 0)
                    {
                        Console.Write("{0, 4} ", status[i * 4 + j]);
                    }
                    else
                    {
                        Console.Write("     ");
                    }
                }
                Console.WriteLine("|\n{0}", line);
            }
        }

        private void shift(ref int[] arr)
        {
            int[] temp = new int[4];
            int read_idx = 0;
            int write_idx = 0;
            for (int i = 0; i < 4; ++i)
            {
                if (arr[i] != 0)
                {
                    temp[read_idx++] = arr[i];
                }
                arr[i] = 0;
            }
            int idx = 0;
            while (idx < read_idx)
            {
                if (idx < 3 && temp[idx] == temp[idx + 1])
                {
                    arr[write_idx++] = temp[idx] * 2;
                    idx += 2;
                }
                else
                {
                    arr[write_idx++] = temp[idx++];
                }
            }
        }

        private void change_status(Direction dirct)
        {
            int[] arr = new int[4];
            switch (dirct)
            {
                case Direction.Up:
                    for (int j = 0; j < 4; ++j)
                    {
                        for (int i = 0; i < 4; ++i)
                        {
                            arr[i] = status[i * 4 + j];
                        }
                        shift(ref arr);
                        for (int i = 0; i < 4; ++i)
                        {
                            status[i * 4 + j] = arr[i];
                        }
                    }
                    break;
                case Direction.Down:
                    for (int j = 0; j < 4; ++j)
                    {
                        for (int i = 0; i < 4; ++i)
                        {
                            arr[3 - i] = status[i * 4 + j];
                        }
                        shift(ref arr);
                        for (int i = 0; i < 4; ++i)
                        {
                            status[i * 4 + j] = arr[3 - i];
                        }
                    }
                    break;
                case Direction.Left:
                    for (int i = 0; i < 4; ++i)
                    {
                        for (int j = 0; j < 4; ++j)
                        {
                            arr[j] = status[i * 4 + j];
                        }
                        shift(ref arr);
                        for (int j = 0; j < 4; ++j)
                        {
                            status[i * 4 + j] = arr[j];
                        }
                    }
                    break;
                case Direction.Right:
                    for (int i = 0; i < 4; ++i)
                    {
                        for (int j = 0; j < 4; ++j)
                        {
                            arr[3 - j] = status[i * 4 + j];
                        }
                        shift(ref arr);
                        for (int j = 0; j < 4; ++j)
                        {
                            status[i * 4 + j] = arr[3 - j];
                        }
                    }
                    break;
            }
        }

        private void add_new_number()
        {
            int zero_counts = 0;
            for (int i = 0; i < 16; ++i)
            {
                zero_counts += Convert.ToInt32(status[i] == 0);
            }
            if (zero_counts == 0) return;
            Random rnd = new Random();
            int rand_num = rnd.Next(zero_counts);
            for (int i = 0; i < 16; ++i)
            {
                zero_counts -= Convert.ToInt32(status[i] == 0);
                if (zero_counts == rand_num)
                {
                    int max_num = status[0];
                    for (int j = 0; j < 16; ++j)
                    {
                        max_num = Math.Max(status[j], max_num);
                    }
                    int temp = rnd.Next();
                    switch (max_num)
                    {
                        case 64:
                            status[i] = temp % 8 > 0 ? 2 : 4;
                            break;
                        case 128:
                            status[i] = temp % 5 > 0 ? 2 : 4;
                            break;
                        case 256:
                            status[i] = temp % 3 > 0 ? 2 : 4;
                            break;
                        case 512:
                            status[i] = temp % 2 > 0 ? 2 : 4;
                            break;
                        case 1024:
                            status[i] = temp % 3 > 1 ? 2 : 4;
                            break;
                        default:
                            status[i] = 2;
                            break;
                    }
                    break;
                }
            }
        }

        private int check_status()
        {
            for (int i = 0; i < 16; ++i)
            {
                if (status[i] == 2048)
                    return 1;
            }
            for (int i = 0; i < 16; ++i)
            {
                if (status[i] == 0)
                    return 0;
            }
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (status[i * 4 + j] == status[i * 4 + j + 1]) return 0;
                }
            }
            for (int j = 0; j < 4; ++j)
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (status[i * 4 + j] == status[(i + 1) * 4 + j]) return 0;
                }
            }
            return -1;
        }

        private void clean_console()
        {
            Console.Write("\x1b[2J");
        }

        private void set_cursor_pos(int x, int y)
        {
            Console.Write("\x1b[{0};{1}H", y + 1, x + 1);
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Game2048 game = new Game2048();
            game.start();
        }
    }
}

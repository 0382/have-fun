/**encoding=utf-8
 * author:      0.382
 * description: 2048小游戏
 * language:    c++
 * enviroment:  windows 10
 */
#pragma once
#ifndef JSHL_GAME2048_H
#define JSHL_GAME2048_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "_console.h"

constexpr int size = 4;
constexpr int full_size = size * size;

class basic_2048{
public:
    basic_2048();

    enum Direction {Up, Down, Left, Right};

    virtual void start();
    virtual void print_frame();
    static void shift(int _change[size]);
    void change_status(const Direction &);
    virtual void add_new();
    bool step();
    int check_status();

private:
    int status[full_size]; // 16个格子中的数
};

// 初始化时，给随机一个格子赋值为 2
basic_2048::basic_2048(){
    std::memset(status, 0, full_size * sizeof(int));
    std::srand(std::time(nullptr));
    status[std::rand()%16] = 2;
}

// 开始游戏
void basic_2048::start(){
    std::cout << "This is a 2048 game;\n"
              << "use 'w'(up), 's'(down), 'a'(left), 'd'(right) to play.\n"
              << "Ready?[Y/n]";
    int choose = std::getchar();
    if(choose != 'Y' && choose != 'y') return;
    clean_console();
    print_frame();
    while(step());  // 开始循环，直到游戏结束
}

// 打印当前状态
void basic_2048::print_frame(){
    set_cursor_potition(0, 0);
    const char* line = "------------------------------";
    std::cout << line << "\n";
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size; ++j)
        {
            std::cout << "| ";
            if(status[i*4 + j])
                std::cout << std::setw(4) << status[i*4 + j];
            else std::cout << "    ";
            std::cout << " ";
        }
        std::cout << " |\n" << line << "\n";
    }
}

void basic_2048::shift(int _change[size]){
    int temp[size];
    int read_idx = 0, write_idx = 0;
    std::memset(temp, 0, size * sizeof(int));
    for(int i=0; i<size; ++i) if(_change[i]) temp[read_idx++] = _change[i];
    std::memset(_change, 0, size * sizeof(int));
    for(int i=0; i<read_idx;)
    {
        if(i<size-1 && temp[i]==temp[i+1])
        {
            _change[write_idx++] = temp[i] << 1;
            i += 2;
        }
        else
        {
            _change[write_idx++] = temp[i++];
        }
    }
}

// 在某个方向上移动
void basic_2048::change_status(const basic_2048::Direction &direction){
    switch(direction)
    {
        case Up:
        {
            int _change[size];
            for(int j=0; j<size; ++j)
            {
                for(int i=0; i<size; ++i) _change[i] = status[i*size+j];
                shift(_change);
                for(int i=0; i<size; ++i) status[i*size+j] = _change[i];
            }
            break;
        }
        case Down:
        {
            int _change[size];
            for(int j=0; j<size; ++j)
            {
                for(int i=0; i<size; ++i) _change[size-i-1] = status[i*size+j];
                shift(_change);
                for(int i=0; i<size; ++i) status[i*size+j] = _change[size-i-1];
            }
            break;
        }
        case Left:
        {
            int _change[size];
            for(int i=0; i<size; ++i)
            {
                for(int j=0; j<size; ++j) _change[j] = status[i*size+j];
                shift(_change);
                for(int j=0; j<size; ++j) status[i*size+j] = _change[j];
            }
            break;
        }
        case Right:
        {
            int _change[size];
            for(int i=0; i<size; ++i)
            {
                for(int j=0; j<size; ++j) _change[size-j-1] = status[i*size+j];
                shift(_change);
                for(int j=0; j<size; ++j) status[i*size+j] = _change[size-j-1];
            }
            break;
        }
        default: break;
    }
}

// 在某个空位添加数字
// 有大数字的时候，有一定概率出现 4,
// 但是我不确定这个概率有没有规定，
// 这里纯粹是我自定义的
void basic_2048::add_new(){
    int zero_counts = 0;
    for(int i=0; i<16; ++i) zero_counts += (status[i]==0);
    if(zero_counts == 0) return;
    std::srand(std::time(nullptr));
    int rand_num = std::rand()%zero_counts;
    for(int i=0; i<16; ++i)
    {
        zero_counts -= (status[i]==0);
        if(zero_counts==rand_num)
        {
            int max_num = status[0];
            for(int j=0; j<16; ++j) max_num = (max_num > status[j]) ? max_num : status[j];
            switch(max_num)
            {
                case 64:   status[i] = (std::rand()%8 > 0) ? 2 : 4; break;
                case 128:  status[i] = (std::rand()%5 > 0) ? 2 : 4; break;
                case 256:  status[i] = (std::rand()%3 > 0) ? 2 : 4; break;
                case 512:  status[i] = (std::rand()%2 > 0) ? 2 : 4; break;
                case 1024: status[i] = (std::rand()%3 > 1) ? 2 : 4; break;
                default: status[i] = 2; break;
            }
            break;
        }
    }
}


// 按下一个按键，执行一步
bool basic_2048::step(){
    int ch = getch();
    switch(ch)
    {
        // 第一列是常用的方向键 wsad
        // 第二列是 windows 下的方向键转义
        // 第三列是 linux 下的方向键转义
        // 这些转义当然撞上了部分字母，所以玩游戏还是要小心点...
        case 'w': case 72: case 65:
            change_status(Up);    break;
        case 's': case 80: case 66:
            change_status(Down);  break;
        case 'a': case 75: case 68:
            change_status(Left);  break;
        case 'd': case 77: case 67:
            change_status(Right); break;
        case 'q': case 'Q':
            std::cout << "You quit!" << std::endl;
            return false;
        default: return true;
    }
    switch(check_status())
    {
        case 0: break;
        case 1:
            print_frame();
            std::cout << "You win!!!" << std::endl;
            return false;
        case -1:
            print_frame();
            std::cout << "You fail!" << std::endl;
            return false;
        default:
            print_frame();
            std::cout << "error" << std::endl;
            return false;
    }
    add_new();
    print_frame();
    return true;
}

// 检测状态
// 1: win
// 0: continue
// -1: fail
int basic_2048::check_status(){
    for(int i=0; i<full_size; ++i) if(status[i]==2048) return 1;
    for(int i=0; i<full_size; ++i) if(status[i]==0) return 0;
    for(int i=0; i<size; ++i)
    {
        for(int j=0; j<size-1; ++j)
        {
            if(status[i*size+j] == status[i*size+j+1]) return 0;
        }
    }
    for(int j=0; j<4; ++j)
    {
        for(int i=0; i<size-1; ++i)
        {
            if(status[i*size+j] == status[(i+1)*size+j]) return 0;
        }
    }
    return -1;
}

#endif // JSHL_GAME2048_H

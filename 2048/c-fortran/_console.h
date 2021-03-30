/**description:
 *     为了能够跨平台，这个文件用来定义控制台相关的函数
 */

#pragma once
#ifndef TEMP_CONSOLE_H
#define TEMP_CONSOLE_H
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#elif __linux__
    #include <stdio.h>
    #include <termios.h>
    #include <unistd.h>

int getch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#ifdef __cpluscplus
extern "C" {
#endif

// 设置光标位置
void set_cursor_potition(int x, int y)
{
#ifdef _WIN32
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
#elif __linux__
    printf("\033[%d;%dH", y+1, x+1);
#endif
}

// 清屏
void clean_console(){
#ifdef _WIN32
    system("cls");
#elif __linux__
    printf("\033[2J");
#endif
}

#ifdef __cpluscplus
}
#endif

#endif // TEMP_CONSOLE_H

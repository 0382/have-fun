/**
 * 为了能够同时被 Fortran 调用，为函数都定义了加下划线并且参数都为指针的版本
 * 这个文件专门为 Fortran 准备，和 c++ 版 2048 无关
 */

#include "_console.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// getch
void getch_(int *ch){
    *ch = getch();
}

void getchar_(int *ch){
    *ch = getchar();
}

void set_cursor_potition_(int *x, int *y){
    set_cursor_potition(*x, *y);
}

void clean_console_(){
    clean_console();
}

#ifdef __cplusplus
}
#endif
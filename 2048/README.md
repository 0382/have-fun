# 2048

在学习 Fortran, 正好把原来写的 2048 小游戏改写成 Fortran. 两个版本行为应该是一样的（如果我没写错）, 其中Fortran调用了c语言的部分函数（写在`_console.c`中），主要是用于操作控制台的输入输出。

## 编译

编译 c++ 版本
```bash
make
```
生成文件名`c2048[.exe]`

编译 Fortran 版本
```bash
make f2048
```
生成文件名`f2048[.exe]`
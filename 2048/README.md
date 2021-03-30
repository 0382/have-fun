# 2048

学习一门语言，可以试着实现一个该语言版本的2048。我不知道标准的2048是如何控制按照一定概率使得新出数字是4的，于是我随便写了一个。

## c/fortran版本

两个版本行为应该是一样的（如果我没写错）, 其中Fortran调用了c语言的部分函数（写在`_console.c`中），主要是用于操作控制台的输入输出。

### 编译

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

## julia版本

- `console.jl`，系统相关的控制台输入输出函数。
- `2048.jl`，游戏入口。

使用 `julia 2048.jl`运行即可。

## javascript版本

一个网页文件，示例见[我的github pages](https://0382.github.io/game/2048.html)，毫无美感的垃圾配色，见谅。

## rust版本

不想搞跨平台了，只支持linux。

使用一下命令编译运行。
```bash
cargo build
cargo run
```
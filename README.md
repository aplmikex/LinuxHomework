# 东北大学2021-2022春季学期Linux课程设计 Xsh
* [功能概述](#功能概述)
* [编译方式](#编译方式)
* [作者](#作者)

## 功能概述

 本项目由C++编写的，由cmake构建而成。  
 xsh的实现目的在于动手实现一个简化的bash版本，以培养我对linux和c++底层开发的熟悉程度。

 xsh在程序内部注册了信号处理函数，在当用户输入Ctrl-C的时候，父进程并不会退出，而会将信号传递给子进程，不会同时退出。  
 为仿照bash每次的输入头，xsh在内部获取了用户的用户名、主机名、当前路径，通过c语言的print函数控制颜色，以不可变的形式达成与bash默认输出近似的效果。  
 为实现输入命令行的时候，可以左右键更改已输入的内容，上下键显示过去输入过的内容，xsh参照bash，同样选择选择[libreadline-dev](https://ftp.gnu.org/gnu/readline/)作为命令行输入库。  
 xsh支持单双引号混用分割，但注意，引号内部内容不被认为命令行应该解析的内容。同时xsh也支持非空白符分割，诸如ls>tmp.txt之类。  
 由于时间较为紧迫而作者能力有限，xsh并未使用bash及其他shell使用的bison或lemon等语法分析器，因而xsh对语法的支持较为有限，只支持连续的管道，输入输出重定向以及后台执行命令等。

 xsh还实现了bash的一些内建指令，如cd,help,echo,history等。与此同时,在xsh输入exit或者quit可以退出xsh.

 xsh对课程设计要求的三种限制进行操作，如rm -rf /保护、禁止sudo执行、及限制/opt/robot权限等。  
 由于这三种限制并非通用shell要求，如果有需要，用户可以注释shellclass.cpp中check函数内容，改成return true;即可。

 除此之外，本项目还用qt5以及libqtermwidget5-0制作了一个简易的窗口程序，其默认的shell是上述自己编写的xsh。  
 由于时间不足的原因，此窗口程序极其简陋，但基础功能可以使用。

### 待完善的
 将默认的，复杂臃肿的，支持有限的手写命令行解析器替换成bison或lemon等语法分析器。  
 补全bash支持的内建命令，如fg，bg，if，fi等  
 支持*?等通配符的展开  
 用户输入Ctrl-C时直接换行，而非保留在原地。  
 完善terminal命令行界面程序。

## 编译方式
 注意：xsh是由C++编写的，cmake为默认构建方式的shell程序，请检查是否安装g++，cmake，make等程序。  
 如果环境中没有这些程序，请按照下列代码安装:
 ```
sudo apt-get -y install build-essential git
 ```
 
 安装好编译程序后，请git此代码。
 ```
git clone https://github.com/aplmikex/LinuxHomework.git
 ```

 xsh是一个依赖于libreadline-dev库的应用程序，所以想正常编译xsh的话请按照如下代码安装：
 ```
sudo apt-get -y install libreadline-dev
 ```

 在安装好此库后，就可以正常编译了。
 ```
cd LinuxHomework
mkdir build
cd build
cmake ..
make -j4
 ```
 编译好的程序默认在LinuxHomework/build/bin里


 默认的构建方式只会编译xsh，而安装额外的库，并设定cmake编译选项才会编译terminal界面程序
 如果需要同时编译terminal界面程序，请在命令行输入
 ```
sudo apt-get install -y libqt5charts5-dev qtermwidget5 qt5-default libutf8proc-dev
 ```
 再进行编译
```
cd LinuxHomework
mkdir build
cd build
cmake -DBUILD_TERMINAL=true ..
make -j4
 ```
 ## 作者
    
东北大学机器人科学与工程学院 机器人工程 项志远
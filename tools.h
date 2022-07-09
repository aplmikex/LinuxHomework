#include <bits/stdc++.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <wait.h>
#include <bits/stdc++.h>

//获取用户主机名
std::string getHostName();
//获取用户名
std::string getName();
//获取用户home目录的路径
std::string getHome();
//把用户当前path路径下的home路径改成~
void replace1(std::string &path,std::string &home);
//把用户输入的~替换回常规目录
void replace2(std::string &str,std::string &home);
//去除字符串前后空格
void trim(std::string & str);
//检查该字符串是否为空白符，是返回true
bool testBlank(char a);

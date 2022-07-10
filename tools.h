#include <bits/stdc++.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <wait.h>
#include <bits/stdc++.h>

struct argStru{
    std::string arg;
    bool iterate = true;
};

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
//查找所有该字符的位置
std::vector<int> findAll(std::string &str, char pattern);
//找到该数字的位置
int findPos(std::vector<int> &array,int i);
//将字符串按照引号分割，分割出的结果存在结构体数组
bool quotationSplit(std::string &line,std::vector<argStru> &tmpargs);
//字符串按照pattern字符分割，保留pattern在数组中
std::vector<std::string> split(std::string str, std::string pattern);
//字符串后续分割
void symbolSplit(std::vector<argStru> &tmpargs,std::vector<std::string> &args);
//整理并检查args中出现问题的
bool checkArgs(std::vector<std::string> &args);
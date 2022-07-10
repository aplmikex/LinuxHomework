#include "tools.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <csignal>
#include <sys/types.h>

#define MAX_STORAGE_CMDS 500

class Xsh {
    public:

    bool run = true;
    std::string name;
    std::string hostName;
    std::string path;
    std::string home;

    Xsh();
    void replace1();//把home路径替换到~显示
    void replace2(std::string &str,std::string &home);//把~路径替换回home路径
    void updatePath();//更新输入路径
    void getHistorys();//获取history并输出
    std::string getHead();//获取输入前的标头
    void addHistory(std::string&line);//添加历史命令记录
    bool purchaseString(std::string &line,std::vector<std::string> &args);//分割处理检查字符串
    int execute(std::vector<std::string> &args);//纯粹的开一个子进程来处理
    void simplpeCal(std::vector<std::string> &args);//单命令解析

    private:

    std::vector<std::string> historys;
    int place = 0;
    pid_t sonpid;

    std::string getName();//获取用户主机名
    std::string getHostName();//获取用户名
    std::string getHome();//获取用户home目录的路径

};
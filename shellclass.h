#include "tools.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <csignal>
#include <sys/types.h>
#include <fcntl.h>

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
    // void replace2(std::string &str,std::string &home);//把~路径替换回home路径
    void updatePath();//更新输入路径
    void getHistorys();//获取history并输出
    std::string getHead();//获取输入前的标头
    void addHistory(std::string&line);//添加历史命令记录
    bool purchaseString(std::string &line,std::vector<std::string> &args);//分割处理检查字符串
    void simplpeCal(std::vector<std::string> &args);//单命令解析
    void symbolCal(std::vector<std::string> &args);//符号解析调用
    void backCal(std::vector<std::string> &args);//后台运行
    void normalCal(std::vector<std::string> &args);//前台运行
    void runArgs(std::vector<std::string> &args);//直接调用系统函数执行命令
    int outPutFileRedirect(std::vector<std::string> &args,std::string pattern);//最后执行重定向输出文件
    int inPutFileRedirect(std::vector<std::string> &args,std::string pattern);//最后执行重定向输入文件
    int runLongCmd(std::vector<std::string> &args);
    int changeStdIO(std::vector<std::string> &args);

    private:
    std::vector<std::string> historys;
    int place = 0;
    pid_t sonpid;

    std::string getName();//获取用户主机名
    std::string getHostName();//获取用户名
    std::string getHome();//获取用户home目录的路径

};
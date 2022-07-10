#include "tools.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <csignal>

#define MAX_STORAGE_CMDS 500

pid_t sonpid;

void signalHandler(int signum) {
    kill(sonpid, SIGINT);
}


bool purchaseString(std::string &line,std::vector<std::string> &args){

    std::vector<argStru> tmpargs;
    if(!quotationSplit(line, tmpargs)){
        return false;
    }
    symbolSplit(tmpargs,args);

    // for(int i=0;i<args.size();i++){
    //     std::cout<<args[i]<<std::endl;
    // }
    return true;
}

void getHistorys(std::vector<std::string> &historys, int place){
    int counter = 0;
    for(int i=0;i<MAX_STORAGE_CMDS;i++){
        if(historys[place].empty()){
            if(place==MAX_STORAGE_CMDS-1)place=0;
            else place++;
            continue;
        }
        std::cout<<counter<<":"<<historys[place]<<std::endl;
        counter++;
        if(place==MAX_STORAGE_CMDS-1)place=0;
        else place++;
    }
}



int execute(std::vector<std::string> &args){
    pid_t pid = fork();  //create child process
    int status;
    if(pid<0){
        std::cout<<"子进程创建出错"<<std::endl;
        return -1;
    }else if(pid == 0){
        char** argv = (char **)malloc(sizeof(char*)*(args.size()+1));
        for(int i=0;i<args.size();i++){
            argv[i] = (char*)args[i].c_str();
        }
        argv[args.size()]=NULL;
        if(execvp(argv[0], argv)==-1){
            perror("shell");
        }
        free(argv);
        return 0;
    }else{
        sonpid = pid;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return 0;
    }
}

bool check(std::vector<std::string> &args,std::string &home){
    if(args[0]=="sudo"){
        std::cout<<"请注意版权保护\n";
        return false;
    }
    if(args.size()>=3&&args[0]=="rm"&&(args[1]=="-rf"||args[1]=="-fr")){
        for(int i=2;i<args.size();i++){
            if(args[i]=="/"||args[i]=="/*"){
                std::cout<<"操作危险，不予执行。"<<std::endl;
                return false;
            }
        }
    }
    return true;
}



int main(){
    signal(SIGINT, signalHandler);
    bool run=true;

    std::string name = getName();
    std::string hostName = getHostName();
    std::string path = getcwd(NULL, 512);
    std::string home = getHome();
    replace1(path, home);
    std::vector<std::string> historys(500);
    int place = 0;

    std::string onceline = "chmod 111 /opt/robot/";
    std::vector<std::string> onceargs;
    purchaseString(onceline,onceargs);
    execute(onceargs);
    onceargs.resize(0);


    while(run){
        std::string line;
        std::vector<std::string> args;
        char head[500];
        std::sprintf(head,"\033[0m\033[1;32m%s@%s\033[0m:\033[0m\033[1;34m%s\033[0m$ ",name.c_str(),hostName.c_str(),path.c_str());
        line = readline(head);
        replace2(line, home);
        historys[place] = line;
        add_history(line.c_str());

        if(place==MAX_STORAGE_CMDS-1)place=0;
        else place++;

        
        if(purchaseString(line,args)==false||args.size()==0||check(args,home)==false){
            continue;
        }
        if(args[0]=="exit"||args[0]=="quit"){
            run = false;
            std::cout<<"即将退出！"<<std::endl;
        }else if(args[0]=="history"){
            getHistorys(historys,place);
        }else if(args[0]=="cd"){
            if(args.size()>2){
                std::cout<<"cd的输入数据过多，无法识别"<<std::endl;
            }else{
                if (args.size()==1) {
                    args.push_back(home);
                }
                if(chdir(args[1].c_str())==-1){
                    std::cout<<"输入目录出错"<<std::endl;
                    continue;
                }
                path = getcwd(NULL, 512);
                replace1(path, home);
            }
        }else if(args[0]=="echo"){
            for(int i=1;i<args.size();i++){
                std::cout<<args[i]<<" ";
            }
            std::cout<<std::endl;
        }else if(args[0]=="help"){//help文档最后写
            std::cout<<"本shell由机器人2002项志远完成，为Linu期末课程设计作品，如有遗漏请指正。\n";
            std::cout<<"\t输入help可获得此帮助文档\n";
            std::cout<<"\t输入exit或quit可退出此shell\n";
            std::cout<<"\t输入cd可切换目录\n";
            std::cout<<"\t输入history可查看命令历史输入记录\n";
            std::cout<<"\t输入其他命令可直接执行其他命令\n";
        }else{
            execute(args);
        }

    }
}





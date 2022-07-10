#include "shellclass.h"

void signalHandler(int signum) {
    std::cout<<"接收到中止信号"<<std::endl;
    // kill(sonpid, SIGINT);
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
    signal(SIGINT, signalHandler);//不是很理解，可能是父进程劫持信号了但子进程没劫持导致的
    Xsh xsh;

    std::string onceline = "chmod 111 /opt/robot/";
    std::vector<std::string> onceargs;
    xsh.purchaseString(onceline,onceargs);
    xsh.execute(onceargs);
    onceargs.resize(0);


    while(xsh.run){
        std::string line;
        std::vector<std::string> args;

        std::string head = xsh.getHead();
        line = readline(head.c_str());
        xsh.addHistory(line);
        replace2(line, xsh.home);
        
        if(xsh.purchaseString(line,args)==false||args.size()==0||check(args,xsh.home)==false){
            continue;
        }
        xsh.simplpeCal(args);

    }
}





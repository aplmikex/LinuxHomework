#include "shellclass.h"
#include <unistd.h>

void signalHandler(int signum) {
    std::cout<<"接收到中止信号"<<std::endl;
    // kill(sonpid, SIGINT);
}


int main(){
    signal(SIGINT, signalHandler);//不是很理解，可能是父进程劫持信号了但子进程没劫持导致的
    Xsh xsh;

    std::string onceline = "chmod 555 /opt/robot/";
    std::vector<std::string> onceargs;
    xsh.purchaseString(onceline,onceargs);
    xsh.symbolCal(onceargs);
    std::vector<std::string>().swap(onceargs);


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
        // xsh.simplpeCal(args);
        xsh.symbolCal(args);
    }
}





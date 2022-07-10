#include "shellclass.h"

Xsh::Xsh(){
    this->historys.resize(MAX_STORAGE_CMDS);
    this->updatePath();
}

std::string Xsh::getHostName(){
    char hName[256];
	gethostname(hName, sizeof(hName));
	std::string hostName = hName;
    return hostName;
}
std::string Xsh::getName(){
    uid_t userid;
    struct passwd* pwd;
    userid=getuid();
    pwd=getpwuid(userid);
    std::string name = pwd->pw_name;
    return name;
}
std::string Xsh::getHome(){
    std::string home;
    home = getenv("HOME");
    return home;
}
std::string Xsh::getHead(){
    char head[500];
    std::sprintf(head,"\033[0m\033[1;32m%s@%s\033[0m:\033[0m\033[1;34m%s\033[0m$ ",this->name.c_str(),this->hostName.c_str(),this->path.c_str());
    return std::string(head);
}
void Xsh::replace1(){
    for(int i=0;i<this->home.size();i++){
        if(this->home[i]!=this->path[i]){
            return;
        }
    }
    this->path = this->path.replace(0,home.size(),"~");
}

void Xsh::replace2(std::string &str,std::string &home){//把~替换回常规目录
    while((int)str.find("~")!=-1){
        str = str.replace(str.find("~"),1,home);
    }
}

void Xsh::updatePath(){
    this->name = getName();
    this->hostName = getHostName();
    this->path = getcwd(NULL, 512);
    this->home = getHome();
    this->replace1();
}

void Xsh::getHistorys(){
    int counter = 0;
    int place = this->place;
    for(int i=0;i<MAX_STORAGE_CMDS;i++){
        if(this->historys[place].empty()){
            if(place==MAX_STORAGE_CMDS-1)place=0;
            else place++;
            continue;
        }
        std::cout<<counter<<":"<<this->historys[place]<<std::endl;
        counter++;
        if(place==MAX_STORAGE_CMDS-1)place=0;
        else place++;
    }
}

void Xsh::addHistory(std::string &line){
    this->historys[place] = line;
    add_history(line.c_str());

    if(this->place==MAX_STORAGE_CMDS-1)this->place=0;
    else this->place++;

}

int Xsh::execute(std::vector<std::string> &args){
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
        this->sonpid = pid;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return 0;
    }
}

bool Xsh::purchaseString(std::string &line,std::vector<std::string> &args){

    std::vector<argStru> tmpargs;
    if(!quotationSplit(line, tmpargs)){
        std::cout<<"引号使用出错！"<<std::endl;
        return false;
    }
    symbolSplit(tmpargs,args);
    if(!checkArgs(args)){
        std::cout<<"不支持此分隔符！"<<std::endl;
        return false;
    }
    // for(int i=0;i<args.size();i++){
    //     std::cout<<args[i]<<std::endl;
    // }
    return true;
}

void Xsh::simplpeCal(std::vector<std::string> &args){
        if(args[0]=="exit"||args[0]=="quit"){
            this->run = false;
            std::cout<<"即将退出！"<<std::endl;
        }else if(args[0]=="history"){
            this->getHistorys();
        }else if(args[0]=="cd"){
            if(args.size()>2){
                std::cout<<"cd的输入数据过多，无法识别"<<std::endl;
            }else{
                if (args.size()==1) {
                    args.push_back(this->home);
                }
                if(chdir(args[1].c_str())==-1){
                    std::cout<<"输入目录出错"<<std::endl;
                    return;
                }
                this->updatePath();
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
            this->execute(args);
        }
}
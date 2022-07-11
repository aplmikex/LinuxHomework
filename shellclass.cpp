#include "shellclass.h"
#include <unistd.h>
#include <vector>

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

// void Xsh::replace2(std::string &str,std::string &home){//把~替换回常规目录
//     while((int)str.find("~")!=-1){
//         str = str.replace(str.find("~"),1,home);
//     }
// }

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


void Xsh::runArgs(std::vector<std::string> &args){
    if(args[0]=="ls"||args[0]=="grep"){
        args.push_back("--color=auto");
    }
    char** argv = (char **)malloc(sizeof(char*)*(args.size()+1));
    for(int i=0;i<args.size();i++){
        argv[i] = (char*)args[i].c_str();
    }
    argv[args.size()]=NULL;
    if(execvp(argv[0], argv)==-1){
        perror("xsh");
    }
    free(argv);
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
    if(args[0]=="history"){
        this->getHistorys();
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
        this->runArgs(args);
    }
}

//先处理&符号，代表前台后台运行
//然后剩下的是基本操作，顺序执行即可

void Xsh::symbolCal(std::vector<std::string> &args){
    args.push_back("&");
    int last = 0;
    for(int i=0;i<args.size();i++){
        if(args[i]=="&"){
            std::vector<std::string> part(args.begin()+last,args.begin()+i);
            if(i!=args.size()-1)
                this->backCal(part);//后台运行
            else
                this->normalCal(part);//前台运行
            last = i+1;
            i+=1;
        }
    }
    args.pop_back();
}


int Xsh::outPutFileRedirect(std::vector<std::string> &args,std::string pattern){//在该命令只能有一个重定向符号
    auto Pos = findpos(args,pattern)[0];


    int oflag = pattern==">"?O_WRONLY|O_CREAT|O_TRUNC:O_WRONLY|O_APPEND|O_CREAT|O_APPEND;

    int fd= open(args[Pos+1].c_str(), oflag, 0666);
    // 文件打开失败
    if (fd < 0) {
        std::cout<<"文件打开失败"<<std::endl;
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);  
    std::vector<std::string> part;
    for(int i=0;i<Pos;i++){
        part.push_back(args[i]);
    }


    this->runArgs(part);
    if (fd != STDOUT_FILENO) {
        close(fd);
    }
    // 代码健壮性: 如果子进程未被成功执行, 则报错
    printf("%s: 命令输入错误\n", args[0].c_str());
    // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
    exit(1);
}


int Xsh::inPutFileRedirect(std::vector<std::string> &args,std::string pattern){//在该命令只能有一个重定向符号
    auto Pos = findpos(args,pattern)[0];
    int oflag = O_RDONLY;

    int fd= open(args[Pos+1].c_str(), oflag, 0666);
    // 文件打开失败
    if (fd < 0) {
        exit(1);
    }
    dup2(fd, STDIN_FILENO);  
    std::vector<std::string> part(args.begin(),args.begin()+Pos);
    this->runArgs(part);
    if (fd != STDIN_FILENO) {
        close(fd);
    }
    // 代码健壮性: 如果子进程未被成功执行, 则报错
    printf("%s: 命令输入错误\n", args[0].c_str());
    // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
    exit(1);

}

int Xsh::thePipeline(std::vector<std::string> &args){
    if(args[args.size()-1]=="|"||args[0]=="|"){
        std::cout<<"命令输入出错，无法执行"<<std::endl;
        return -1;
    }
    int status;

    args.push_back("|");
    auto pos = findpos(args, "|");

    std::vector<std::vector<std::string>> parts;
    int last = 0;
    for(int i=0;i<pos.size();i++){
        std::vector<std::string> part(args.begin()+last,args.begin()+pos[i]);
        last = pos[i] + 1;
        parts.push_back(part);
    }
    std::string tmpfile = "/tmp/temp.txt"; //共享文件，取巧做法


    for(int i=0;i<parts.size();i++){
        int pid = fork();
        if(pid<0){
            std::cout<<"fork子进程未成功"<<std::endl;
            return -1;
        }else if(pid == 0){
            int oflag = O_WRONLY|O_CREAT|O_TRUNC;
            if(i!=0){//在非第一个命令的情况下，标准输入应该重定向到文件
                close(STDIN_FILENO);
                int fd = open(tmpfile.c_str(), O_RDONLY); //输入重定向
                remove(tmpfile.c_str());
            }
            if(i!=parts.size()-1){//在非最后一个命令的情况下，标准输出重定向到文件
                close(STDOUT_FILENO);
                int fd = open(tmpfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
            }

            this->runArgs(parts[i]);
            exit(0);

        }else{
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

}


void Xsh::backCal(std::vector<std::string> &args){//不完全
    pid_t pid = fork();
    if(pid<0){
        std::cout<<"出错了"<<std::endl;
        return;
    }else if(pid==0){
        freopen( "/dev/null", "w", stdout );
        freopen( "/dev/null", "r", stdin ); 
        signal(SIGCHLD,SIG_IGN);

        auto orPos = findpos(args,"|");
        auto bigPos = findpos(args,">");
        auto bigbigPos = findpos(args,">>");
        auto smallPos = findpos(args,"<");
        if(!orPos.empty()){
            if(bigPos.empty() && smallPos.empty() && bigbigPos.empty()){
                thePipeline(args);
            }else{
                return;
            }
        }else if(bigPos.size()+smallPos.size()+bigbigPos.size()==1){
            if(bigPos.size()==1){
                this->outPutFileRedirect(args, ">");
            }else if(smallPos.size()==1){
                this->inPutFileRedirect(args, "<");
            }else{
                this->outPutFileRedirect(args, ">>");
            }
        }else if(bigbigPos.empty()&&orPos.empty()&&bigPos.empty()&&smallPos.empty()){
            this->simplpeCal(args);
        }
        exit(0);
    }else{
        return;
    }
}

void Xsh::normalCal(std::vector<std::string> &args){ //不完全
    if(args[0]=="exit"||args[0]=="quit"){
        std::cout<<"即将退出"<<std::endl;
        this->run=false;
        return;
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
        return;
    }
    pid_t pid = fork();
    if(pid<0){
        std::cout<<"出错了"<<std::endl;
        return;
    }else if(pid==0){
        auto orPos = findpos(args,"|");
        auto bigPos = findpos(args,">");
        auto bigbigPos = findpos(args,">>");
        auto smallPos = findpos(args,"<");
        if(!orPos.empty()){
            if(bigPos.empty() && smallPos.empty() && bigbigPos.empty()){
                thePipeline(args);
            }else{
                return;
            }
        }else if(bigPos.size()+smallPos.size()+bigbigPos.size()==1){
            if(bigPos.size()==1){
                this->outPutFileRedirect(args, ">");
            }else if(smallPos.size()==1){
                this->inPutFileRedirect(args, "<");
            }else{
                this->outPutFileRedirect(args, ">>");
            }
        }else if(bigbigPos.empty()&&orPos.empty()&&bigPos.empty()&&smallPos.empty()){
            this->simplpeCal(args);
        }
        exit(0);
    }else{
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}
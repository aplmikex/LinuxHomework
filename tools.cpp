#include "tools.h"

std::string getHostName(){
    char hName[256];
	gethostname(hName, sizeof(hName));
	std::string hostName = hName;
    return hostName;
}
std::string getName(){
    uid_t userid;
    struct passwd* pwd;
    userid=getuid();
    pwd=getpwuid(userid);
    std::string name = pwd->pw_name;
    return name;
}
std::string getHome(){
    std::string home;
    home = getenv("HOME");
    return home;
}
void replace1(std::string &path,std::string &home){
    for(int i=0;i<home.size();i++){
        if(home[i]!=path[i]){
            return;
        }
    }
    path = path.replace(0,home.size(),"~");
}

void replace2(std::string &str,std::string &home){//把~替换回常规目录
    while((int)str.find("~")!=-1){
        str = str.replace(str.find("~"),1,home);
    }
}
void trim(std::string & str){
    std::string blanks("\f\v\r\t\n ");
    str.erase(0,str.find_first_not_of(blanks));
    str.erase(str.find_last_not_of(blanks) + 1);
}
bool testBlank(char a){
    if(a==' '||a=='\t'||a=='\n'||a=='\r'){
        return true;
    }
    return false;
}
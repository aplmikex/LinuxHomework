#include "tools.h"
#include <string>



bool check(std::vector<std::string> &args,std::string &home){
    for(int i=0;i<args.size();i++){
        if(args[i]=="sudo"){
            std::cout<<"Copyright © 2022, NEU. All Rights Reserved. \n";
            return false;
        }
    }
    for(int i=0;i<args.size();i++){
        if(args.size()-i>=3&&args[i]=="rm"&&(args[i+1]=="-rf"||args[i+1]=="-fr")){
            for(int j=i+2;j<args.size();i++){
                if(args[j]=="/"){
                    std::cout<<"操作危险，不予执行。"<<std::endl;
                    return false;
                }
            }
        }

    }
    return true;
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
    if(a==' '||a=='\t'||a=='\n'||a=='\r'||a=='\0'){
        return true;
    }
    return false;
}
std::vector<int> findAll(std::string &str, char pattern) {
    
    std::vector<int> result;
    for(int i=0;i<str.size();i++){
        if(str[i]==pattern){
            result.push_back(i);
        }
    }
    return result;
}

bool checkArgs(std::vector<std::string> &args){
    std::vector<std::string> tmpargs;
    for(int i=0;i<args.size();i++){
        if(args[i]!="&"&&args[i]!="|"&&args[i]!=">"){
            tmpargs.push_back(args[i]);
        }else{
            if(tmpargs.empty()){
                tmpargs.push_back(args[i]);
            }else{
                std::string last = tmpargs[tmpargs.size()-1];
                char c = last[last.size()-1];

                if(c!='&'&&c!='|'&&c!='>')
                    tmpargs.push_back(args[i]);
                else{
                    tmpargs.pop_back();
                    tmpargs.push_back(last+args[i]);
                }
            }
        }
    }
    args.swap(tmpargs);
    std::vector<std::string>().swap(tmpargs);//置空
    for(int i=0;i<args.size();i++){
        char c = args[i][0];
        if(c!='&'&&c!='|'&&c!='>'){
            continue;
        }else{
            if(args[i]!="&&"&&args[i]!="||"&&args[i]!="&"&&args[i]!="|"&&args[i]!=">"&&args[i]!=">>"&&args[i]!="<"){
                return false;
            }
        }
    }
    return true;
}

std::vector<int> findpos(std::vector<std::string> &args,std::string pattern){
    std::vector<int> pos;
    for(int i=0;i<args.size();i++){
        if(args[i]==pattern){
            pos.push_back(i);
        }
    }
    return pos;
}



int findPos(std::vector<int> &array,int i) {
    for(int j=0;j<array.size();j++){
        if(array[j]==i){
            return j;
        }
    }
    return -1;
}

bool quotationSplit(std::string &line,std::vector<argStru> &tmpargs){
    auto simplePos = findAll(line,'\'');
    auto doublePos = findAll(line,'\"');

    int i=0;
    int lastI = 0;
    while(true){

        int simpleI = findPos(simplePos, i);
        int doubleI = findPos(doublePos, i);

        if(simpleI!=-1){
            argStru tmparg;
            tmparg.arg = line.substr(lastI,i-lastI);
            tmpargs.push_back(tmparg);
            if(simpleI+1>=simplePos.size()){
                return false;
            }
            argStru tmparg1;
            tmparg1.arg = line.substr(i+1,simplePos[simpleI+1]-i-1);
            tmparg1.iterate = false;
            tmpargs.push_back(tmparg1);
            lastI = simplePos[simpleI+1]+1;
            i = lastI;
        }else if(doubleI!=-1){
            argStru tmparg;
            tmparg.arg = line.substr(lastI,i-lastI);
            tmpargs.push_back(tmparg);
            if(doubleI+1>=doublePos.size()){
                return false;
            }
            argStru tmparg1;
            tmparg1.arg = line.substr(i+1,doublePos[doubleI+1]-i-1);
            tmparg1.iterate = false;
            tmpargs.push_back(tmparg1);
            lastI = doublePos[doubleI+1]+1;
            i = lastI;
        }else
            i++;
        if(i>=line.size()){
            argStru arg;
            
            if(lastI<=line.size()-1&&lastI!=0){
                arg.arg = line.substr(lastI+1);
                tmpargs.push_back(arg);
            }else if(lastI==0){
                arg.arg = line.substr(lastI);
                tmpargs.push_back(arg);
            }
            break;
        }
    }
    return true;
}

void symbolSplit(std::vector<argStru> &tmpargs,std::vector<std::string> &args){
    
    for(int i=0;i<tmpargs.size();i++){
        if(tmpargs[i].iterate){
            std::vector<std::string> tmp;
            std::vector<std::string> strs = split(tmpargs[i].arg, "&");
            for(int j=0;j<strs.size();j++){
                std::vector<std::string> strs1 = split(strs[j], "|");
                tmp.insert(tmp.end(),strs1.begin(),strs1.end());
            }
            strs.swap(tmp);
            std::vector<std::string>().swap(tmp);//tmp置空，strs为分割完的

            for(int j=0;j<strs.size();j++){
                std::vector<std::string> strs1 = split(strs[j], ">");
                tmp.insert(tmp.end(),strs1.begin(),strs1.end());
            }
            strs.swap(tmp);
            std::vector<std::string>().swap(tmp);//tmp置空，strs为分割完的

            for(int j=0;j<strs.size();j++){
                std::vector<std::string> strs1;
                std::istringstream buffer(strs[j]);
                std::copy(std::istream_iterator<std::string>(buffer), std::istream_iterator<std::string>(), std::back_inserter(strs1));
                tmp.insert(tmp.end(),strs1.begin(),strs1.end());
            }
            strs.swap(tmp);
            std::vector<std::string>().swap(tmp);//tmp置空，strs为分割完的

            args.insert(args.end(),strs.begin(),strs.end());

        }else{
            args.push_back(tmpargs[i].arg);
        }
    }
}
std::vector<std::string> split(std::string str, std::string pattern) {
   
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    { 
   
        pos = str.find(pattern, i);
        if (pos < size)
        { 
   
            std::string s = str.substr(i, pos - i);
            if(s!="")
                result.push_back(s);
            result.push_back(pattern);
            i = pos + pattern.size() - 1;
        }
    }
    result.pop_back();
    // for(int i=0;i<result.size();i++){
    //     std::cout<<result[i]<<std::endl;
    // }
    return result;
}
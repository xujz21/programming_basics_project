#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"
std::string s;
struct BashCmd
{
    std::string CmdName;
    void (*fx)(int,char**){};
}CmdList[1000];
int cmdcnt=9;

inline void Addfunction()
{
    CmdList[0].CmdName="exit";
    CmdList[1].CmdName="diff";
    CmdList[1].fx=doDiff;
    CmdList[2].CmdName="grep";
    CmdList[2].fx=doGrep;
    CmdList[3].CmdName="tee";
    CmdList[3].fx=doTee;
    CmdList[4].CmdName="cat";
    CmdList[4].fx=doCat;
    CmdList[5].CmdName="cp";
    CmdList[5].fx=doCp;
    CmdList[6].CmdName="cd";
    CmdList[6].fx=doCd;
    CmdList[7].CmdName="pwd";
    CmdList[7].fx=doPwd;
    CmdList[8].CmdName="echo";
    CmdList[8].fx=doEcho;
}
inline void not_impl(std::string &token)
{
    std::cerr<<"Command '"<<token<<"' not found"<<std::endl;
}
inline void Analyze(std::vector<std::string> tokens)
{
    char **args;
    int n=(int)tokens.size()-1;
    args=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++)
    {
        args[i]=(char*)malloc((tokens[i+1].size()+1)*sizeof(char));
        strcpy(args[i],tokens[i+1].c_str());
    }
    bool called=false;
    for(int i=0;i<cmdcnt;i++)
    {
        if(tokens[0]==CmdList[i].CmdName)
        {
            if(tokens[0]=="echo")
            {
                CmdList[i].fx(n, args);
            }
            if(tokens[0]!="echo")
            {
                memset(gTerm.strout,'\0',sizeof(gTerm.strout));
            }
            called=true;
            break;
        }
    }
    //for(int i=0;i<n;i++)
      //  free(args[i]);
    //free(args);
    if(!called)
    {
        not_impl(tokens[0]);
    }
}
inline void Parse()
{
    std::string str;
    std::getline(std::cin,str);
    int nn=(int)str.size();
    for(int i=nn-1;i>=0;i--)
    {
        if(str[i]==' ')
        str.erase(i,1);
        else break;
    }
    std::stringstream ss(str);
    std::vector<std::string> tokens;
    std::string tempstring;
    bool cmplx=false;
    while(!ss.eof())
    {
        ss>>tempstring;
        if(tempstring!="|")
        {
            tokens.push_back(tempstring);         
        }
        else
        {
            Analyze(tokens);
            tokens.clear();
            memset(gTerm.strin,0,sizeof gTerm.strin);
            strcpy(gTerm.strin,gTerm.strout);
            cmplx=true;
        }
    }
    if(!cmplx&&tokens.size()==1&&tokens[0]=="exit")
    {
        exit(0);
    }
    Analyze(tokens);
    tokens.clear();
    std::cout<<gTerm.strout;
    memset(gTerm.strout,'\0',sizeof(gTerm.strout));
}

int main(int argc,char **argv)
{
    Addfunction();
    std::cout<<"Machine Name: ";
    std::cin>>gTerm.mach;
    std::cout<<"Root Directory: ";
    std::cin>>gTerm.root;
    std::cout<<"Login: ";
    std::cin>>gTerm.user;
    std::getline(std::cin,s);
    while(true)
    {
        std::cout<<"\e[92;1m"<<gTerm.user<<"@"<<gTerm.mach<<"\e[0m:\e[94;1m"<<gTerm.wdir<<"$ \e[0m";
        Parse();
    }
    return 0;
}


#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "map"
#include "cstdio"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"

std::string s;
std::map <std::string,void(*)(int,char**)> cmdlist;
inline void Addfunction()
{
    cmdlist["diff"] = doDiff;
    cmdlist["grep"] = doGrep;
    cmdlist["tee" ] = doTee;
    cmdlist["cat" ] = doCat;
    cmdlist["cp"  ] = doCp;
    cmdlist["cd"  ] = doCd;
    cmdlist["pwd" ] = doPwd;
    cmdlist["echo"] = doEcho;
    cmdlist["exit"] = doExit;
}
inline void not_impl(std::string &token)
{
    std::cerr<<"Command '"<<token<<"' not found"<<std::endl;
}

std::vector<std::string> vec_argv;
std::vector<std::vector<std::string> > vec_pipeline;
std::vector<std::vector<std::vector<std::string> > > commands;
std::string str;
char c;
inline void clear_all()
{
    vec_argv.clear();
    vec_pipeline.clear();
    commands.clear();
    str.clear();
    c=0;
}
inline void push_word()
{
    if(str.size()>0)
        vec_argv.push_back(str);
    str="";
}
inline void push_pipeline()
{
    push_word();
    if(!vec_argv.empty())
        vec_pipeline.push_back(vec_argv);
    vec_argv.clear();
}
inline void push_command()
{
    push_pipeline();
    if(!vec_pipeline.empty())
        commands.push_back(vec_pipeline);
    vec_pipeline.clear();
}
inline void tokenize()
{
    while(c=getchar())
    {
        if(c=='\n')
        {
            if(vec_argv.empty()&&!vec_pipeline.empty())
            {
                std::cout<<"> ";
                continue;
            }
            else break;
        }
        else if(c=='\'')
        {
            while(true)
            {
                c=getchar();
                if(c=='\'')break;
                if(c=='\n')std::cout<<"> ";
                str+=c;
            }
        }
        else if(c=='\"')
        {
            while(true)
            {
                c=getchar();
                if(c=='\"')break;
                if(c=='\\')
                {
                    c=getchar();
                    if(!(c=='\n'||c=='\"'||c=='`'||c=='$'))
                    {
                        str+='\\';
                    }
                }

                if(c=='\n')std::cout<<"> ";
                str+=c;
            }
        }
        else if(c=='\\')
        {
            c=getchar();
            str+=c;
        }
        else if(c=='\t'||c==' ')
        {
            push_word();
        }
        else if(c==';')
        {
            push_command();
        }
        else if(c=='|')
        {
            push_pipeline();
        }
        else
        {
            str+=c;
        }
    }
    push_command();
}
void parse(int pipe_idx,int arglist_idx)
{
    char **args;
    int n=(int)commands[pipe_idx][arglist_idx].size();
    args=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++)
    {
        args[i]=(char*)malloc((commands[pipe_idx][arglist_idx][i].size()+1)*sizeof(char));
        strcpy(args[i],commands[pipe_idx][arglist_idx][i].c_str());
    }
    auto _tmpcmd=cmdlist.find(commands[pipe_idx][arglist_idx][0]);
    if(_tmpcmd!=cmdlist.end())
    {
        _tmpcmd->second(n, args);
    }
    else
    {
        not_impl(commands[pipe_idx][arglist_idx][0]);
    }
    for(int i=0;i<n;i++)
      free(args[i]);
    free(args);
}
inline void handle_pipeline(int pipe_idx)
{
    int w=(int)commands[pipe_idx].size();
    if(w!=1)gTerm.multistages=true;
    for(int i=0;i<w;i++)
    {
        memset(gTerm.strout,0,sizeof(gTerm.strout));
        parse(pipe_idx,i);
        strcpy(gTerm.strin,gTerm.strout);
    }
    gTerm.multistages=false;
    std::cout<<gTerm.strout;
}
inline void evaluate()
{
    tokenize();
    int w=(int)commands.size();
    for(int i=0;i<w;i++)
        handle_pipeline(i);
    clear_all();
}
inline void Initialize()
{
    std::cout<<"Machine Name: ";
    std::cin>>gTerm.mach;
    std::cout<<"Root Directory: ";
    std::cin>>gTerm.root;
    std::cout<<"Login: ";
    std::cin>>gTerm.user;
    std::getline(std::cin,s);
}
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    Addfunction();
    Initialize();
    while(true)
    {
        std::cout<<"\e[92;1m"<<gTerm.user<<"@"<<gTerm.mach<<"\e[0m:\e[94;1m"<<gTerm.wdir<<"$ \e[0m";
        evaluate();
    }
    return 0;

}


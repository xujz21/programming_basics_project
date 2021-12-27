#include <iostream>
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
char c;
std::string str,tmp_str;
std::vector<std::string> vec_argv;
std::vector<std::vector<std::string> > vec_pipeline;
std::vector<std::vector<std::vector<std::string> > > commands;

std::map <std::string,void(*)(int,char**)> cmdlist;
std::map <std::string,std::string> varlist;
inline void AddFunc();
inline void AddVar();
inline void clear_all();
inline int evaluate();
inline void handle_pipeline(int);
inline void Initialize();
inline void parse(int,int);
inline int push_command();
inline int push_pipeline();
inline void push_word();
inline int tokenize();

int main(int argc,char **argv)
{
    AddFunc();
    Initialize();
    AddVar();
    while(true)
    {
        std::cout<<"\e[92;1m"<<gTerm.user<<"@"<<gTerm.mach<<"\e[0m:\e[94;1m"<<gTerm.wdir<<"\e[0m"<<"$ ";
        evaluate();
    }
    return 0;

}

inline void AddFunc()
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
inline void AddVar()
{
    varlist["PS1"]="";
    varlist["PS2"]="> ";
    varlist["PS4"]="+ ";
    //The set of characters that are treated as word
    //separators when performing word completion.
    varlist["COMP_WORDBREAKS"]="\"\';|";
    //varlist["EPOCHSECONDS"]= std::to_string((long long)(time(NULL)/3600));
    varlist["HOSTNAME"]=gTerm.mach;
    varlist["PWD"]=gTerm.wdir;
    //varlist["RANDOM"]=std::to_string(rand());
}
inline void clear_all()
{
    vec_argv.clear();
    vec_pipeline.clear();
    commands.clear();
    str.clear();
    c=0;
}
inline int evaluate()
{
    if(!tokenize())
    {
        clear_all();
        return 0;
    }
    int w=(int)commands.size();
    for(int i=0;i<w;i++)
        handle_pipeline(i);
    clear_all();
    return 1;
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
    memset(gTerm.strout,0,sizeof (gTerm.strout));
    memset(gTerm.strin,0,sizeof(gTerm.strin));
}
inline void Initialize()
{
    gTerm.wdir[0]='/';
    std::cout<<"Machine Name: ";
    std::getline(std::cin,tmp_str);
    strcpy(gTerm.mach,tmp_str.c_str());
    std::cout<<"Root Directory: ";
    std::getline(std::cin,tmp_str);
    strcpy(gTerm.root,tmp_str.c_str());
    std::cout<<"Login: ";
    std::getline(std::cin,tmp_str);
    strcpy(gTerm.user,tmp_str.c_str());
}
inline void not_impl(std::string &token)
{
    std::cerr<<"Command '"<<token<<"' not found"<<std::endl;
}
inline void sytxerr(char token)
{
    std::cerr<<"-bash: syntax error near unexpected token `"<<token<<"'"<<std::endl;
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
inline int push_command()
{
    push_pipeline();
    if(!vec_pipeline.empty())
    {
        commands.push_back(vec_pipeline);
        vec_pipeline.clear();
        return 1;
    }
    else
    {
        return 0;
    }
}
inline int push_pipeline()
{
    push_word();
    if(!vec_argv.empty())
        vec_pipeline.push_back(vec_argv);
    else return 0;
    vec_argv.clear();
    return 1;
}
inline void push_word()
{
    if(str.size()>0)
        vec_argv.push_back(str);
    str="";
}
inline int tokenize()
{
    while(c=getchar())
    {
        if(c=='\n')
        {
            if(!push_pipeline())
            {
                std::cout<<varlist["PS2"];
                continue;
            }
            else break;
        }
        else if(c=='#')
        {
            do
            {
                c=getchar();
            }while (c!='\n');
            break;
        }
        else if(c=='\'')
        {
            while(true)
            {
                c=getchar();
                if(c=='\'')break;
                if(c=='\n')std::cout<<varlist["PS2"];
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

                if(c=='\n')std::cout<<varlist["PS2"];
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
            if(!push_command())
            {
                sytxerr(c);
                return 0;
            }
            handle_pipeline(0);
            clear_all();
        }
        else if(c=='|')
        {
            if(!push_pipeline())
            {
                sytxerr(c);
                return 0;
            }
        }
        else
        {
            str+=c;
        }
    }
    push_command();
    return true;
}

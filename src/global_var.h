#pragma once
//#define red "\e[91;1m"
#define red "\033[1m\033[31m"
#define green "\033[1m\e[92;1m"
#define blue "\033[1m\e[94;1m"
#define purple "\033[35m"
#define white "\e[0m"
#define MAXLINE 10000
#define MAXFILE 10000000
#define MAX_LINE_NUM 1000       // 文件最大行数
#define MAX_CHAR_PER_LINE 1000  // 单行最大字符数
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
struct Terminal
{
    char user[MAXLINE];
    char mach[MAXLINE];
    char root[MAXLINE];
    char wdir[MAXLINE];
    char strin[MAXFILE];
    char strout[MAXFILE];
    bool multistages;
    int EXIT_CODE=0;
};
Terminal gTerm;
struct _Document
{
    int line_count;
    char content[MAX_LINE_NUM][MAX_CHAR_PER_LINE];
};
inline void cinStrout(char* src){
    strcat(gTerm.strout, src);
}
inline void cinStrout(const char* src){
    strcat(gTerm.strout, src);
}
inline void cinStrout(char c){
    size_t len = strlen(gTerm.strout);
    gTerm.strout[len] = c;
    gTerm.strout[len+1] = '\0';
}
/**
 * Convert [DIR] or [FILE] to its physical location in the storage device.
 * \Arguments:
 * char* path:      the file name with given relative directory or absolute directory
 * \Returns:
 * char*    :       the physical location
 */
inline char* Physical_Path(char* path)
{
    if(strcmp(path,"-")==0)
        return (char*)"-";
    char *return_value=new char[MAXLINE];
    if(path[0]!='/')
    {
        strcat(return_value,gTerm.wdir);
        strcat(return_value,"/");
    }
    strcat(return_value,path);
    std::stack<std::string> st;
    std::string dir;

    size_t len_A = strlen(return_value);
    char* res=new char[len_A];
    res[0]='/';
    for (int i = 0; i < len_A; i++) {
        dir.clear();
        while (return_value[i] == '/')i++;
        while (i < len_A && return_value[i] != '/') {
            dir.push_back(return_value[i]);
            i++;
        }
        if (dir == "..") {
            if (!st.empty())
                st.pop();
        }
        else if (dir == ".")continue;
        else if (dir.length() != 0)
            st.push(dir);
    }

    std::stack<std::string> st1;
    while (!st.empty()) {
        st1.push(st.top());
        st.pop();
    }
    // the st1 will contain the actual res.
    while (!st1.empty()) {
        strcat(res,st1.top().c_str());
        if (st1.size() != 1)
            strcat(res,"/");
        st1.pop();
    }
    char *tmp1=new char[len_A];
    strcpy(tmp1,gTerm.strin);
    strcat(tmp1,res);
    return tmp1;
}
inline char modified_getc(bool mode,FILE* fd=nullptr,int* pos=nullptr)
{
    if(mode)return gTerm.strin[(*pos)++];
    else    return fgetc(fd);

}
_Document Handle_File_Input(char *path,bool end_with_lf=true,char *funcname=NULL)
{
    _Document Input_Document{};
    int *itpos=new int(0);
    FILE* doc;
    char* abs_path= Physical_Path(path);
    if(strcmp(abs_path,"-") != 0){
        doc= fopen(abs_path,"r");
        if(doc==nullptr)
        {
            std::cerr<<(std::string)funcname<<": "<<(std::string)path<<": No such file or directory"<<std::endl;
            return Input_Document;
        }
    }
    int LINE_CNT=0;
    char c=40;
    while(c)
    {
        c=modified_getc(strcmp(abs_path,"-")==0,doc,itpos);
        int LINE_IDX=0;
        while(c!='\n'&&c)
        {
            Input_Document.content[LINE_CNT][LINE_IDX++]=c;
            c= modified_getc(strcmp(abs_path,"-")==0,doc,itpos);
        }
        if(c=='\n'&&end_with_lf)
        {
            Input_Document.content[LINE_CNT][LINE_IDX++]='\n';
        }
        LINE_CNT++;
    }
    Input_Document.line_count=LINE_CNT;
    if(strcmp(abs_path,"-") != 0)
        fclose(doc);
    return Input_Document;
}
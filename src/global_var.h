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
    char *return_value=new char[MAXLINE];
    strcat(return_value,gTerm.root);
    if(path[0]!='/')
    {
        strcat(return_value,gTerm.wdir);
        strcat(return_value,"/");
    }
    strcat(return_value,path);
    return return_value;
}
inline char _modified_getc(bool mode,FILE* fd=NULL,int* pos=NULL)
{
    if(mode)return gTerm.strin[(*pos)++];
    else    return fgetc(fd);

}
_Document Handle_File_Input(char *abs_path,bool end_with_lf=true)
{
    _Document Input_Document{};
    int *itpos=new int(0);
    FILE* doc;
    if(strcmp(abs_path,"-") != 0)
        doc= fopen(abs_path,"r");
    int LINE_CNT=0;
    char c=40;
    while(c)
    {
        c=_modified_getc(strcmp(abs_path,"-")==0,doc,itpos);
        int LINE_IDX=0;
        while(c!='\n'&&c)
        {
            Input_Document.content[LINE_CNT][LINE_IDX++]=c;
            c= _modified_getc(strcmp(abs_path,"-")==0,doc,itpos);
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
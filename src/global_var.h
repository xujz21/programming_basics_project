#pragma once
//#define red "\e[91;1m"
#define red "\033[1m\033[31m"
#define green "\033[1m\e[92;1m"
#define blue "\033[1m\e[94;1m"
#define white "\e[0m"
#define MAXLINE 10000
#define MAXFILE 10000000
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
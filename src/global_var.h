#pragma once
#define MAXLINE 10000
#define MAXFILE 10000000
#include <string>
#include <fstream>
#include <iostream>
struct Terminal
{
    char user[MAXLINE];
    char mach[MAXLINE];
    char root[MAXLINE];
    char wdir[MAXLINE];
    char strin[MAXFILE];
    char strout[MAXFILE];
    bool multistages;
};
Terminal gTerm;

inline void cinStrout(char* src){
    strcat(gTerm.strout, src);
}
inline void cinStrout(const char* src){
    strcat(gTerm.strout, src);
}
inline void cinStrout(char c){
    int len = strlen(gTerm.strout);
    gTerm.strout[len] = c;
    gTerm.strout[len+1] = '\0';
}
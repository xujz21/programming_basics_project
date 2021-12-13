#pragma once
#define MAXLINE 10000
#define MAXFILE 10000000
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
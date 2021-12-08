#include <string>
#include <cstdlib>
#include <cstring>
#include "global_var.h"
void doEcho(int argc, char **argv)
{
    std::string _output;
    if((argc==2)&&(strcmp(argv[1],"--help")==0))
    {
        _output="Usage: echo [SHORT-OPTION]... [STRING]...\n  or:  echo LONG-OPTION\nEcho the STRING(s) to standard output.\n\n  -n             do not output the trailing newline\n  -e             enable interpretation of backslash escapes\n  -E             disable interpretation of backslash escapes (default)\n      --help     display this help and exit\n      --version  output version information and exit\n\nIf -e is in effect, the following sequences are recognized:\n\n  \\\\      backslash\n  \\a      alert (BEL)\n  \\b      backspace\n  \\c      produce no further output\n  \\e      escape\n  \\f      form feed\n  \\n      new line\n  \\r      carriage return\n  \\t      horizontal tab\n  \\v      vertical tab\n  \\0NNN   byte with octal value NNN (1 to 3 digits)\n  \\xHH    byte with hexadecimal value HH (1 to 2 digits)\n\nNOTE: your shell may have its own version of echo, which usually supersedes\nthe version described here.  Please refer to your shell's documentation\nfor details about the options it supports.\n\nGNU coreutils online help: <https://www.gnu.org/software/coreutils/>\nReport echo translation bugs to <https://translationproject.org/team/>\nFull documentation at: <https://www.gnu.org/software/coreutils/echo>\nor available locally via: info '(coreutils) echo invocation'\n";
    }
    else if(argc>=2)
    {
        int i=1;
        bool _newline=true;
        /*
        if(strcmp(argv[0],"-n")==0)
        {
            _newline=false;
            i++;
        }
         */
        while (argv[i][0]=='-'&& strlen(argv[i])>1)
        {
            int k=1;
            bool _not_flag=false;
            while(argv[i][k]!=0)
            {
                _not_flag=(argv[i][k++]!='n');
                if(_not_flag)break;
            }
            if(!_not_flag){_newline=false;i++;}
            else break;
        }
        if(i<argc)
        {
            _output+=argv[i++];
        }
        for(;i<argc;i++)
        {
            _output+=" ";
            _output+=argv[i];
        }
        if(_newline)_output+="\n";
    }
    else _output="\n";
    strcpy(gTerm.strout,_output.c_str());
}
void doExit(int argc, char **argv)
{
    if(argc >= 2 && strcmp(argv[1],"--help")==0)
    {
        strcpy(gTerm.strout,"exit: exit [n]\n    Exit the shell.\n\n    Exits the shell with a status of N.  If N is omitted, the exit status\n    is that of the last command executed.");
        return;
    }
    if(!gTerm.multistages)
    {
        if(argc==1)exit(0);
        else exit(atoi(argv[1]));
    }
}
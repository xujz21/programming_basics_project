#include <string>
#include <cstdlib>
#include <cstring>
#include "global_var.h"
void doEcho(int argc, char **argv)
{
    std::string _output;
    if((argc==1)&&(strcmp(argv[0],"--help")==0))
    {
        _output="Usage: echo [SHORT-OPTION]... [STRING]...\n  or:  echo LONG-OPTION\nEcho the STRING(s) to standard output.\n\n  -n             do not output the trailing newline\n  -e             enable interpretation of backslash escapes\n  -E             disable interpretation of backslash escapes (default)\n      --help     display this help and exit\n      --version  output version information and exit\n\nIf -e is in effect, the following sequences are recognized:\n\n  \\\\      backslash\n  \\a      alert (BEL)\n  \\b      backspace\n  \\c      produce no further output\n  \\e      escape\n  \\f      form feed\n  \\n      new line\n  \\r      carriage return\n  \\t      horizontal tab\n  \\v      vertical tab\n  \\0NNN   byte with octal value NNN (1 to 3 digits)\n  \\xHH    byte with hexadecimal value HH (1 to 2 digits)\n\nNOTE: your shell may have its own version of echo, which usually supersedes\nthe version described here.  Please refer to your shell's documentation\nfor details about the options it supports.\n\nGNU coreutils online help: <https://www.gnu.org/software/coreutils/>\nReport echo translation bugs to <https://translationproject.org/team/>\nFull documentation at: <https://www.gnu.org/software/coreutils/echo>\nor available locally via: info '(coreutils) echo invocation'\n";
    }
    else
    {
        int i=0;
        bool _newline=true;
        if(strcmp(argv[0],"-n")==0)
        {
            _newline=false;
            i++;
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
    memset(gTerm.strout,'\0',sizeof(gTerm.strout));
    strcpy(gTerm.strout,_output.c_str());
}
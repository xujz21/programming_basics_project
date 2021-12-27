#include <string>
#include <cstdlib>
#include <cstring>
#include "global_var.h"
const char Echo_docs[]="Usage: echo [SHORT-OPTION]... [STRING]...\
\n  or:  echo LONG-OPTION\
\n Echo the STRING(s) to standard output.\
\n\
\n  -n             do not output the trailing newline\
\n      --help     display this help and exit\
\n";    

const char Exit_docs[]="exit: exit [n]\
\n    Exit the shell.\
\n\
\n    Exits the shell with a status of N.  If N is omitted, the exit status\
\n    is that of the last command executed.";
void doEcho(int argc, char **argv)
{
    std::string _output;
    if((argc==2)&&(strcmp(argv[1],"--help")==0))
    {
        _output=Echo_docs;
    }
    else if(argc>=2)
    {
        int i=1;
        bool _newline=true;
        for (;i<argc&&argv[i][0]=='-'&& strlen(argv[i])>1;i++)
        {
            int k=1;
            bool _not_flag=false;
            while(k<strlen(argv[i]))
            {
                _not_flag=(argv[i][k++]!='n');
                if(_not_flag)break;
            }
            if(!_not_flag){_newline=false;}
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
        strcpy(gTerm.strout,Exit_docs);
        return;
    }
    if(!gTerm.multistages)
    {
        if(argc==1)exit(0);
        else exit(atoi(argv[1]));
    }
}

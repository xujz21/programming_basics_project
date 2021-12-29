#include <string>
#include <cstdlib>
#include <cstring>
#include "global_var.h"
const char Echo_docs[]="\
Usage: echo [SHORT-OPTION]... [STRING]...\n\
  or:  echo LONG-OPTION\n\
Echo the STRING(s) to standard output.\n\
\n\
  -n             do not output the trailing newline\n\
  -e             enable interpretation of backslash escapes\n\
  -E             disable interpretation of backslash escapes (default)\n\
      --help     display this help and exit\n\
      --version  output version information and exit\n\
\n\
If -e is in effect, the following sequences are recognized:\n\
\n\
  \\\\      backslash\n\
  \\a      alert (BEL)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new line\n\
  \\r      carriage return\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
  \\0NNN   byte with octal value NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal value HH (1 to 2 digits)\n\
\n\
TinyShell online help: <https://www.gnu.org/software/coreutils/>\n\
Report echo bugs to <yuezq21@mails.tsinghua.edu.cn>\n\
Full documentation at: <https://www.gnu.org/software/coreutils/echo>\n\n";
const char Exit_docs[]="exit: exit [n]\
\n    Exit the shell.\
\n\
\n    Exits the shell with a status of N.  If N is omitted, the exit status\
\n    is that of the last command executed.";
inline int handle_args(char *arg,bool ipt)
{
    int n= (int)strlen(arg);
    int i=0;
    if(ipt)
    {
        while(i<n)
        {
            if(arg[i]=='\\')
            {
                i++;
                if(i>=n)
                {
                    cinStrout('\\');
                    break;
                }
                switch (arg[i]) {
                    case 'a':
                    {
                        cinStrout('\a');
                        i++;
                        break;
                    }
                    case 'b':
                    {
                        cinStrout('\b');
                        i++;
                        break;
                    }
                    case 'c':
                    {
                        return 1;
                    }
                    case 'e':
                    {
                        cinStrout('\033');
                        i++;
                        break;
                    }
                    case 'f':
                    {
                        cinStrout('\f');
                        i++;
                        break;
                    }
                    case 'n':
                    {
                        cinStrout('\n');
                        i++;
                        break;
                    }
                    case 'r':
                    {
                        cinStrout('\r');
                        i++;
                        break;
                    }
                    case 't':
                        cinStrout('\t');
                        i++;
                        break;
                    case 'v':
                        cinStrout('\v');
                        i++;
                        break;
                    case '\\':
                        cinStrout('\\');
                        i++;
                        break;
                    case '0': {
                        int char_idx = 0;
                        i++;

                        for (int iii = 0; iii < 3; iii++) {
                            if (arg[i] >= '0' && arg[i] <= '7') {
                                char_idx *= 8;
                                char_idx += arg[i] - '0';
                                i++;
                            } else break;
                        }
                        if(char_idx<128)
                            cinStrout((char) char_idx);
                        break;
                    }
                    case 'x':
                    {
                        int char_idx = 0;
                        i++;

                        for (int iii = 0; iii < 2; iii++) {
                            if (arg[i] >= '0' && arg[i] <= '9') {
                                char_idx *= 16;
                                char_idx += arg[i] - '0';
                                i++;
                            } else if (arg[i] >= 'A' && arg[i] <= 'F') {
                                char_idx *= 16;
                                char_idx += arg[i] - 'A'+10;
                                i++;
                            }else if (arg[i] >= 'a' && arg[i] <= 'f') {
                                char_idx *= 16;
                                char_idx += arg[i] - 'a'+10;
                                i++;
                            }
                            else break;
                        }
                        if(char_idx<128)
                            cinStrout((char) char_idx);
                        break;
                    }
                    default:
                        cinStrout('\\');
                        cinStrout(arg[i++]);
                        break;
                }
            }
            else cinStrout(arg[i++]);
        }
    }
    else
    {
        cinStrout(arg);
    }
    return 0;
}
void doEcho(int argc, char **argv)
{
    std::string _output;
    if((argc==2)&&(strcmp(argv[1],"--help")==0))
    {
        cinStrout(Echo_docs);
    }
    else if(argc>=2)
    {
        int i=1;
        bool _newline=true,_parse_bkslash=false;
        memset(gTerm.strout,0,sizeof(gTerm.strout));
        for (;i<argc&&argv[i][0]=='-'&& strlen(argv[i])>1;i++)
        {
            int k=1;
            bool _not_flag=false;
            for(;k<strlen(argv[i]);k++)
            {
                if(argv[i][k]!='n'&&argv[i][k]!='e'&&argv[i][k]!='E')_not_flag=true;
                if(_not_flag)break;
            }
            if(!_not_flag)
            {
                for(int j=1;j< strlen(argv[i]);j++)
                {
                    switch(argv[i][j])
                    {
                        case 'n':
                            _newline=false;break;
                        case 'e':
                            _parse_bkslash=true;break;
                        case 'E':
                            _parse_bkslash=false;break;
                    }
                }
            }
            else break;
        }
        for(;i<argc;i++)
        {
            if(handle_args(argv[i],_parse_bkslash))return;
            if(i<argc-1)cinStrout(' ');
        }
        if(_newline)cinStrout('\n');
    }
    else cinStrout('\n');

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

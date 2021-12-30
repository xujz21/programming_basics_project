/* part4.h -- All methods created by */
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
TinyShell online help: <https://github.com/Leo-Ngok/programming_basics_project>\n\
Report echo bugs to <yuezq21@mails.tsinghua.edu.cn>\n\
Full documentation at: <https://github.com/Leo-Ngok/programming_basics_project>\n\n";
const char Exit_docs[]="\
exit: exit [n]\
\n    Exit the shell.\
\n\
\n    Exits the shell with a status of N.  If N is omitted, the exit status\
\n    is that of the last command executed, which is zero.\n";
const char Echo_ver[]="\
echo (TinyShell) 1.10\n"
"There is NO WARRANTY, to the extent permitted by law.\n"
"\n"
"Written by Yue Zhangqiao.\n";
/**
 *  \Arguments
 *  char* arg : the string to be handled\n
 *  bool  ipt : whether or not the escape sequences should be handled\n
 *  \Returns
 *  an int, 0 when the method proceeded and quitted successfully\n
 *          1 when \\c does not exist or does not need handling\n
 *  \Description
 *      This method parses representations of the special characters, namely\n
 *      \\n, \\t, etc. to its actual form , i.e. \\x0A,... , when (ipt==true).\n
 *\n
 *      Then it puts the transformed string to the redirect output stream.\n
 *
 */
inline int handle_args(char *arg,bool ipt)
{
    int n= (int)strlen(arg);
    int i=0;

    if(ipt)
    {
        //Handle escape sequences, if any.
        //All escape sequences are defined to start
        //with a backslash '\'.
        while(i<n)
        {
            if(arg[i]=='\\')
            {
                i++;
                //if the backslash is itself the last
                //character, then put it to the stream directly.
                if(i>=n)
                {
                    cinStrout('\\');
                    break;
                }

                switch (arg[i]) {
                    //handles alert symbol
                    case 'a':
                    {
                        cinStrout('\a');
                        i++;
                        break;
                    }
                    //handles backspace symbol
                    case 'b':
                    {
                        cinStrout('\b');
                        i++;
                        break;
                    }
                    //abort the echo method
                    case 'c':
                    {
                        return 1;
                    }
                    //handles the escape symbol
                    case 'e':
                    case 'E':
                    {
                        cinStrout('\033');
                        i++;
                        break;
                    }
                    //
                    case 'f':
                    {
                        cinStrout('\f');
                        i++;
                        break;
                    }
                    //handles line feed, end of line symbol
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
                    //handles tab symbol
                    case 't':
                        cinStrout('\t');
                        i++;
                        break;
                     //handles vertical tab symbol
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
                        //handle the case of \0nnn
                        //saves the octal value to be shown as its
                        //corresponding ASCII representation.
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
                        //handle the case of \xHH
                        //saves the hexadecimal value to be shown as its
                        //corresponding ASCII representation.
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
                    //...when it is not a special character.
                    default:
                        cinStrout('\\');
                        cinStrout(arg[i++]);
                        break;
                }
            }
            else cinStrout(arg[i++]);
        }
    }
    else //when the string does not need 'special treatment'
    {
        cinStrout(arg);
    }
    return 0;
}
/**
 *  \Arguments
 *  int argc:    the number of command line arguments with regards to argv\n
 *  char** argv: a list of command line arguments expressed as string\n
 *  \Returns
 *  void, the method does return nothing.\n
 *  \Description
 *      This method is an analog to the 'echo' function in the\n
 *      GNU Bourne Again SHell, which handles the command line arguments\n
 *      to the standard output stream.\n
 *      When flag '-e' appeared, the shell handles all the escape sequences
 *      in the command line arguments.
 */
void doEcho(int argc, char **argv)
{
    if(argc==2)
    {
        //special cases
        if(strcmp(argv[1],"--help")==0)
            cinStrout(Echo_docs);
        else if(strcmp(argv[1],"--version")==0)
            cinStrout(Echo_ver);
        else goto normal_output;
    }
    else if(argc>=2)
    {
        normal_output:
        int i=1;
        bool _newline=true,_parse_bkslash=false;
        memset(gTerm.strout,0,sizeof(gTerm.strout));
        //handle flags first
        for (;i<argc&&argv[i][0]=='-'&& strlen(argv[i])>1;i++)
        {
            int k=1;
            bool _not_flag=false;
            //check if it is really a flag
            for(;k<strlen(argv[i]);k++)
            {
                if(argv[i][k]!='n'&&argv[i][k]!='e'&&argv[i][k]!='E')_not_flag=true;
                if(_not_flag)break;
            }
            //if it is a flag, then manipulate its corresponding options
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
        //put the line arguments to the output stream
        for(;i<argc;i++)
        {
            if(handle_args(argv[i],_parse_bkslash))return;
            if(i<argc-1)cinStrout(' ');
        }
        if(_newline)cinStrout('\n');
    }
    else cinStrout('\n');

}
/**
 *  \Arguments
 *  int argc:    the number of command line arguments with regards to argv\n
 *  char** argv: a list of command line arguments expressed as string\n
 *  \Returns
 *  void, the method does return nothing.\n
 *  \Description
 *      This method is an extra feature designed by the student taking part\n
 *      in module 4.\n
 *      This method would quit the shell safely.\n
 */
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

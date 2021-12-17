#include "global_var.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#define red "\e[91;1m"
#define green "\e[92;1m"
#define blue "\e[94;1m"
#define white "\e[0m"
#define MAX_FILE_NUM 10         // 最大可读取文件数量
#define MAX_CHAR_NUM 1000000    // 单个文件最大字符数
#define MAX_LINE_NUM 1000       // 文件最大行数
#define MAX_CHAR_PER_LINE 1000  // 单行最大字符数
#define MAX_RE 100              // 正则表达式最大长度
#define MAX_FILENAME 100        // 文件名最大长度

ifstream fin; // 读入文件流

struct Segment{
    int start_id;
    int end_id;
};

struct printLinetag{
    Segment Seg;
    bool Print;
};

struct File{ // 文件结构
    char filename[MAX_FILENAME];          // 文件名
    char filepath[200];                   // 文件的相对目录（相对wdir）
    int line_cnt = 0;                     // 文件行数
    char line[MAX_LINE_NUM][MAX_CHAR_PER_LINE]; // 按行记录文件内容
    printLinetag tag[MAX_LINE_NUM];       // 行输出标记
}file[MAX_FILE_NUM];
int file_cnt=0; // 当前已有文件数量

struct GrepArgs{
    char re[MAX_RE];           // 待匹配正则表达式 （最大字符数100）
    bool showLineCnt= false;   // 只显示符合样式的行数（仅可与 -H 叠加）
    bool showFileName = false; // 行首显示文件名称（单文件默认false）
    bool setH = false;
    bool ignoreCase = false;   // 忽略字符大小写
    bool showLineNum = false;  // 行首显示该行编号
    bool invert = false;       // 反向匹配
    int sucline = 0;           // 连续显示之后的行数
    int preline = 0;           // 连续显示之前的行数
};

bool gotRE = false; // 是否已经读到正则表达式
bool dp[100][MAX_CHAR_PER_LINE]; // 动态规划进行正则匹配

char grepHelpStr[]="  Usage: grep [OPTION]... PATTERNS [FILE]...\
\n  Search for PATTERNS in each FILE.\
\n  Example: grep -i 'hello world' menu.h main.c\
\n  PATTERNS can contain multiple patterns separated by newlines.\
\n\
\n  Pattern selection and interpretation:\
\n  -i,           ignore case distinctions in patterns and data\
\n  --h, --help   display this help text and exit\
\n  \
\n  Output control: \
\n  -n,           print line number with output lines\
\n  -H,           print file name with output lines\
\n  -h,           suppress the file name prefix on output\
\n  -c,           print only a count of selected lines per FILE\
\n  \
\n  Context control:\
\n  -B,           print NUM lines of leading context\
\n  -A,           print NUM lines of trailing context\
\n  -v,           inverse match\
\n  \
\n  When FILE is '-', read standard input.\
\n  With fewer than two FILEs, assume -h.\
\n  \
\n  Report bugs to: nzk20@mails.tsinghua.edu.cn\n\n";

char s[200];
void itos(int x){
    int t=0;
    while(x){
        s[t++] = x%10+'0';
        x/=10;
    }
    s[t]='\0';
    for(int i=0 ; i<t/2 ; i++){
        char tmp = s[i];
        s[i] = s[t-i-1];
        s[t-i-1] = tmp;
    }
}

bool enhanceEqual(char a, char b, bool ignore){
    if(!ignore) return(a==b);
    if( a==b || ((a-'a'+'A')==b && a>='a' && a<='z' && b>='A' && b<='Z')
        || ((b-'a'+'A')==a && b>='a' && b<='z' && a>='A' && a<='Z')) return true;
    return false;
}

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


int match(char* re, char* a, bool ignore){ // 从a开头开始匹配
    int s = strlen(re);
    int t = strlen(a);
    dp[0][0] = true;
    for(int i=1 ; re[i-1]=='*' ; i++)
        dp[i][0] = true;
    for(int i=1 ; i<=s ; i++)
        for(int j=1 ; j<=t ; j++){
            if(re[i-1]=='.')
                dp[i][j] = dp[i-1][j-1];
            else if(re[i-1]=='*')
                dp[i][j] = dp[i-1][j] || dp[i][j-1] || dp[i-1][j-1];
            else
                dp[i][j] = dp[i-1][j-1] && enhanceEqual(re[i-1], a[j-1], ignore);
        }
    for(int i=t ; i>0 ; i--)
        if(dp[s][i])
            return i-1; // 在索引为i处匹配成功(最长串)
    return -1; // 匹配不成功
}

Segment matchLine(char* re, char* singleLine, bool ignore){ // 时间复杂度O(st^2)
    Segment matchSegment;
    matchSegment.start_id = -1;
    matchSegment.end_id = -1;
    int linelen = strlen(singleLine);
    for(int i=0 ; i<linelen ; i++){
        int ans = match(re, singleLine+i, ignore);
        if(ans!=-1){
            matchSegment.start_id = i;
            matchSegment.end_id = i+ans;
            return matchSegment;
        }
    }
    return matchSegment;
}

void Analyse(GrepArgs args){ // 根据接收到的各种参数分析文件内容  
    if(file_cnt == 0){ // 无文件，报错返回（Linux中使用标准输入作为母字符串，此处简化）
        std::cerr<<"grep: You didn't input a file!\nTo use standard input, please set '-' as your filename."<<endl;
        return;
    }
    if(!args.setH && file_cnt>1)
        args.showFileName = true;
 
    // 开始分析!
    for(int f=0 ; f<file_cnt ; f++){ // 对每个文件（第f个文件）
        int* matchLineArr = new int[file[f].line_cnt];
        int lineidx=0, linecnt=0;
        for(int i=0 ; i<file[f].line_cnt ; i++){ // 确定行i是否包含匹配字符串，记录匹配区间
            file[f].tag[i].Seg = matchLine(args.re, file[f].line[i], args.ignoreCase);
            if(!args.invert){ // 正向匹配
                if(file[f].tag[i].Seg.start_id == -1) file[f].tag[i].Print = false;
                else{
                    file[f].tag[i].Print = true;
                    matchLineArr[lineidx++] = i;
                }
            }
            else{ // 反向匹配
                if(file[f].tag[i].Seg.start_id == -1){
                    file[f].tag[i].Print = true;
                    matchLineArr[lineidx++] = i;
                }
                else file[f].tag[i].Print = false;  
            }
        }

        linecnt = lineidx;

        for(int i=0 ; i<lineidx ; i++){ // 确定matchLineArr[i]前后行是否输出
            if(file[f].tag[matchLineArr[i]].Print){
                for(int q=1 ; q<=args.preline ; q++)
                    if( matchLineArr[i]-q >= 0 ) file[f].tag[matchLineArr[i]-q].Print = true;
                for(int p=1 ; p<=args.sucline ; p++)
                    if( matchLineArr[i]+p < file[f].line_cnt ) file[f].tag[matchLineArr[i]+p].Print = true;
            }
        }
        
        delete[] matchLineArr;

        if(args.showLineCnt){ // 只输出匹配行数
            if(args.showFileName){ // 是否输出文件名
                cinStrout(green); cinStrout(file[f].filename); cinStrout(blue); cinStrout(':'); cinStrout(white);
            }
            itos(linecnt);
            cinStrout(s);
            cinStrout('\n');
            continue;
        } 

        for(int i=0 ; i<file[f].line_cnt ; i++){ // 输出行i
            if(file[f].tag[i].Print){
                if(args.showFileName){ // 输出文件名
                    cinStrout(green); cinStrout(file[f].filename); cinStrout(blue);
                    if((file[f].tag[i].Seg.start_id!=-1 && !args.invert) || (file[f].tag[i].Seg.start_id==-1 && args.invert))
                         cinStrout(':');
                    else cinStrout('-'); 
                    cinStrout(white);
                }
                if(args.showLineNum){ // 输出行号
                    cinStrout(green);
                    itos(i+1);
                    cinStrout(s);
                    cinStrout(blue); 
                    if((file[f].tag[i].Seg.start_id!=-1 && !args.invert) || (file[f].tag[i].Seg.start_id==-1 && args.invert))
                        cinStrout(':'); 
                    else cinStrout('-');
                    cinStrout(white);
                }

                int leni = strlen(file[f].line[i]);
                if(file[f].tag[i].Seg.start_id == -1){ // 无有效匹配
                    cinStrout(file[f].line[i]);
                    cinStrout('\n');
                }
                else{ // 有效匹配：白-红-白
                    for(int c=0 ; c<file[f].tag[i].Seg.start_id ; c++) cinStrout(file[f].line[i][c]);
                    cinStrout(red);
                    for(int c=file[f].tag[i].Seg.start_id ; c<=file[f].tag[i].Seg.end_id ; c++) cinStrout(file[f].line[i][c]);
                    cinStrout(white);
                    for(int c=file[f].tag[i].Seg.end_id+1 ; c<leni ; c++) cinStrout(file[f].line[i][c]);
                    cinStrout('\n');
                }
            }
        }  
    }
}

void doGrep(int argc, char* argv[]){
    // 初始化
    gotRE = false;
    GrepArgs args;
    file_cnt = 0;

    // 解析参数
    for(int i=1 ; i<argc ; i++){ // argv[0]=="./grep"

        //只要出现--help，不管有多少个文件/参数，显示提示后返回
        if(strcmp(argv[i], "--help")==0 || strcmp(argv[i], "--h")==0){
            strcpy(gTerm.strout, grepHelpStr);
            return;
        }

        // 正则表达式 与 文件
        if(argv[i][0]!='-'){

            // 获取正则表达式: 首个不以'-'开头的字符串
            if(!gotRE){
                strcpy(args.re, argv[i]);
                gotRE = true;
            }

            // 文件：
            else{
                // *** 1.如何判断该参数是个文件: 不存在前缀-
                // *** 2.如何判断该文件存在: 使用读取文件流fstream是否为空来判断 
                // *** 3.如何判断命令行末尾参数有多文件的情况：循环读入，每次读入新的文件
                //（可设置文件最大读取数量为 MAX_FILE_NUM, 单个文件最多字符数 MAX_CHAR_NUM）
                strcat(file[file_cnt].filepath, gTerm.root);
                strcat(file[file_cnt].filepath, gTerm.wdir);
                strcat(file[file_cnt].filepath, argv[i]);
                fin.open(argv[i]); // 读入文件
                if(fin){ // 文件存在
                    while(!fin.eof()){
                        string tmp;
                        getline(fin, tmp);
                        int len = tmp.length();
                        for(int ii=0 ; ii<len ; ii++)
                            file[file_cnt].line[file[file_cnt].line_cnt][ii] = tmp[ii];
                        if(file[file_cnt].line[file[file_cnt].line_cnt][len-1]=='\n'
                            || file[file_cnt].line[file[file_cnt].line_cnt][len-1]=='\r')
                            file[file_cnt].line[file[file_cnt].line_cnt][len-1] = '\0'; // 去掉\n\r？
                        else file[file_cnt].line[file[file_cnt].line_cnt][len] = '\0';
                        file[file_cnt].line_cnt++;
                    }
                    strcpy(file[file_cnt].filename, argv[i]); // 记录文件名称
                    file_cnt++;
                }
                else if(!fin){ // 文件不存在
                    std::cerr<<"grep: "<<argv[i]<<": No such file or directory"<<endl;
                }
                fin.close();
            }    
        }

        // 设置参数 与 从标准输入流中读入
        else if(argv[i][0]=='-'){
            if(argv[i][1]=='\0'){ // 从标准输入读入字符                
                strcpy(file[file_cnt].line[0], gTerm.strin);
                file[file_cnt].line_cnt++;
                strcpy(file[file_cnt++].filename, "(standard input)"); // 记录文件名称
                continue;
            }
            if(argv[i][1] == 'c'){ // 只显示符合样式的行数
                args.showLineCnt = true; continue;                
            } 
            if(argv[i][1] == 'h'){ // 不显示文件名称
                args.showFileName = false;
                args.setH = true; continue;               
            }
            if(argv[i][1] == 'H'){ // 显示文件名称
                args.showFileName = true;
                args.setH = true; continue;               
            }
            if(argv[i][1] == 'I' || argv[i][1] == 'i'){ // 忽略字符大小写
                args.ignoreCase = true; continue;               
            }
            if(argv[i][1] == 'n'){ // 显示该行编号
                args.showLineNum = true; continue;               
            }
            if(argv[i][1] == 'A'){ // 显示该行之后的若干行
                args.sucline = argv[i][2]-'0'; continue;               
            }
            if(argv[i][1] == 'B'){ // 显示该行之后的若干行
                args.preline = argv[i][2]-'0'; continue;              
            }
            if(argv[i][1] == 'v'){ // 反向匹配（选择不匹配的行）
                args.invert = true; continue;            
            }
            // more args......

            std::cerr<<"grep: invalid option "<<red<<argv[i]<<white<<endl;
            std::cerr<<"Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep --help' for more information."<<endl;
        }
    }
    Analyse(args); // 传好了一堆参数
}

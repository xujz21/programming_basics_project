#include "global_var.h"
#include "fstream"
void doTee(int argc, char **argv)
{
    FILE *docw;
    bool append=false;
    std::ios_base::openmode mode;
    int i=0;
    if(strcmp(argv[0],"-a")==0)
    {
        append=true;
        mode=std::ios_base::app;
    }
    else
    {
        mode=std::ios_base::out;
    }
    i+=append;
    for(;i<argc;i++)
    {
        std::fstream fs;
        //fs.open((((std::string)gTerm.root)+((std::string)gTerm.wdir)+((std::string)argv[i])).c_str(),mode);
        fs.open("echo_docs.txt",mode);
        fs<<gTerm.strin;
        fs.close();
        strcpy(gTerm.strout,gTerm.strin);
    }
}
void doCat(int argc, char **argv)
{
    std::ofstream fout("doc1.txt");
}
void doCp(int argc, char **argv)
{

}
void doCd(int argc, char **argv)
{

}
void doPwd(int argc, char **argv)
{

}
#include "global_var.h"
#include <string>
#include <cstring>
#include "fstream"
std::string get_abs_path()
{
	std::string s;
	s=gTerm.root;
	s+=gTerm.wdir;
	return s;
}
void doTee(int argc, char **argv)
{
    std::ios_base::openmode mode;
    if(argc>=2)
    {
        int i=1;
        bool _append=false;
        for (;i<argc&&argv[i][0]=='-'&& strlen(argv[i])>1;i++)
        {
            int k=1;
            bool _not_flag=false;
            while(k<strlen(argv[i]))
            {
                _not_flag=(argv[i][k++]!='a');
                if(_not_flag)break;
            }
            if(!_not_flag){_append=true;}
            else break;
        }
    	if(_append)
    	{
        	mode=std::ios_base::app;
    	}
    	else
    	{
        	mode=std::ios_base::out;
    	}
    	i+=_append;
    	for(;i<argc;i++)
    	{
        	std::fstream fs;
        	std::string filepath;
        	if(argv[i][0]!='/')
        	{
        		filepath=get_abs_path();
        		filepath+="/";
        		filepath+=argv[i];
        	}
        	fs.open(filepath.c_str(),mode);
        	fs<<gTerm.strin;
        	fs.close();
    	}
    }
    strcpy(gTerm.strout,gTerm.strin);
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

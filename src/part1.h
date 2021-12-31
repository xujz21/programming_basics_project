#include "global_var.h"
using namespace std;
char rta[200],rtb[200];
char wda[200],wdb[200];

struct like{ 
	int sa_a;
	int sa_b;
};
like same[300];
int sai=1;

int s_g[6]={0,0,0,0,0,0};
//lines of files FILE1 and FILE2
string a[300],b[300];
//the position of the matching string, if any
int I=0; 

void sc() 
{
	ifstream fout("answer.txt");
	string ls,_tmp;
	while(!fout.eof()){
		getline(fout,ls);
		_tmp+=ls+"\n";
	}
	strcpy(gTerm.strout,_tmp.c_str());
}

void judge(int hang,int hx_b)
{
	for(int i=1;i<=hx_b;i++)
	{
        //no flags, then just compare
		if((s_g[0]==0)&&(s_g[1]==0)&&(s_g[2]==0)&&(s_g[3]==0)&&(s_g[4]==0)&&(s_g[5]==0)){
			if(a[hang]==b[i])
			{
				same[sai].sa_a=hang;
				same[sai].sa_b=i;
				sai++;
				return;
			}
		} 
		else
		{
            char as[100],bs[100];
            strcpy(as,a[hang].c_str());
            strcpy(bs,b[i].c_str());
            int alen=strlen(a[hang].c_str());
            int blen=strlen(b[i].c_str());
            // ignore changes in amount of whitespace
            if(s_g[0]==1)
            {
                int iba=0,ibb=0;
                for(iba;iba<=alen-1;iba++){
                        if(as[iba]==' '&&bs[iba]==' ')
                        {
                            //remove extra whitespace by
                            //moving the succeeding substring forward
                            while(as[iba+1]==' '){
                                for(int ils=iba+1;ils<=alen-2;ils++){
                                    as[ils]=as[ils+1];
                                }
                                as[alen-1]='\0';
                                alen--;
                            }
                            while(bs[iba+1]==' '){
                                for(int ils=iba+1;ils<=blen-2;ils++){
                                    bs[ils]=bs[ils+1];
                                }
                                bs[blen-1]='\0';
                                blen--;
                            }
                        }

                }
            }
            //ignore all whitespace (-w)
            if(s_g[4]==1)
            {
                int iwa=0,iwb=0;
                //moving trailing substring forward
                for(iwa;iwa<=alen-1;iwa++){
                    if(as[iwa]==' ')
                    {
                        while(as[iwa]==' '){
                                for(int ils=iwa;ils<=alen-2;ils++){
                                    as[ils]=as[ils+1];
                                }
                                as[alen-1]='\0';
                                alen--;
                            }
                    }
                }
                for(iwb;iwb<=blen-1;iwb++){
                    if(bs[iwb]==' ')
                    {
                        while(bs[iwb]==' '){
                                for(int ils=iwb;ils<=blen-2;ils++){
                                    bs[ils]=bs[ils+1];
                                }
                                bs[blen-1]='\0';
                                blen--;
                            }
                    }
                }

            }
            //same length of the lines is a necessary condition for
            //the lines to be equal.
            if(alen!=blen)continue;
            //ignore case (-i)
            if(s_g[2]==1)
            {
                //convert all big letters into small letters
                for(int ils=0;ils<=alen-1;ils++)
                {
                    if(as[ils]>='A'&&as[ils]<='Z')
                    as[ils]=as[ils]+' ';
                }
                for(int ils=0;ils<=blen-1;ils++)
                {
                    if(bs[ils]>='A'&&bs[ils]<='Z')
                    bs[ils]=bs[ils]+' ';
                }

            }
            //finally, when the lines are equal for the first time
            if(!(strcmp(as,bs)))
            {
                same[sai].sa_a=hang;
                same[sai].sa_b=i;
                sai++;
                return;
            }
		}
	}
}
//tg and ia are the two lines that same[tg]<=same[tg+ia]
void change(int tg,ofstream &fcg,int ia)
{
	//(-B) igonre blank lines
    //this branch takes blank lines into account
	if(s_g[1]==0)
	{ 
		if((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))
		{}
		else
		if((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b>1))
		{
            //"a" case: lines are added in FILE2
			fcg<<same[tg].sa_a<<"a"<<same[tg].sa_b+1<<endl;
			int ils=same[tg].sa_b+1;
			while(ils<same[tg+ia].sa_b){
				fcg<<">"<<b[ils]<<endl;
				ils++;
			}
		}
		else
		if((same[tg+ia].sa_a-same[tg].sa_a>1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))
		{
            //"d" case: lines are deleted in FILE2
			fcg<<same[tg].sa_a+1<<"d"<<same[tg].sa_b<<endl;
			int ils=same[tg].sa_a+1;
			while(ils<same[tg+ia].sa_a){
				fcg<<"<"<<a[ils]<<endl;
				ils++;
			}
		}
		else
		{
            //"c" cases: some lines are added, while some others are removed.
			fcg<<same[tg].sa_a+1<<"c"<<same[tg].sa_b+1<<endl;
            //the lines added
			int ilsa=same[tg].sa_a+1;
			while(ilsa<same[tg+ia].sa_a){
				fcg<<"<"<<a[ilsa]<<endl;
				ilsa++;
			}
			fcg<<"---"<<endl;
            //the lines removed
			int ilsb=same[tg].sa_b+1;
			while(ilsb<same[tg+ia].sa_b){
				fcg<<">"<<b[ilsb]<<endl;
				ilsb++;
			}
		}
	}
	else
	{
        //mark the empty lines
		bool *xa=new bool[same[tg+ia].sa_a-same[tg].sa_a-1];
		bool *xb=new bool[same[tg+ia].sa_b-same[tg].sa_b-1];
		for(int ils=0;ils<=same[tg+ia].sa_a-same[tg].sa_a-2;ils++)
		{
			if(a[same[tg].sa_a+ils+1]=="\0")xa[ils]=1;
			else xa[ils]=0;	
		} 
		for(int ils=0;ils<=same[tg+ia].sa_b-same[tg].sa_b-2;ils++)
		{
			if(b[same[tg].sa_b+ils+1]=="\0")xb[ils]=1;
			else xb[ils]=0;	
		} 
		//check if there is a non-empty line in between the lines differ.
		bool xlsa=0;
		for(int ils=0;ils<=same[tg+ia].sa_a-same[tg].sa_a-2;ils++)
			{
				if(xa[ils]==0)xlsa=1;
			}
		bool xlsb=0;
		for(int ils=0;ils<=same[tg+ia].sa_b-same[tg].sa_b-2;ils++)
			{
				if(xb[ils]==0)xlsb=1;
			}
		
		if((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))
		{}
		else
		{
            if(((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b>1))||xlsa==0)
            {
                if(xlsb==1)
                {
                    fcg<<same[tg].sa_a<<"a"<<same[tg].sa_b+1<<endl;
                    int ils=same[tg].sa_b+1;
                    int ilsb=-1;
                    while(ils<same[tg+ia].sa_b){
                        ilsb++;
                        ils++;
                        if(xb[ilsb]!=0)continue;
                        fcg<<">"<<b[ils]<<endl;
                    }
                }
            }
            else
            if(((same[tg+ia].sa_a-same[tg].sa_a>1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))||xlsb==0)
            {
                if(xlsa==1)
                {
                    fcg<<same[tg].sa_a+1<<"d"<<same[tg].sa_b<<endl;
                    int ils=same[tg].sa_a+1;
                    int ilsa=-1;
                    while(ils<same[tg+ia].sa_a){
                        ilsa++;
                        ils++;
                        if(xa[ilsa]!=0)continue;
                        fcg<<"<"<<a[ils+1]<<endl;
                    }
                }
            }
            else
            {
                fcg<<same[tg].sa_a+1<<"c"<<same[tg].sa_b+1<<endl;
                int ilsa=same[tg].sa_a;
                int ilsa_=-1;
                while(ilsa<same[tg+ia].sa_a-1){
                    ilsa_++;
                    ilsa++;
                    if(xa[ilsa_]!=0)continue;
                    fcg<<"<"<<a[ilsa]<<endl;
                }

                fcg<<"---"<<endl;


                int ilsb=same[tg].sa_b;
                int ilsb_=-1;
                while(ilsb<same[tg+ia].sa_b-1){
                    ilsb_++;
                    ilsb++;
                    if(xb[ilsb_]!=0)continue;
                    fcg<<">"<<b[ilsb]<<endl;
                }

            }
		}
		delete []xa;
		delete []xb;
	}
	
}

void doDiff(int argc, char **argv)
{
	if(argc==1)
	{
		cerr<<"diff: missing operand after 'diff'"<<endl;
		cerr<<"Try 'diff --help' for more information"<<endl;
		return;
	}
	
	char help1[]="--help";
	char *help2=argv[1];
	bool zs0=1; 
	if(argc==2)
	{
		if(strcmp(help2,help1)){
			cerr<<"diff: "<<help2<<": "<<"No such command"<<endl;
			cerr<<"Try 'diff --help' for more information"<<endl;
			zs0=0;
		}
	}
    //linux path
    strcpy(rta,gTerm.root);
    strcpy(rtb,gTerm.root);
    //windows path
    strcpy(wda,gTerm.root);
    strcpy(wdb,gTerm.root);
    //link the
    strcat(strcat(rta,"/"),argv[argc-2]);
    strcat(strcat(rta,"/"),argv[argc-1]);
    strcat(strcat(wda,gTerm.wdir),argv[argc-2]);
    strcat(strcat(wdb,gTerm.wdir),argv[argc-1]);
	bool zs1=1;
    //check if flags are valid
	for(int i=1;i<=argc-3;i++)
	{
		char *pd=argv[i];
		if(pd[1]!='I')
		{
            if((strcmp(pd,"-b")&&strcmp(pd,"-B")&&strcmp(pd,"-i")&&strcmp(pd,"-q")&&strcmp(pd,"-w")))
            {
                cerr<<"diff: "<<pd<<": "<<"No such command"<<endl;
                cerr<<"Try 'diff --help' for more information"<<endl;
                zs1=0;
            }
		}
		else if(pd[0]!='-'){
			cerr<<"diff: "<<pd<<": "<<"No such command"<<endl;
			cerr<<"Try 'diff --help' for more information"<<endl;
			zs1=0;
		}
	}
	
	bool zs2=1;
    //check if path exists
	if(argc!=2)
	{
		ifstream fina;
		if(gTerm.root[0]=='/') fina.open(rta);
		else fina.open(wda);
		if(!fina){
		cerr<<"diff: "<<argv[argc-2]<<": "<<"No such file or dictionary"<<endl;
		cerr<<"Try 'diff --help' for more information"<<endl;
		zs2=0;
		}
		fina.close();
		
		ifstream finb;
		if(gTerm.root[0]=='/') finb.open(rtb);
		else fina.open(wdb);
		if(!finb){
		cerr<<"diff: "<<argv[argc-1]<<": "<<"No such file or dictionary"<<endl;
		cerr<<"Try 'diff --help' for more information"<<endl;
		zs2=0;
		}
		finb.close();
		
	} 
	//continue only if all arguments and files are valid
	if((zs0==0)||(zs1==0)||(zs2==0))return; 
	//output documentation
	if(!(strcmp(help2,help1))){
		ofstream fout1("answer.txt");
		fout1<<"Usage: diff [OPTION]... FILES"<<endl;
		fout1<<"Compare FILES line by line."<<endl;
		fout1<<"-b, --ignore-space-change,--ignore changes in the amount of white space"<<endl;
		fout1<<"-B, --ignore-blank-line, --ignore changes where lines are all blank"<<endl;
		fout1<<"-i, --ignore-case, --ignore case differences in file contents"<<endl;
		fout1<<"-q, --brief--report only when files differ"<<endl;
		fout1<<"-w, --ignore-all-space,--ignore all white space"<<endl;
		fout1<<"-I, --ignore-matching-lines=RE,--ignore changes where all lines match RE"<<std::endl;
		fout1.close();
		sc();
	return;
	}
	//parse flags
	for(int i=1;i<=argc-3;i++){
		if(!strcmp(argv[i],"-b"))s_g[0]=1;
		if(!strcmp(argv[i],"-B"))s_g[1]=1;
		if(!strcmp(argv[i],"-i"))s_g[2]=1;
		if(!strcmp(argv[i],"-q"))s_g[3]=1;
		if(!strcmp(argv[i],"-w"))s_g[4]=1;
		if(argv[i][1]=='I')
		{s_g[5]=1;
		I=i;
		}
	}
	
	int hx_a=1;int hx_b=1;
    //the case when both are files
	if((strcmp(argv[argc-2],"-"))&&(strcmp(argv[argc-1],"-")))
	{
        //in linux
        if(gTerm.root[0]=='/')
        {
            ifstream fin1;
            fin1.open(rta);
            //push the file contents into the array of strings
            while(true)
            {
                getline(fin1,a[hx_a]);
                if(fin1.eof())break;
                hx_a++;
            }
            fin1.close();

            ifstream fin2;
            fin2.open(rtb);
            while(true)
            {
                getline(fin2,b[hx_b]);
                if(fin2.eof())break;
                hx_b++;
            }
            fin2.close();
        }
        //in windows
        else
        {
            ifstream fin1;
            fin1.open(wda);
            while(true)
            {
                getline(fin1,a[hx_a]);
                if(fin1.eof())break;
                hx_a++;
            }
            fin1.close();

            ifstream fin2;
            fin2.open(wdb);
            while(true)
            {
                getline(fin2,b[hx_b]);
                if(fin2.eof())break;
                hx_b++;
            }
            fin2.close();
        }
	
	}
	else 
	{
        //FILE1 is standard input
		if(!(strcmp(argv[argc-2],"-"))&&strcmp(argv[argc-1],"-"))
		{                             
			int i=0;
			while(!(gTerm.strin[i]=='\0'))
			{
				ofstream fout_strina1("-a.txt");
				while(gTerm.strin[i]!='\n'){
					fout_strina1<<gTerm.strin[i];
					i++;
				}
				fout_strina1.close();
				ifstream fin_strina2("-a.txt");
				getline(fin_strina2,a[hx_a]);
				hx_a++;
				i++; 
			}
		}
        //FILE2 is standard input
		else if(!(strcmp(argv[argc-1],"-"))&&strcmp(argv[argc-2],"-"))
		{
			int i=0;
			while(!(gTerm.strin[i]=='\0'))
			{
				ofstream fout_strinb1("-b.txt");
				while(gTerm.strin[i]!='\n'){
					fout_strinb1<<gTerm.strin[i];
					i++;
				}
				fout_strinb1.close();
				ifstream fin_strinb2("-b.txt");
				getline(fin_strinb2,b[hx_b]);
				hx_b++;
				i++; 
			}
		}
        //both are standard input
		else
		{
			int i=0;
			while(!(gTerm.strin[i]=='\0'))
			{
				ofstream fout_strina1("-a.txt");
				while(gTerm.strin[i]!='\n'){
					fout_strina1<<gTerm.strin[i];
					i++;
				}
				fout_strina1.close();
				ifstream fin_strina2("-a.txt");
				getline(fin_strina2,a[hx_a]);
				hx_a++;
				i++; 
			}
			
			int j=i+1;
			while(!(gTerm.strin[j]=='\0'))
			{
                ofstream fout_strinb1("-b.txt");
                while(gTerm.strin[j]!='\n'){
                    fout_strinb1<<gTerm.strin[i];
                    j++;
                }
                fout_strinb1.close();
                ifstream fin_strinb2("-b.txt");
                getline(fin_strinb2,b[hx_b]);
                hx_b++;
                j++;
			}
		}
	} 
	if(a[1]=="\0"&&hx_a==1)hx_a--;
	if(b[1]=="\0"&&hx_b==1)hx_b--;
	//consider the case when a matching string is provided
	if(s_g[5]==1)
	{
		char *Ibj=argv[I];
		char is[100];
		int i_ls=0;
		for(int i=strcspn(Ibj,"I")+1;i<strlen(Ibj);i++)
		{
			is[i_ls]=Ibj[i];
			i_ls++;
		}
		bool x=0;
        //if there is no match in some lines of FILE (1,2)
		for(int i=1;i<=hx_a;i++)
		{
			char as[100];
			strcpy(as,a[i].c_str());
			char *p=strstr(as,is);
			if(p==NULL){
				x=1;
			}
		}		
		for(int i=1;i<=hx_b;i++)
		{
			char bs[100];
			strcpy(bs,b[i].c_str());
			char *p=strstr(bs,is);
			if(p==NULL){
				x=1;
			}
		}			
		if(x==1){
			ofstream foutI("answer.txt");
			foutI.close();
			return;
		}
	} 
	 //to get a list of lines which are equal
	for(int hang=1;hang<=hx_a;hang++)
	{
		judge(hang,hx_b);
	}
    //define the boundary values
	same[0].sa_a=0;
	same[0].sa_b=0;
	same[sai].sa_a=hx_a+1;
	same[sai].sa_b=hx_b+1;
	// when -q is enabled, that is just to show whether they are diferent
	if(s_g[3]==1)
	{
		bool x=1;
		for(int i=0;i<=sai;sai++){
			if(same[i].sa_a!=same[i].sa_b){
			x=0;break;
			} 
		}
		if(x)
		{
            ofstream foutq("answer.txt");
            foutq<<"File [";
            foutq<<argv[argc-2];
            foutq<<"] and [";
            foutq<<argv[argc-1];
            foutq<<"] differ";
            foutq.close();
            sc();
            return;
		}
	}
	
	ofstream fcg("answer.txt");
	for(int tg=0;tg<=sai-1;tg++)
	{
		int ia=1;
		while(same[tg].sa_b-same[tg+ia].sa_b>0){
			ia++;
		}
		change(tg,fcg,ia);
		tg+=ia-1;
	}
	fcg.close();
	sc();
	return;

}

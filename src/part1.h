#include "global_var.h"

char rta[200],rtb[200];
char wda[200],wdb[200];//存储连接后文件路径 

struct like{//存储相同的行 
	int sa_a;
	int sa_b;
};
like same[300];
int sai=1;

int s_g[6]={0,0,0,0,0,0};//数组存储6个功能

string a[300],b[300];//用两个字符串数组存储比较内容

int I=0;//存出I所在的argv 

void sc()//将输出文件放入strout 
{
	ifstream fout("answer.txt");
	string ls;
	while(!fout.eof()){
	getline(fout,ls);
	}
	strcpy(gTerm.strout,ls.c_str());
}

void judge(int hang,int hx_b)//进行行之间相同的判断 
{
	for(int i=1;i<=hx_b;i++)
	{
		if((s_g[0]==0)&&(s_g[1]==0)&&(s_g[2]==0)&&(s_g[3]==0)&&(s_g[4]==0)&&(s_g[5]==0)){
			if(a[hang]==b[i])
			{
				same[sai].sa_a=hang;
				same[sai].sa_b=i;
				sai++;
				return;
			}
		}//基础判断 
		else
		{
		char as[100],bs[100];	
		strcpy(as,a[hang].c_str());
		strcpy(bs,b[i].c_str());//用两个字符数组存储比较字符串内容
		int alen=strlen(a[hang].c_str());
		int blen=strlen(b[i].c_str()); 
		
		if(s_g[0]==1)//判断是否 不检查空格字符的不同
		{
			int iba=0,ibb=0;
			for(iba;iba<=alen-1;iba++){
				    if(as[iba]==' '&&bs[iba]==' ')
					{
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
		}//将两个字符数组中的连在一起的空格变成一个 
		if(s_g[4]==1)
		{
			int iwa=0,iwb=0;
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
			
		}//去掉全部的空格字符 
		if(alen!=blen)continue;//判断长度是否相同 
		if(s_g[2]==1)
		{
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
			
		}//将所有的大写转成小写 
		
		if(!(strcmp(as,bs))) 
		{
			same[sai].sa_a=hang;
			same[sai].sa_b=i;
		    sai++;
			return;	
		}//进行判断是否相等 
		}
	}
}

void change(int tg,ofstream &fcg,int ia)
{
	
	if(s_g[1]==0)//判断是否忽略空白行
	{ 
		if((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))
		//相邻的两行相等 
		{}//无事发生
		else
		if((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b>1))
		//进行添加
		{
			fcg<<same[tg].sa_a<<"a"<<same[tg].sa_b+1<<endl;
			int ils=same[tg].sa_b+1;
			while(ils<same[tg+ia].sa_b){
				fcg<<">"<<b[ils]<<endl;
				ils++;
			}
		}
		else
		if((same[tg+ia].sa_a-same[tg].sa_a>1)&&(same[tg+ia].sa_b-same[tg].sa_b==1))
		//进行删减
		{
			fcg<<same[tg].sa_a+1<<"d"<<same[tg].sa_b<<endl;
			int ils=same[tg].sa_a+1;
			while(ils<same[tg+ia].sa_a){
				fcg<<"<"<<a[ils]<<endl;
				ils++;
			}
		}
		else
		//进行更换 
		{
			fcg<<same[tg].sa_a+1<<"c"<<same[tg].sa_b+1<<endl;
			int ilsa=same[tg].sa_a+1;
			while(ilsa<same[tg+ia].sa_a){
				fcg<<"<"<<a[ilsa]<<endl;
				ilsa++;
			}
			fcg<<"---"<<endl;
			int ilsb=same[tg].sa_b+1;
			while(ilsb<same[tg+ia].sa_b){
				fcg<<">"<<b[ilsb]<<endl;
				ilsb++;
			}
		}
	}
	else//忽略空白行 
	{
		bool *xa=new bool[same[tg+ia].sa_a-same[tg].sa_a-1];
		bool *xb=new bool[same[tg+ia].sa_b-same[tg].sa_b-1];//记录行是否空白
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
		//相邻的两行相等 
		{}//无事发生
		else
		{
		if(((same[tg+ia].sa_a-same[tg].sa_a==1)&&(same[tg+ia].sa_b-same[tg].sa_b>1))||xlsa==0)
		//进行添加
		{
			if(xlsb==1)//不是全零行 
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
		//进行删减
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
		//进行更换  
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
	strcpy(rta,gTerm.root);
	strcpy(rtb,gTerm.root);
	strcpy(wda,gTerm.root);
	strcpy(wdb,gTerm.root);
	strcat(strcat(rta,"\\"),argv[argc-2]);
	strcat(strcat(rtb,"\\"),argv[argc-1]);
	strcat(strcat(strcat(strcat(wda,"\\"),gTerm.wdir),"\\"),argv[argc-2]);
	strcat(strcat(strcat(strcat(wdb,"\\"),gTerm.wdir),"\\"),argv[argc-1]);
	
	char help1[]="--help";
	char *help2=argv[1];
	bool zs0=1;//检查--help是否拼写正确 
	if(argc==2)
	{
		if(strcmp(help2,help1)){
			cerr<<"diff: "<<help2<<": "<<"No such command"<<endl;
			zs0=0;
		}
	}
	
	bool zs1=1;//判断功能是否可执行 
	for(int i=1;i<=argc-3;i++)
	{
		char *pd=argv[i];
		if(pd[1]!='I')
		{
		if((strcmp(pd,"-b")&&strcmp(pd,"-B")&&strcmp(pd,"-i")&&strcmp(pd,"-q")&&strcmp(pd,"-w")))
		{
			cerr<<"diff: "<<pd<<": "<<"No such command"<<endl;
			zs1=0;
		}
		}
		else
		if(pd[0]!='-'){
			cerr<<"diff: "<<pd<<": "<<"No such command"<<endl;
			zs1=0;
		}
	}
	
	bool zs2=1;//判断文件是否存在
	if(argc!=2)
	{
		ifstream fina;
		if(gTerm.root[0]=='\\') fina.open(rta);
		else fina.open(wda);
		if(!fina){
		cerr<<"diff: "<<argv[argc-2]<<": "<<"No such file or dictionary"<<endl;
		zs2=0;
		}
		fina.close();
		
		ifstream finb;
		if(gTerm.root[0]=='\\') finb.open(rtb);
		else fina.open(wdb);
		if(!finb){
		cerr<<"diff: "<<argv[argc-1]<<": "<<"No such file or dictionary"<<endl;
		zs2=0;
		}
		finb.close();
		
	} 
	
	if((zs0==0)||(zs1==0)||(zs2==0))return; 
	
	if(!(strcmp(help2,help1))){
		ofstream fout1("answer.txt");
		fout1<<"-b, --ignore-space-change,--ignore changes in the amount of white space""-B, --ignore-blank-line, --ignore changes where lines are all blank""-i, --ignore-case, --ignore case differences in file contents""-q, --brief--report only when files differ""-w, --ignore-all-space,--ignore all white space""-I, --ignore-matching-lines=RE,--ignore changes where all lines match RE";
		fout1.close();
		sc();
	return;
	}//将help指令输出到文件中;退出进程 
	
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
	}//对输入的功能进行检测
	
	int hx_a=1;int hx_b=1;//存储文件的行数 
	if((strcmp(argv[argc-2],"-"))&&(strcmp(argv[argc-1],"-")))//判断是否从strin中读取 
	{
	if(gTerm.root[0]=='\\')
	{
		ifstream fin1;
		fin1.open(rta);
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
	
	}//完成从文件中的读取 
	else 
	{
		if(!(strcmp(argv[argc-2],"-"))&&strcmp(argv[argc-1],"-")) //判断那个读取strin
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
		else
		if(!(strcmp(argv[argc-1],"-"))&&strcmp(argv[argc-2],"-"))
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
	}//完成从strin中的读取（这里默认只有一个strin) 
	//前期工作完成，正式进入diff 
	if(a[1]=="\0"&&hx_a==1)hx_a--;
	if(b[1]=="\0"&&hx_b==1)hx_b--;//补丁：只在文件为空时减少行数 
	
	if(s_g[5]==1)//完成I的功能（有歧义) 
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
	 
	for(int hang=1;hang<=hx_a;hang++)
	{
		judge(hang,hx_b);
	}//进行行相同的判断
	 
	same[0].sa_a=0;
	same[0].sa_b=0;
	same[sai].sa_a=hx_a+1;
	same[sai].sa_b=hx_b+1;//将两文件的首尾视作相同
	
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
	}//完成q的功能
	
	ofstream fcg("answer.txt");
	for(int tg=0;tg<=sai-1;tg++)//进行调整 
	{
		int ia=1;
		while(same[tg].sa_b-same[tg+ia].sa_b>0){
			ia++;
		}//判断是否交叉 
		change(tg,fcg,ia);
		tg+=ia-1;
	}
	fcg.close();
	sc();
	return;

}

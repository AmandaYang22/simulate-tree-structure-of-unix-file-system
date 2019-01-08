#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sstream>
#include <unistd.h>
using namespace std;
string folderName[200];
string fileName[300];
string fileContent[300];
string newfileName[300];
string getFile[300];
string getPath[300];
int pathIndex[300]={0};
int fileNumber=1;
int flag=0;


void init_dir(struct directory *p);
void creat_dir_file(struct directory *p);
void get_folderName();
void get_fileName_Content(int length);
int if_dir(string s);
void creat_file(struct directory *p);
int readFileList(string path);
void search_file(char ch);
void modify_file_name(char ch,void(*func)(char ch));
void show_chmod(char ch,void(*func)(char c));
void mode_to_letter(int mode,char *str);
void show_id(char ch,void(*func)(char c));
void Depthtra(struct directory *dirent1,void(*func)(struct directory *dirent2));

string to_string(int x);
struct file{
	string name;
	string path;
};

struct directory{
	string name;
	string path;
	directory *parent;
	directory *next;
	directory *child;
	struct file fi;
};
struct directory *curDir;
int main()
{
	int length1=5,length2=100,length3=8;
	directory *dir=new directory;
	string path;
	get_folderName();
	get_fileName_Content(length1);
	get_fileName_Content(length2);
	get_fileName_Content(length3);

	init_dir(dir);

//	cout<<dir->name<<endl;

	Depthtra(dir,creat_dir_file);
	
/*	cout<<"你想进入的目录: ";
	cin>>path;
	readFileList(path);
*/  
	int choice;
	char ch1,ch2,ch3;
//	string path;
	cout<<"你想进行的操作:";
	cin>>choice;
	switch(choice){
	case 1:
		cout<<"你想进入的目录:";
		cin>>path;
	    readFileList(path);
		cout<<"你想查找的字符:";
		cin>>ch1;
		modify_file_name(ch1,search_file);
		break;
	case 2:
		cout<<"你想进入的目录:";
		cin>>path;
		readFileList(path);
		cout<<"你想查找的字符:";
		cin>>ch2;
		show_chmod(ch2,search_file);
		break;
	case 3:
		cout<<"你想进入的目录:";
		cin>>path;
		readFileList(path);
		cout<<"你想查找的字符:";
		cin>>ch3;
		show_id(ch3,search_file);
		break;
	default: ;
	}
	delete dir;
	return 0;
}

string to_string(int x)
{
	stringstream ss;
	ss<<x;
	return ss.str();
}

void get_folderName()
{
	int randNumber;
	string s,str;
	srand((unsigned)time(NULL));
	for(int j=0;j<100;j++)
	{
			for(int i=0;i<10;i++)
			{
				randNumber=rand()%10;
				str=to_string(randNumber);
				s=s+str;
			}
			folderName[j]=s;
			s="";
	}
}


void get_fileName_Content(int length)//length 5 8 100
{
	int strLength;
	int randomCount=300;
	string temp="";
	string randomString;
	char str[53]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	strLength=strlen(str);
	for(int i=0;i<randomCount;i++)
	{
			for(int j=0;j<length;j++)
			{
					temp=str[rand()%strLength];
					randomString=temp+randomString;
			}
			if(length==5)
			{
					fileName[i]=randomString;
					randomString="";

			}
			else if(length==8)
			{
					newfileName[i]=randomString;
					randomString="";
			}
			else if(length==100)
			{
					fileContent[i]=randomString;
					randomString="";
			}
	}
}

/*int if_dir(string s)
{
	int index=0;
	if((s[index]>='a' && s[index]<='z')||(s[index]>'A' &&s[index]<='Z'))
	{
			cout<<"is file"<<endl;
			return 1;//1  file
	}
	else if(s[index]>='0' &&s[index]<='9')
			return 2;//2 directory
	else
			cout<<"input error"<<endl;
}*/

void creat_file(struct directory *q)
{
	string symbol="/";
	srand((unsigned)time(NULL));
	int ranNum=rand()%3+1;
	for(int k=0;k<ranNum;k++)
	{
			FILE *fp;
			const char *content=fileContent[k+fileNumber].c_str();
			q->fi.name=fileName[k+fileNumber];
			q->fi.path=q->path+symbol+fileName[k+fileNumber];
			if((fp=fopen(q->fi.path.c_str(),"ab"))!=NULL)
			{
					cout<<"creat file success"<<endl;
					fputs(content,fp);
					fclose(fp);
					cout<<q->fi.path;
			}
	}
}

void init_dir(struct directory *p)
{
	string symbol="/";
	p->name="rootDir";
	p->path="/home/summer/rootDir";
	p->next=p->parent=p->child=NULL;
	if(mkdir(p->path.c_str(),0755)==0)
	{
		cout<<"mkdir rootDir success"<<endl;
		creat_file(p);
	//	Depthtra(p,creat_dir_file);

	}
	else
		cout<<"mkdir rootDir failed"<<endl;
}

/*void create_dir(directory *d)
{
			
}*/

void creat_dir_file(struct directory *p)
{

	string symbol="/";
	curDir=p;
	curDir->next=NULL;
	if(fileNumber>=80)
		cout<<"100 files have built"<<endl;
	else
	{
		for(int i=0;i<5;i++)
		{

			struct directory *newDir;
			newDir=(struct directory *)malloc(1000000*sizeof(struct directory));
	//		directory *newDir=new directory;
			newDir->name=folderName[fileNumber+i];
			newDir->path=curDir->path+symbol+folderName[fileNumber];

			newDir->next=curDir->child;
			newDir->parent=curDir;
			curDir->child=newDir;
			if(mkdir(newDir->path.c_str(),0755)==0)
			{
				creat_file(newDir);
				fileNumber++;
			}
		}
	}
}

void Depthtra(struct directory *dirent1,void(*func)(struct directory *dirent2))
{
		if(dirent1)
		{
			Depthtra(dirent1->next,func);
			func(dirent1);
			Depthtra(dirent1->child,func);
		}
}

int readFileList(string path)
{   
    DIR *dir;
    struct dirent *ptr;
	string symbol="/";
    char base[10000];
	
    if((dir=opendir(path.c_str()))==NULL)
    {
        cout<<"open dir error..."<<endl;
        exit(1);
    }
    while((ptr=readdir(dir))!=NULL)
    {
         if(strcmp(ptr->d_name,".")==0||strcmp(ptr->d_name,"..")==0)
               continue;
	     else if(ptr->d_type==8)  //file
		 {
	           cout<<ptr->d_name<<endl;
			   getFile[flag]=ptr->d_name;
			   getPath[flag]=path+symbol+ptr->d_name;
			   cout<<"getFile["<<flag<<"]: "<<getFile[flag]<<endl;
			   cout<<"getPath["<<flag<<"]: "<<getPath[flag]<<endl;

			   flag++;
	     }
	     else if(ptr->d_type==4) //dir
	     {
	           memset(base,'\0',sizeof(base));
	           strcpy(base,(path.c_str()));
	           strcat(base,"/");
	           strcat(base,ptr->d_name);
	           readFileList(base);
	     }
																				    }
																				    closedir(dir);
																					return 1;
}
void search_file(char ch)//,void(*func)(char ch))
{
	int X=0;
	for(int i=0;i<300;i++)
	{
		//cout<<"get_File[i].find(ch)"<<getFile[i].find(ch)<<"  ";
		if((getFile[i].find(ch))<getFile[i].length())
		{
			FILE *fp;
			char c;
	//		cout<<"找到"<<ch<<endl;
			cout<<"含有该字符的文件名:";
			cout<<getFile[i]<<endl;
			cout<<"含有该字符的文件所在路径: "<<endl;
			cout<<getPath[i]<<endl;
			//cout<<i<<endl;
			pathIndex[X]=i;
			
		//	cout<<"pathIndex["<<X<<"]:"<<pathIndex[X]<<endl;
			X++;
			cout<<"含有该字符的文件的内容:"<<endl;
			fp=fopen(getPath[i].c_str(),"r");
			while((c=fgetc(fp))!=EOF)
				cout<<c;
			fclose(fp);
		}
	//	else
	//		cout<<"not find"<<endl;
	}
/*	for(int k=0;k<300;k++)
	{
		if(pathIndex[k]!=0)
			cout<<"##path["<<k<<"]:"<<pathIndex[k]<<endl;
	}*/
	cout<<endl;
}


void modify_file_name(char ch,void(*func)(char c))
{
	int temp;
	string tempPath,tempFile,symbol="/";
	search_file(ch);

	for(int i=0;i<300;i++)
	{
		/*if(array[i]==NULL)
		{*/
		if(pathIndex[i]==0)
			break;
		else
		{
			//cout<<"!!! pathIndex["<<i<<"]"<<pathIndex[i]<<endl;
			temp=pathIndex[i];
			tempPath=getPath[temp];
			tempFile=getFile[temp];
	//		cout<<"$$$"<<tempPath<<endl;

			int pos=tempPath.find(tempFile);
			if(pos>-1)
			{
				tempPath.erase(pos,5);
			}
	//		cout<<"tempPath"<<tempPath<<endl;
			if(rename(getPath[temp].c_str(),(tempPath+newfileName[i]).c_str())==0)
			{
				cout<<tempPath<<" ";
				cout<<"重命名成功"<<endl;
			}
			else
				cout<<"重命名失败"<<endl;
	     }
	}

}
void mode_to_letter(int mode,char *str)
{
	if(S_ISDIR(mode)) str[0]='d'; //dir
	if(S_ISCHR(mode)) str[0]='c';//char
	if(S_ISBLK(mode)) str[0]='b'; //block
	if(mode &S_IRUSR) str[1]='r';
	else str[1]='-';
	if(mode &S_IWUSR) str[2]='w';
	else str[2]='-';
	if(mode &S_IXUSR) str[3]='x';
	else str[3]='-';
	if(mode &S_IRGRP) str[4]='r';
	else str[4]='-';
	if(mode &S_IWGRP) str[5]='w';
    else str[5]='-';
	if(mode &S_IXGRP) str[6]='x';
	else str[6]='-';
	if(mode &S_IROTH) str[7]='r';
	else str[7]='-';
	if(mode &S_IWOTH) str[8]='w';
	else str[8]='-';
	if(mode &S_IXOTH) str[9]='x';
	else str[9]='-';
	str[10]='\0';
}


void show_chmod(char ch,void(*func)(char c))
{
	int temp;

	char str[12];
	search_file(ch);

	for(int i=0;i<300;i++)
	{
		temp=pathIndex[i];
		if(temp!=0)
		{
			struct stat buf;
			stat(getPath[temp].c_str(),&buf);
			cout<<"修改之前:"<<endl;
			mode_to_letter(buf.st_mode,str);
			for(int j=1;j<11;j++)
				cout<<str[j];
			cout<<endl;
			if(chmod(getPath[temp].c_str(),0777)==0)
			{
				cout<<"修改"<<getPath[temp];
				cout<<"的权限成功"<<endl;
				cout<<"修改之后:"<<endl;
				struct stat buf2;
				stat(getPath[temp].c_str(),&buf2);
				mode_to_letter(buf2.st_mode,str);
		   	    for(int k=1;k<11;k++)
			        cout<<str[k];
				cout<<endl;
			}
			else
				cout<<"修改文件权限失败"<<endl;
		}
	}
}

void show_id(char ch,void(*func)(char c))
{
	int temp;
	uid_t uid;
	gid_t gid;

	search_file(ch);
	for(int i=0;i<300;i++)
	{
		temp=pathIndex[i];
		if(temp!=0)
		{
			 struct stat buf;
		     stat(getPath[temp].c_str(),&buf);
			 cout<<"修改前:"<<endl;
			 cout<<"该文件的uid: "<<buf.st_uid<<" ";
			 cout<<"该文件的gid: "<<buf.st_gid<<" ";
			 cout<<endl;
			 cout<<"修改后:"<<endl;
			 buf.st_uid=buf.st_uid+1;
			 //cout<<uid;
			 buf.st_gid=buf.st_gid+1;

			 chown(getPath[temp].c_str(),uid,gid);
		   	 cout<<"该文件的uid: "<<buf.st_uid<<" ";
		     cout<<"该文件的gid: "<<buf.st_gid<<" ";
			 cout<<endl;
		}
	}
}
			 

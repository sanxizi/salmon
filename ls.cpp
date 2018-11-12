#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>//下边两个是为了显示用户名与组名
#include <grp.h>
#include <sys/time.h>
using namespace std;

bool cmp( string s1,string s2){
    return s1<s2;
}

string fileType(char *fileName){
    struct stat buf;
    struct passwd *pw;
    struct group *gr;
    char s[100];
    struct tm *ttime;
    pw = getpwuid(getuid());
    gr = getgrgid(buf.st_gid);
    string type="";
    stat(fileName, &buf);
    switch(buf.st_mode & S_IFMT) //获取字符串的属性：普通文件-、目录d、字符设备c、块设备b、管道文件p、连接文件l、套接字文件s
    {
        case S_IFREG://普通文件
            type="-";
            break;
        case S_IFDIR:
            type="d";
            break;
        case S_IFCHR://字符装置
            type="c";
            break;
        case S_IFBLK://区块装置
            type="b";
            break;
        case S_IFIFO://先进先出
            type="p";
            break;
        case S_IFLNK://符号链接
            type="l";
            break;
        case S_IFSOCK://socket
            type="s";
            break;
    }
    switch(buf.st_mode & S_IRWXU)//当前用户
    {
        case 448:
            type+="rwx";
            break;
        case 384:
            type+="rw-";
            break;
        case 320:
            type+="r-x";
            break;
        case 192:
            type+="-wx";
            break;
        case 256:
            type+="r--";
            break;
        case 128:
            type+="-w-";
            break;
        case 64:
            type+="--x";
            break;
    }
    switch(buf.st_mode & S_IRWXG)//组用户权限
    {
        case 56:
            type+="rwx";
            break;
        case 48:
            type+="rw-";
            break;
        case 40:
            type+="r-x";
            break;
        case 24:
            type+="-wx";
            break;
        case 32:
            type+="r--";
            break;
        case 16:
            type+="-w-";
            break;
        case 8:
            type+="--x";
            break;
    }
    switch(buf.st_mode & S_IRWXO)//其他用户权限
    {
        case 7:
            type+="rwx";
            break;
        case 6:
            type+="rw-";
            break;
        case 5:
            type+="r-x";
            break;
        case 3:
            type+="-wx";
            break;
        case 4:
            type+="r--";
            break;
        case 2:
            type+="-w-";
            break;
        case 1:
            type+="--x";
            break;
    }
    ttime = localtime(&(buf.st_mtime));
    strftime(s,sizeof(s),"%Y-%m-%d %H:%M:%S",ttime);
    type+=to_string(buf.st_nlink);
    type+="  ";
    string n=pw->pw_name;
    type+=n;
    type+="  ";
    type+=gr->gr_name;
    type+="  ";
    type+=to_string(buf.st_size);
    type+="  ";
    type+=s;
    return type;
}

int main()
{
    DIR *dir;
    char s[100];
    string data[100];
    int tot=0;
    struct dirent *rent;
    dir =opendir(".");
    while(rent=readdir(dir))
    {
      strcpy(s,rent->d_name);
      if(s[0]!='.'){
       data[tot]=s;
      tot++;
      }
    }
    sort(data,data+tot,cmp);
    for(int i=0;i<tot;i++){
        char * fileName=(char*)data[i].data();
        cout<<fileType(fileName)<<"  "<<data[i]<<endl;
    }
    closedir(dir);
    return 0;
}


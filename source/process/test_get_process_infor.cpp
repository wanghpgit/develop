#include "process.hpp"
#include <iostream>
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/wait.h>


int errno;
extern char **environ;
void getNameById(pid_t pid);

int get_current_process_info()
{
	printf("开始创建临时文件\n");
	//execl("/usr/bin/gedit","gedit","/home/john/Desktop/process/makefile",(char * )0);
	char mm[]="tmp-XXXXXX";
	char* sfd=mktemp(mm);
	if(NULL==sfd)
	{
		printf("创建临时文件失败\n");
		exit(0);
	}
	printf("临时文件成功建立\n");
	
	pid_t nChild={0};
	int fd=open(sfd,O_CREAT|O_RDWR|O_TRUNC,0644);
	unlink(sfd);//文件关闭后自动删除

	int status;	
	pid_t wait_pid = wait(&status);
	close(fd);	
	printf("接下来是对各种ID 的操作\n");
	int uid=getuid();
	struct passwd *pwd=getpwuid(uid);
	printf("该进程的UID用户名是：%s\n",pwd->pw_name);
	printf("该进程的UID密码是：%s\n",pwd->pw_passwd);
	
	gid_t gid=pwd->pw_gid;
	struct group * gr_info=getgrgid(gid);
	printf("该进程的组名是：%s\n",gr_info->gr_name);
	
	getNameById(getpid());
	uid_t euid=geteuid();	
	pwd=getpwuid(euid);
	printf("该进程的EUID是:%s\n",pwd->pw_name);
	
	gid=getegid();
	gr_info=getgrgid(gid);
	printf("该进程的EGID是:%s\n",gr_info->gr_name);
	
	//获得root的用户名
	pwd=getpwnam("root");
	if(NULL!=pwd)
	{
		uid_t nUid=pwd->pw_uid;
		if(-1!=setuid(nUid))
			printf("设置root为UID成功\n");
		if(-1!=setgid(pwd->pw_gid))
			printf("设置root为GID成功\n");

	}	
	return 0;
}

void getNameById(pid_t pid)
{
	char proc_id_path[256]={0};
	char proc_name[256]={0};
	sprintf(proc_id_path,"/proc/%d/status",pid);
	int fd=open(proc_id_path,O_RDONLY,0644);
	if(-1!=fd)
	{
		read(fd,proc_name,256);
	}
	close(fd);
	char* index=strstr(proc_name,"State");
	if(NULL==index)
		return ;
	proc_name[index-proc_name]=0;
	printf("当前进程名称是：%s\n",proc_name);
}
*/

int main() {
	//get_current_process_info();
	std::vector<pid_t> vtPID;
	if (0 != wanghp::process::get_pid_by_name("tcp_test:worker", vtPID)) {
		std::cout << "get_pid_by_name faield!" << std::endl;
		return 1;
	}

	if (vtPID.empty()) {
		std::cout << "not find process" << std::endl;
		return 2;
	}

	for (std::vector<pid_t>::const_iterator cit = vtPID.begin();
		cit != vtPID.end(); ++cit) {
		std::cout << "pid:" << *cit << std::endl;
		wanghp::process::send_signal_by_pid(*cit, 3);
	}
	return 0;
}

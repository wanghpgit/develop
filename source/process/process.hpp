#ifndef __PROCESS_H
#define	__PROCESS_H
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/prctl.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <vector>
#ifdef __DEBUG
#include <iostream>
#endif

namespace wanghp{

/*
* 进程管理类
*/
class process {
public:
	process() {

	}

	~process() {

	}

	/*
	* 子进程基类
 	*/
	class child_process_base{
	public:
		child_process_base() {
	
		}

		~child_process_base() {

		}

		virtual int work() = 0;

	private:
		child_process_base(const child_process_base&);
		child_process_base & operator=(const child_process_base&);
	};

	/*
	* 进程信息类
	*/
	class proc_info{
	public:
		friend class process;
		proc_info(const std::string & name, child_process_base* ptr) throw(std::runtime_error)	{
			if (name.empty()) {
				throw std::runtime_error("process name is empty");
			} else if(name.length() > 16) {
				throw std::runtime_error("process name too long, must bu less than 17");
			} else if(NULL == ptr) {
				throw std::runtime_error("child_process_base ptr is null");
			} else {
				this->name = name;
				this->proc_ptr = ptr;
				this->pid = -1;
			}
		}
	
		const std::string & get_name() const {
			return name;
		}

		const child_process_base * get_ptr() const {
			return proc_ptr;
		}

		const pid_t get_pid() const {
			return pid;
		}

	private:
		proc_info(const proc_info&);
		proc_info & operator=(const proc_info&);		

		std::string			name;
		child_process_base*	proc_ptr;
		pid_t				pid;
	};
	
	enum create_result {
		CREATE_OK,		//创建成功
		CREATE_EAGAIN,	//创建进程数量达到上限
		CREATE_ENOMEM,	//创建进程系统内存不足
		CREATE_UN_ER,	//创建进程未知错误
		CHILD_OK,		//子进程正常结束 父进程拿不到这个结果
		CHILD_ER 		//子进程异常结束 父进程拿不到这个结果
	} createResult;
	
	/*
 	 * 创建子进程函数
 	*/
	create_result create_process(proc_info & info) {
		pid_t fpid = fork();
		if (0 > fpid) {
			if (EAGAIN == errno) {
				return CREATE_EAGAIN;
			} else if ( ENOMEM == errno) {
				return CREATE_ENOMEM;
			} else {
				return CREATE_UN_ER;			
			}
		} else if (0 == fpid) {
			 prctl(PR_SET_NAME, (unsigned long)info.name.c_str(), NULL, NULL, NULL);
			if (0 == info.proc_ptr->work()) {
				return CHILD_OK;
			} else {
				return CHILD_ER;
			}
		} else {
			info.pid = fpid;
			return CREATE_OK;
		}
	}

	/*
	* 批量创建子进程
	*/
	create_result create_process(std::vector<proc_info> & vt_info) {
		if (vt_info.empty()) {
			return CREATE_OK;
		}
		
		std::vector<pid_t> vt_pid;
		for (std::vector<proc_info>::iterator it = vt_info.begin(); it != vt_info.end(); ++it) {
			create_result ret = create_process(*it);
			if (CREATE_OK != ret) {
				kill_processes(vt_info);
				return ret;	
			} 
		}

		return CREATE_OK;
	}	

	/*
	* 获取指定进程名称的所有进程
	*/
	static int get_pid_by_name(const std::string & procName, std::vector<pid_t> & vtPid) {
		struct dirent * pstEnt = NULL;
	
		DIR * pDir = opendir("/proc");
		if (NULL == pDir) {
#ifdef __DEBUG
			std::cout << "opendir failed!" << std::endl;
#endif
			return 1;
		}

		while(NULL != (pstEnt = readdir(pDir))) {
			if ((std::string(".") == pstEnt->d_name) || (std::string("..") == pstEnt->d_name)) {
				continue;
			}

			if (DT_DIR != pstEnt->d_type) {
				continue;
			}

			std::string tmpPath("/proc/");
			tmpPath += pstEnt->d_name;
/*			tmpPath += "/cmdline"; //这个是进程的可执行文件名称
			FILE * pf = fopen(tmpPath.c_str(), "r");
			if (NULL == pf) {
#ifdef __DEBUG
				std::cout << "open file '" << tmpPath << "' failed!" << std::endl;
#endif
				continue;
			}

			char tmpName[1024] = {0};
			//int len = fread(tmpName, sizeof(char), sizeof(tmpName)/sizeof(char), pf);
#ifdef __DEBUG
			std::cout << "#pid#:" << pstEnt->d_name << std::endl;
#endif
			std::string tmp(tmpName);
			//std::string::size_type pos = tmp.rfind("/");
*/
			tmpPath += "/status";
			std::fstream fs(tmpPath.c_str(), std::ios::in);
			if (fs.bad()) {
#ifdef __DEBUG
				std::cout << "open file '" << tmpPath << "' failed!" << std::endl;			
#endif
				continue;
			}			

			std::string tmp;			
			if(!std::getline(fs, tmp)) {
#ifdef __DEBUG
				std::cout << "read file '" << tmpPath << "'  failed!" << std::endl;			
#endif
				continue;

			}
	
			std::string::size_type pos = tmp.rfind("Name:");
			if (std::string::npos == pos) {
				continue;
			}
#ifdef __DEBUG
			std::cout << "1 tmp:" << tmp << std::endl;
#endif
			tmp=tmp.substr(pos+6);
#ifdef __DEBUG
			std::cout << "2 tmp:" << tmp << std::endl;
#endif
		
			if(tmp ==  procName) {
#ifdef __DEBUG
				std::cout << tmp << std::endl;
#endif
				std::stringstream strFmt;
				strFmt << pstEnt->d_name;
				pid_t pid;
				strFmt >> pid;
				vtPid.push_back(pid);
			}
		} 
	
		closedir(pDir);
		return 0;	
	}

	typedef enum kill_result {
		RET_OK,		/*执行成功*/
		RET_EINVAL, /*信号不合法*/
		RET_EPERM,	/*权限不够，无法发送信号到指定进程*/
		RET_ESRCH,	/*进程或进程组不存在*/
		RET_UNKNOWN	/*未知错误*/
	} killResult;
	
	/*
	* 向指定进程ID的进程发送指定信号
	*/
	static kill_result send_signal_by_pid(const std::string & pid, int signal) {
		std::stringstream strFmt;
		strFmt << pid;
		int iPid;
		strFmt >> iPid;
		return send_signal_by_pid(iPid, signal);
	}

	/*
	* 向指定进程ID的进程发送指定信号
	*/
	static kill_result send_signal_by_pid(pid_t pid, int signal) {
		if(pid == waitpid(pid, NULL, WNOHANG)){
			RET_OK;
		}

		if (!kill(pid, signal)) {
			return RET_OK;
		}
		
		switch (errno) {
			default:
				return RET_UNKNOWN;	
			case EINVAL:
				return RET_EINVAL;
			case EPERM:
				return RET_EPERM;
			case ESRCH:
				return RET_ESRCH;
		}
	}

private:
	void kill_processes(std::vector<proc_info> & vt_info){
		for (std::vector<proc_info>::iterator it = vt_info.begin(); it != vt_info.end(); ++it) {
			if (-1 != it->pid) {
				send_signal_by_pid(it->pid, SIGINT);
				it->pid = -1;
			}
		}
	}
}; 

}

#endif//__PROCESS_H

#ifndef MY_THREAD_POOL_H
#define MY_THREAD_POOL_H
#include <queue>
#include <boost/thread.hpp>
#include <boost/thread/thread_pool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#ifdef _DEBUG
#include <iostream>
#endif

//Task_queue :
//*************************************************************************
//Copyright: 
//Author:     Sail
//Filename:   My_task_queue.h 
//Last Mod time: 
//*************************************************************************
//Remarks:
//任务队列用来管理一系列的任务，多个工作线程阻塞在队列的条件变量上，当有任务
//加入时，则唤醒工作线程，工作完毕后继续阻塞
//
//*************************************************************************
 
//定义任务
class my_thread_pool;

typedef class Ctask {
public:
	friend class my_thread_pool;
	Ctask(){}
	~Ctask(){}

private:
	virtual void work() = 0;
} *my_task; 

class task_queue:boost::noncopyable
{
private:
	friend class my_thread_pool;
	task_queue() 
	{

	}

	~task_queue()
	{

	}

    void push_task(const my_task & task_func)
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        m_queue.push(task_func);
        m_cond.notify_one(); 
    }

    my_task get_task()
    {
        boost::unique_lock<boost::mutex> lock(m_mutex);
        if(m_queue.size()==0)
        {
            m_cond.wait(lock);
        }
        my_task task(m_queue.front());
        m_queue.pop();
        return task;
    }

    int get_size()
    {
        return m_queue.size();
    }

private:
    std::queue<my_task> 			m_queue;
    boost::condition_variable_any 	m_cond;
    boost::mutex 					m_mutex;
};

//My_thread_pool:
//*************************************************************************
//Copyright: 
//Author:     Sail
//Filename:    
//Last Mod time: My_thread_pool
//*************************************************************************
//Remarks:
//线程池使用boost中的thread_group来管理和创建工作线程，使其阻塞在任队列中
//
//*************************************************************************
class my_thread_pool:boost::noncopyable
{
public:
	my_thread_pool(size_t thread_num, size_t max_task_count):m_thread_pool(thread_num), 
	m_max_task_count(max_task_count),
	m_dispatch_thread(boost::bind(&my_thread_pool::task_dispatch, this)){
			
	}

	~my_thread_pool(){}
 
    bool  post(my_task task)
    {
		if (m_queue.get_size() >= m_max_task_count) {
			return false;
		}

        m_queue.push_task(task);

		return true;
    }
 
    void stop()
    {
       m_is_run = false; 
    }

	void wait()
	{
		m_dispatch_thread.join();
	}
     
private:
    task_queue 					m_queue;
    boost::thread_pool			m_thread_pool;
	size_t						m_max_task_count;
	volatile bool 				m_is_run;
	boost::thread				m_dispatch_thread;

	void task_dispatch() {
#ifdef _DEBUG
		std::cout << "task dispatch thread start! id:" << boost::this_thread::get_id() << std::endl;
#endif
		while(m_is_run) 
		{
			my_task task = m_queue.get_task();
			m_thread_pool.schedule(boost::bind(&Ctask::work, task));	
		}
#ifdef _DEBUG
		std::cout << "task dispatch thread stop! id:" << boost::this_thread::get_id() << std::endl;
#endif
	}
};
 
#endif//MY_THREAD_POOL_H

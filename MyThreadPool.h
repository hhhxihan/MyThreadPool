#include "safeQueue.h"
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>



class MyThreadPool{
    private:
        //任务队列queue
        std::vector<std::thread> threadPool;
        std::condition_variable cv; //
        SafeQueue<void (*)()> task; //任务队列
        int task_size; //当前任务数量
        std::mutex cv_mux; //互斥锁

        class threadfunc{ //类必须在MythreadPool类中声明，因为用到全局的condition_variable;
            private:
            void (*p)(); //函数指针，用于取函数
            public:
                void operator()(){//重载操作符，当作函数对象用
                    //从任务队列中取任务
                    //如果任务队列为空，在挂起，等待唤醒
                    //考虑如何实现这个功能；互斥锁，只能实现对资源的互斥访问
                    //信号量，标记有多少资源量，如果没有资源了，阻塞线程；等有任务了，再唤醒；可以
                    //条件变量，条件设为任务队列是不是空。如果为空，那么wait挂起等嗲唤醒
                    std::unique_lock(cv_mun);
                    if(task)

                }
        }
    public:
        MyThreadPool(int thread_num=10):threadPool(thread_num){}   //构造函数

        void init(){
            for(int i=0;i<threadPool.size();i++){
                threadPool[i]=std::thread(); 
            }
        }
};
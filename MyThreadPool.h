#include "safeQueue.h"
#include <mutex>
#include <thread>
#include <vector>
#include <funcitonal>
#include <memory>
#include <condition_variable>



class MyThreadPool{
    private:
        //任务队列queue
        std::vector<std::thread> threadPool;
        std::condition_variable cv; //
        bool is_shutdown;
        SafeQueue::SafeQueue<std::function<void()>> task; //任务队列
        int task_size; //当前任务数量
        std::mutex cv_mux; //互斥锁

        class threadfunc{ //类必须在MythreadPool类中声明，因为用到全局的condition_variable;
            private:
                MyThreadPool* thispool;
                std::function<void()> func;   //用function来封装函数
            
            public:
                void operator()(){//重载操作符，当作函数对象用
                    //从任务队列中取任务
                    //如果任务队列为空，在挂起，等待唤醒
                    //考虑如何实现这个功能；互斥锁，只能实现对资源的互斥访问
                    //信号量，标记有多少资源量，如果没有资源了，阻塞线程；等有任务了，再唤醒；可以
                    //条件变量，条件设为任务队列是不是空。如果为空，那么wait挂起等嗲唤醒
                    while(!is_shutdown){
                        std::unique_lock lock(thispool->cv_mun);
                        if(task.empty()){
                            //任务列表为空
                            thispool->cv.wait(lock);
                        }
                        //此处为获得了锁，且任务队列不为空
                        thispool->task.front_pop(func);
                        
                        func();
                    }
                    

                }
        }
    public:
        MyThreadPool(int thread_num=10):threadPool(thread_num){}   //构造函数

        void init(){ //初始化线程池
            for(int i=0;i<threadPool.size();i++){
                threadPool[i]=std::thread(); 
            }
        }

        template<typename F,typename ...Args> //目前不是一个异步函数
        auto submit(F &&f,Args &&...args)->decltype(f(...args)){
            auto func=std::bind(std::forward<F>(f),std::bind<Args>(args)...);
            auto funcPtr=std::make_share<decltype(f(args...))>(func);
            
            std::function<void()> myfunc=[funcPtr](){
                (*funcPtr)();
            }

            task.push_back(myfunc);
            cv.notify_one(); //唤醒一个线程
        }
};